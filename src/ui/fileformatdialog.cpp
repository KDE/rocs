/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2014       Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fileformatdialog.h"
#include "libgraphtheory/fileformats/fileformatmanager.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include <settings.h>
#include <KLocalizedString>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <KMessageBox>
#include <QUrl>
#include <QFile>
#include <QPointer>

using namespace GraphTheory;

FileFormatDialog::FileFormatDialog(QObject* parent)
    : QObject(parent)
{
}

bool FileFormatDialog::exportFile(GraphDocumentPtr document) const
{
    FileFormatManager manager;

    //TODO rewrite this whole class, it is a wonder that the code works :)
    QString ext;
    QList<FileFormatInterface*> exportBackends = manager.backends(FileFormatManager::Export);
    foreach(FileFormatInterface * f, exportBackends) { //TODO fragile code
        ext.append(f->extensions().join(";;"));
    }
    ext.append(i18n("*|All files"));

    QUrl startDirectory = QUrl::fromLocalFile(Settings::lastOpenedDirectory());

    QPointer<QFileDialog> exportDialog = new QFileDialog(qobject_cast< QWidget* >(parent()), QString(), QString(), ext);
    exportDialog->setLabelText(QFileDialog::Accept, i18nc("@action:button", "Export"));
    if (exportDialog->exec() != QDialog::Accepted) {
        return false;
    }

    if (exportDialog->selectedFiles().isEmpty()) {
        return false;
    }

    // set file ending
    QUrl file;
    ext = exportDialog->selectedNameFilter().remove('*');
    if (exportDialog->selectedFiles().first().endsWith(ext)) {
        file = QUrl::fromLocalFile(exportDialog->selectedFiles().first());
    } else {
        file = QUrl::fromLocalFile(exportDialog->selectedFiles().first().append(ext));
    }

    // test if any file is overwritten
    if (QFile::exists(exportDialog->selectedFiles().first())) {
        if (KMessageBox::warningContinueCancel(qobject_cast< QWidget* >(parent()), i18n(
                "<p>The file <br /><strong>'%1'</strong><br /> already exists; if you "
                "do not want to overwrite it, change the file name to "
                "something else.</p>", file.toDisplayString()),
            i18n("File Exists"), KGuiItem(i18n("Overwrite") ))
            == KMessageBox::Cancel ) {
            return false;
        }
    }

    // select plugin by extension
    FileFormatInterface * filePlugin = manager.backendByExtension(ext);
    if (!filePlugin) {
        qDebug() << "Cannot export file: " << file.toLocalFile();
        return false;
    }

    filePlugin->setFile(file);
    filePlugin->writeFile(document);
    if (filePlugin->hasError()) {
        qDebug() << "Error occurred when writing file: " << filePlugin->errorString();
        return false;
    }

    Settings::setLastOpenedDirectory(file.path());
    return true;
}

GraphDocumentPtr FileFormatDialog::importFile()
{
    FileFormatManager manager;

    QString ext;
    QList<FileFormatInterface*> importBackends = manager.backends(FileFormatManager::Import);
    foreach(FileFormatInterface * f, importBackends) {
        ext.append(f->extensions().join(""));
    }
    ext.append(i18n("*|All files"));

    QPointer<QFileDialog> dialog = new QFileDialog(qobject_cast< QWidget* >(parent()));
//     dialog->setCaption(i18nc("@title:window", "Import Graph File into Project")); //FIXME commented out for porting
    if (!dialog->exec()) {
        return GraphDocumentPtr();
    }

    qDebug() << "Extensions:" << ext;
    QString fileName = dialog->selectedFiles().first();
    if (fileName.isEmpty()) {
        return GraphDocumentPtr();
    }

    int index = fileName.lastIndexOf('.');
    FileFormatInterface * filePlugin = 0;
    if (index == -1) {
        qDebug() << "Cannot open file without extension.";
        return GraphDocumentPtr();
    }

    qDebug() << fileName.right(fileName.count() - index);
    filePlugin =manager.backendByExtension(fileName.right(fileName.count() - index));

    if (!filePlugin) {
        qDebug() <<  "Cannot handle extension " <<  fileName.right(3);
        return GraphDocumentPtr();
    }

    filePlugin->setFile(QUrl::fromLocalFile(fileName));
    filePlugin->readFile();
    if (filePlugin->hasError()) {
        qDebug() << "Error loading file" << fileName << filePlugin->errorString();
        return GraphDocumentPtr();
    }
    else {
        return filePlugin->graphDocument();
    }
}

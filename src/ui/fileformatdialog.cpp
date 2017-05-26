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
#include <QRegularExpression>

using namespace GraphTheory;

FileFormatDialog::FileFormatDialog(QObject* parent)
    : QObject(parent)
{
}

bool FileFormatDialog::exportFile(GraphDocumentPtr document) const
{
    FileFormatManager manager;

    QStringList nameFilter;
    QList<FileFormatInterface*> exportBackends = manager.backends(FileFormatManager::Export);
    foreach(FileFormatInterface * f, exportBackends) { //TODO fragile code
        nameFilter << f->extensions();
    }

    QUrl startDirectory = QUrl::fromLocalFile(Settings::lastOpenedDirectory());

    QPointer<QFileDialog> exportDialog = new QFileDialog(qobject_cast< QWidget* >(parent()));
    exportDialog->setNameFilters(nameFilter);
    exportDialog->setLabelText(QFileDialog::Accept, i18nc("@action:button", "Export"));
    if (exportDialog->exec() != QDialog::Accepted) {
        return false;
    }

    if (exportDialog->selectedFiles().isEmpty()) {
        return false;
    }

    // set file ending
    const QUrl file = QUrl::fromLocalFile(exportDialog->selectedFiles().first());

    // test if any file is overwritten
    if (QFile::exists(exportDialog->selectedFiles().first())) {
        if (KMessageBox::warningContinueCancel(qobject_cast< QWidget* >(parent()), i18n(
                "<p>The file <br /><strong>'%1'</strong><br /> already exists; if you "
                "do not want to overwrite it, change the file name to "
                "something else.</p>", file.toDisplayString()),
            i18n("File Exists"), KStandardGuiItem::overwrite())
            == KMessageBox::Cancel ) {
            return false;
        }
    }

    // select plugin by extension
    const QString filter = exportDialog->selectedNameFilter();

    // find match for "(*.foo)"
    QRegularExpressionMatch match;
    filter.lastIndexOf(QRegularExpression("\\*\\.[a-zA-Z0-9]+"), -1, &match);
    const QString ext = match.captured(0).right(match.captured(0).length() - 2);
    FileFormatInterface * filePlugin = manager.backendByExtension(ext);
    if (ext == QString() || !filePlugin) {
        KMessageBox::error(qobject_cast< QWidget* >(parent()), i18n(
            "<p>Cannot resolve suffix of file <strong>'%1'</strong> to an available file backend."
            "Aborting export.</p>",
            file.toDisplayString()));
        qCritical() << "Cannot export file, since cannot find plugin for extension: " << file.toLocalFile() << ext;
        return false;
    }

    filePlugin->setFile(file);
    filePlugin->writeFile(document);
    if (filePlugin->hasError()) {
        KMessageBox::error(qobject_cast< QWidget* >(parent()), i18n(
            "<p>Error occured when writing file: <strong>'%1'</strong></p>", filePlugin->errorString()));
        qCritical() << "Error occurred when writing file: " << filePlugin->errorString();
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

    QPointer<QFileDialog> dialog = new QFileDialog(qobject_cast< QWidget* >(parent()),
                                                i18nc("@title:window", "Import Graph File into Project"));
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

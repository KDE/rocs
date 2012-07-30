/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

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

#include "ImporterExporterManager.h"
#include <KFileDialog>
#include <KLocalizedString>
#include "Plugins/FilePluginInterface.h"
#include <KDebug>
#include <PluginManager.h>
#include <KPushButton>
#include <kdebug.h>
#include "Document.h"

#include <QFile>
#include <QPointer>

#include <iostream>
#include <string>

ImporterExporterManager::ImporterExporterManager(QObject* parent): QObject(parent), _scriptToRun(QString())
{
}

bool ImporterExporterManager::exportFile(Document * doc) const
{
    QString ext;
    foreach(FilePluginInterface * f, PluginManager::instance()->filePlugins()) {
        ext.append(f->extensions().join(""));
    }
    ext.append(i18n("*|All files"));


    QPointer<KFileDialog> exportDialog = new KFileDialog(QString(), ext, qobject_cast< QWidget* >(parent()));
    exportDialog->okButton()->setText(i18nc("@action:button", "Export"));
    exportDialog->okButton()->setToolTip(i18nc("@info:tooltip", "Export graphs to file"));
    if (exportDialog->exec() != KDialog::Accepted) {
        return false;
    }

    kDebug() << "Exporting File..";
    if (exportDialog->selectedFile().isEmpty()) {
        return false;
    }

    ext = exportDialog->currentFilter().remove('*');
    kDebug() << " Selected to export: " << ext;
    QString file = exportDialog->selectedFile();
    if (!file.endsWith(ext)) {
        file.append(ext);
    }

    FilePluginInterface * p = PluginManager::instance()->filePluginsByExtension(ext);
    if (!p) {
        kDebug() << "Cannot export file: " << file;
        return false;
    }

    if (!p->writeFile(*doc, file)) {
        kDebug() << "Error writing file: " << p->lastError();
        return false;
    }
    kDebug() << "File Exported!" << file;
    return true;
}

Document* ImporterExporterManager::importFile()
{
    QString ext;

    foreach(FilePluginInterface * f, PluginManager::instance()->filePlugins()) {
        ext.append(f->extensions().join(""));
    }
    ext.append(i18n("*|All files"));

    QPointer<KFileDialog> dialog = new KFileDialog(QString(), ext, qobject_cast< QWidget* >(parent()));
    dialog->setCaption(i18nc("@title:window", "Import Graph File into Project"));
    if (!dialog->exec()) {
        return 0;
    }

    kDebug() << "Extensions:" << ext;
    QString fileName = dialog->selectedFile();
    if (fileName.isEmpty()) {
        return 0;
    }

    int index = fileName.lastIndexOf('.');
    FilePluginInterface * f = 0;
    if (index == -1) {
        kDebug() << "Cannot open file without extension.";
        return 0;
    }

    kDebug() << fileName.right(fileName.count() - index);
    f = PluginManager::instance()->filePluginsByExtension(fileName.right(fileName.count() - index));

    if (!f) {
        kDebug() <<  "Cannot handle extension " <<  fileName.right(3);
        return 0;
    }

    Document * gd = f->readFile(fileName);
    if (!gd) {
        kDebug() << "Error loading file" << fileName << f->lastError();
    }
    _scriptToRun = f->scriptToRun();
    return gd;
}

void ImporterExporterManager::dialogExec()
{

}

bool ImporterExporterManager::hasDialog()
{
    return false;
}

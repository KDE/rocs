/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fileformatmanager.h"
#include "logging_p.h"

#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <KLocalizedString>

#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

using namespace GraphTheory;

class GraphTheory::FileFormatManagerPrivate
{
public:
    FileFormatManagerPrivate()
        : defaultGraphFilePlugin(nullptr)
    {

    }

    ~FileFormatManagerPrivate()
    { }

    QList<FileFormatInterface*> backends;
    FileFormatInterface *defaultGraphFilePlugin;
};

FileFormatManager::FileFormatManager()
    : d(new FileFormatManagerPrivate)
{
    loadBackends();
}

FileFormatManager::~FileFormatManager()
{

}

QList<FileFormatInterface*> FileFormatManager::backends() const
{
    return d->backends;
}

QList<FileFormatInterface*> FileFormatManager::backends(PluginType type) const
{
    QList<FileFormatInterface*> backends;
    for (FileFormatInterface *backend : std::as_const(d->backends)) {
        switch(type) {
            case Import:
                if (backend->pluginCapability() == FileFormatInterface::ImportOnly
                    || backend->pluginCapability() == FileFormatInterface::ImportAndExport)
                {
                    backends.append(backend);
                }
                break;
            case Export:
                if (backend->pluginCapability() == FileFormatInterface::ExportOnly
                    || backend->pluginCapability() == FileFormatInterface::ImportAndExport)
                {
                    backends.append(backend);
                }
                break;
            default:
                break;
        }
    }
    return backends;
}

void FileFormatManager::loadBackends()
{
    // remove all present backends
    for (FileFormatInterface *f : std::as_const(d->backends)) {
        delete f;
    }
    d->backends.clear();

    const QVector<KPluginMetaData> metadataList = KPluginLoader::findPlugins("rocs/fileformats");

    QPluginLoader loader;

    for (const auto &metadata : metadataList) {
        loader.setFileName(metadata.fileName());

        qCDebug(GRAPHTHEORY_FILEFORMAT) << "Load Plugin: " << metadata.name();
        if (!loader.load()) {
            qCCritical(GRAPHTHEORY_FILEFORMAT) << "Error while loading plugin: " << metadata.name();
        }

        KPluginFactory *factory = KPluginLoader(loader.fileName()).factory();
        FileFormatInterface *plugin = factory->create<FileFormatInterface>(this);
        d->backends.append(plugin);
    }

    // display a QMessageBox if no plugins are found
    if (d->backends.empty()) {
        QMessageBox pluginErrorMessageBox;
        pluginErrorMessageBox.setWindowTitle(i18nc("@title:window", "Plugin Error"));
        pluginErrorMessageBox.setTextFormat(Qt::RichText);
        pluginErrorMessageBox.setText(i18n("Plugins could not be found in specified directories:<br>")+
                                         QCoreApplication::libraryPaths().join("/rocs/fileformats<br>")+
                                         i18n("<br><br> Check <a href='https://doc.qt.io/qt-5/deployment-plugins.html'>"
                                         "this link</a> for further information."));
        pluginErrorMessageBox.setDefaultButton(QMessageBox::Close);
        pluginErrorMessageBox.exec();
        exit(1);
    }

    // load static plugins
    d->defaultGraphFilePlugin = backendByExtension("graph2");
}

FileFormatInterface * FileFormatManager::backendByExtension(const QString &ext)
{
    QString suffix = "*." + ext.section('.', -1); // get suffix
    if (suffix.isEmpty()) {
        qCWarning(GRAPHTHEORY_FILEFORMAT) << "File does not contain extension, falling back to default file format";
        return defaultBackend();
    }
    for (FileFormatInterface * p :  std::as_const(d->backends)) {
        if (p->extensions().join(";").contains(suffix, Qt::CaseInsensitive)) {
            return p;
        }
    }
    return nullptr;
}

FileFormatInterface * FileFormatManager::defaultBackend()
{
    return d->defaultGraphFilePlugin;
}

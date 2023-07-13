/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fileformatmanager.h"
#include "logging_p.h"

#include <KLocalizedString>
#include <KPluginFactory>
#include <KPluginMetaData>

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QString>

using namespace GraphTheory;

class GraphTheory::FileFormatManagerPrivate
{
public:
    QList<FileFormatInterface *> backends;
    FileFormatInterface *defaultGraphFilePlugin = nullptr;
};

FileFormatManager::FileFormatManager()
    : d(new FileFormatManagerPrivate)
{
    const QVector<KPluginMetaData> metadataList = KPluginMetaData::findPlugins("rocs/fileformats");
    for (const auto &metadata : metadataList) {
        const auto result = KPluginFactory::instantiatePlugin<FileFormatInterface>(metadata, this);
        if (result) {
            d->backends.append(result.plugin);
            qCDebug(GRAPHTHEORY_FILEFORMAT) << "Loaded fileformat plugin:" << metadata.pluginId();
        } else {
            qWarning(GRAPHTHEORY_FILEFORMAT) << "Failed to load fileformat plugin" << result.errorText;
        }
    }

    // display a QMessageBox if no plugins are found
    if (d->backends.empty()) {
        QMessageBox pluginErrorMessageBox;
        pluginErrorMessageBox.setWindowTitle(i18nc("@title:window", "Plugin Error"));
        pluginErrorMessageBox.setTextFormat(Qt::RichText);
        pluginErrorMessageBox.setText(i18n("Plugins could not be found in specified directories:<br>")
                                      + QCoreApplication::libraryPaths().join("/rocs/fileformats<br>")
                                      + i18n("<br><br> Check <a href='https://doc.qt.io/qt-5/deployment-plugins.html'>"
                                             "this link</a> for further information."));
        pluginErrorMessageBox.setDefaultButton(QMessageBox::Close);
        pluginErrorMessageBox.exec();
        exit(1);
    }

    // find default file plugin
    d->defaultGraphFilePlugin = backendByExtension("graph2");
}

FileFormatManager::~FileFormatManager() = default;

QList<FileFormatInterface *> FileFormatManager::backends() const
{
    return d->backends;
}

QList<FileFormatInterface *> FileFormatManager::backends(PluginType type) const
{
    QList<FileFormatInterface *> backends;
    for (FileFormatInterface *backend : std::as_const(d->backends)) {
        switch (type) {
        case Import:
            if (backend->pluginCapability() == FileFormatInterface::ImportOnly || backend->pluginCapability() == FileFormatInterface::ImportAndExport) {
                backends.append(backend);
            }
            break;
        case Export:
            if (backend->pluginCapability() == FileFormatInterface::ExportOnly || backend->pluginCapability() == FileFormatInterface::ImportAndExport) {
                backends.append(backend);
            }
            break;
        default:
            break;
        }
    }
    return backends;
}

FileFormatInterface *FileFormatManager::backendByExtension(const QString &ext) const
{
    QString suffix = "*." + ext.section('.', -1); // get suffix
    if (suffix.isEmpty()) {
        qCWarning(GRAPHTHEORY_FILEFORMAT) << "File does not contain extension, falling back to default file format";
        return defaultBackend();
    }
    for (FileFormatInterface *p : std::as_const(d->backends)) {
        if (p->extensions().join(";").contains(suffix, Qt::CaseInsensitive)) {
            return p;
        }
    }
    return nullptr;
}

FileFormatInterface *FileFormatManager::defaultBackend() const
{
    return d->defaultGraphFilePlugin;
}

#include "moc_fileformatmanager.cpp"

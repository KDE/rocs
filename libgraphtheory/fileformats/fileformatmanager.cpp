// SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "fileformatmanager.h"
#include "fileformats/dot/dotfileformat.h"
#include "fileformats/gml/gmlfileformat.h"
#include "fileformats/rocs1/rocs1fileformat.h"
#include "fileformats/rocs2/rocs2fileformat.h"
#include "fileformats/tgf/tgffileformat.h"
#include "fileformats/tikz/tikzfileformat.h"
#include "logging_p.h"
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

using namespace GraphTheory;

FileFormatManager::FileFormatManager()
    : d(new FileFormatManagerPrivate)
{
    d->backends.append(std::make_shared<DotFileFormat>());
    d->backends.append(std::make_shared<Rocs1FileFormat>());
    d->backends.append(std::make_shared<Rocs2FileFormat>());
    d->backends.append(std::make_shared<TgfFileFormat>());
    d->backends.append(std::make_shared<TikzFileFormat>());
    d->backends.append(std::make_shared<GmlFileFormat>());

    // find default file plugin
    d->defaultGraphFilePlugin = backendByExtension("graph2");
}

QList<FileFormatInterface *> FileFormatManager::backends() const
{
    QList<FileFormatInterface *> backends;
    for (const auto &backend : std::as_const(d->backends)) {
        backends << backend.get();
    }
    return backends;
}

QList<FileFormatInterface *> FileFormatManager::backends(PluginType type) const
{
    QList<FileFormatInterface *> backends;
    for (const auto &backend : std::as_const(d->backends)) {
        switch (type) {
        case Import:
            if (backend->pluginCapability() == FileFormatInterface::ImportOnly || backend->pluginCapability() == FileFormatInterface::ImportAndExport) {
                backends.append(backend.get());
            }
            break;
        case Export:
            if (backend->pluginCapability() == FileFormatInterface::ExportOnly || backend->pluginCapability() == FileFormatInterface::ImportAndExport) {
                backends.append(backend.get());
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
    const QString suffix = "*." + ext.section('.', -1); // get suffix
    if (suffix.isEmpty()) {
        qCWarning(GRAPHTHEORY_FILEFORMAT) << "File does not contain extension, falling back to default file format";
        return defaultBackend();
    }
    for (const auto &backend : std::as_const(d->backends)) {
        if (backend->extensions().join(";").contains(suffix, Qt::CaseInsensitive)) {
            return backend.get();
        }
    }
    return nullptr;
}

FileFormatInterface *FileFormatManager::defaultBackend() const
{
    return d->defaultGraphFilePlugin;
}

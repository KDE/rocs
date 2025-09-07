// SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef FILEFORMATMANAGER_P_H
#define FILEFORMATMANAGER_P_H

#include "fileformatinterface.h"
#include <QList>

namespace GraphTheory
{

class FileFormatManagerPrivate
{
public:
    QList<std::shared_ptr<FileFormatInterface>> backends;
    FileFormatInterface *defaultGraphFilePlugin = nullptr;
};

}

#endif

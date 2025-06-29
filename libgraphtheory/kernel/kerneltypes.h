/*
 *  SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KERNELTYPES_H
#define KERNELTYPES_H

#include "graphtheory_export.h"
#include <QObject>

namespace GraphTheory
{
Q_NAMESPACE_EXPORT(GRAPHTHEORY_EXPORT)

enum class MessageType {
    InfoMessage,
    WarningMessage,
    ErrorMessage
};
Q_ENUM_NS(MessageType);
}

#endif // KERNELTYPES_H

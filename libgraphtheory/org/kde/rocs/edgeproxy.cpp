// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "edgeproxy.h"

using namespace GraphTheory;

EdgeProxy::EdgeProxy(EdgePtr edge, QObject *parent)
    : QObject(parent)
    , m_edge(edge)
{
}

EdgePtr EdgeProxy::edge() const
{
    return m_edge;
}

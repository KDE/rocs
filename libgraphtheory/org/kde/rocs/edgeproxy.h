// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef EDGEPROXY_H
#define EDGEPROXY_H

#include "edge.h"
#include <QObject>
#include <QQmlEngine>

namespace GraphTheory
{
class EdgeProxy : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Edge)
    QML_UNCREATABLE("EdgeProxy for Edge object is managed by Graph")

public:
    EdgeProxy(EdgePtr edge, QObject *parent = nullptr);
    ~EdgeProxy() override = default;

    EdgePtr edge() const;

private:
    EdgePtr m_edge;
};
}

#endif

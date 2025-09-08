// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef VIEW_H
#define VIEW_H

#include "graphdocument.h"
#include "graphtheory_export.h"
#include "typenames.h"
#include <QList>
#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>

namespace GraphTheory
{

class ViewPrivate;

/**
 * \class View
 */
class GRAPHTHEORY_EXPORT View : public QQuickWidget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent);
    ~View() override;
    void setGraphDocument(GraphDocumentPtr document);
    GraphDocumentPtr graphDocument() const;

private:
    const QScopedPointer<ViewPrivate> d;
};
}

#endif

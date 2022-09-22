/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include "typenames.h"
#include <QWidget>

class QListView;
class QModelIndex;

namespace GraphTheory
{

class NodeTypePropertyModel;
class EdgeTypePropertyModel;

class PropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertiesWidget(QWidget *parent = nullptr);
    void setType(GraphTheory::NodeTypePtr type);
    void setType(GraphTheory::EdgeTypePtr type);

public Q_SLOTS:
    void addProperty();
    void deleteProperty(const QModelIndex &index);
    void renameProperty(const QModelIndex &index, const QString &name);

private:
    NodeTypePtr m_nodeType;
    NodeTypePropertyModel *m_nodeModel;
    EdgeTypePtr m_edgeType;
    EdgeTypePropertyModel *m_edgeModel;
    QListView *m_view;
};
}

#endif

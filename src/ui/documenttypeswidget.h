/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef DOCUMENTTYPESWIDGET_H
#define DOCUMENTTYPESWIDGET_H

#include "libgraphtheory/edgetype.h"
#include "libgraphtheory/models/edgetypemodel.h"
#include "libgraphtheory/models/nodetypemodel.h"
#include <QWidget>

class QPushButton;

class DocumentTypesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentTypesWidget(QWidget *parent);
    ~DocumentTypesWidget() override = default;

public Q_SLOTS:
    void setDocument(GraphTheory::GraphDocumentPtr document);

private Q_SLOTS:
    void onNodeTypeColorChanged(const QModelIndex &index, const QColor &color);
    void onNodeTypeNameChanged(const QModelIndex &index, const QString &name);
    void onCreateNodeType();
    void onEdgeTypeColorChanged(const QModelIndex &index, const QColor &color);
    void onEdgeTypeNameChanged(const QModelIndex &index, const QString &name);
    void onEdgeTypeDirectionChanged(const QModelIndex &index, const GraphTheory::EdgeType::Direction &direction);
    void onCreateEdgeType();

private:
    GraphTheory::GraphDocumentPtr m_document;
    GraphTheory::NodeTypeModel m_nodeTypeModel;
    GraphTheory::EdgeTypeModel m_edgeTypeModel;
    QPushButton *m_createNodeTypeButton;
    QPushButton *m_createEdgeTypeButton;
};

#endif

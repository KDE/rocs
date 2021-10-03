/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGETYPESDELEGATE_H
#define EDGETYPESDELEGATE_H

#include <KWidgetItemDelegate>
#include "libgraphtheory/typenames.h"
#include "libgraphtheory/edgetype.h"

class QPushButton;

class  EdgeTypesDelegate : public KWidgetItemDelegate
{
    Q_OBJECT
public:
    explicit EdgeTypesDelegate(QAbstractItemView *parent);
    ~EdgeTypesDelegate() override;
    /** reimplemented from QAbstractItemDelegate **/
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /** reimplemented fromKWidgetItemDelegate **/
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /** reimplemented fromKWidgetItemDelegate **/
    QList<QWidget*> createItemWidgets(const QModelIndex &index) const override;
    /** reimplemented fromKWidgetItemDelegate **/
    void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const override;

private Q_SLOTS:
    void onColorDialogOpened();
    void onColorChanged(const QColor &color);
    void onNameChanged(const QString &name);
    void onDirectionSwitched();
    void showPropertiesDialog();

Q_SIGNALS:
    void colorChanged(const QModelIndex &index, const QColor &color);
    void nameChanged(const QModelIndex &index, const QString &name);
    void directionChanged(const QModelIndex &index, GraphTheory::EdgeType::Direction direction);

private:
    QModelIndex m_workaroundColorButtonIndex; // temporary store index for time when dialog is closed
    static const int m_vPadding = 2;
    static const int m_hPadding = 7;
};

#endif

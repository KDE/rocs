/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef PROPERTYDELEGATE_H
#define PROPERTYDELEGATE_H

#include <KWidgetItemDelegate>
#include "libgraphtheory/typenames.h"

class QPushButton;

namespace GraphTheory {

class PropertyDelegate : public KWidgetItemDelegate
{
    Q_OBJECT
public:
    explicit PropertyDelegate(QAbstractItemView *parent);
    ~PropertyDelegate() override;
    /** reimplemented from QAbstractItemDelegate **/
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /** reimplemented fromKWidgetItemDelegate **/
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /** reimplemented fromKWidgetItemDelegate **/
    QList<QWidget*> createItemWidgets(const QModelIndex &index) const override;
    /** reimplemented fromKWidgetItemDelegate **/
    void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const override;

private Q_SLOTS:
    void onNameEditingFinished();
    void onNameInputChanged(const QString &name);
    void onDelete();

Q_SIGNALS:
    void nameChanged(const QModelIndex &index, const QString &name);
    void deleteProperty(const QModelIndex &index);

private:
    static const int m_vPadding = 2;
    static const int m_hPadding = 7;
    QString m_lastNameInput; //<! saves last input text
    QModelIndex m_propertyIndex; //<! index of this property
};
}

#endif

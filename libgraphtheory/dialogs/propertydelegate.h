/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
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
    virtual ~PropertyDelegate();
    /** reimplemented from QAbstractItemDelegate **/
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
    /** reimplemented fromKWidgetItemDelegate **/
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
    /** reimplemented fromKWidgetItemDelegate **/
    virtual QList<QWidget*> createItemWidgets(const QModelIndex &index) const Q_DECL_OVERRIDE;
    /** reimplemented fromKWidgetItemDelegate **/
    virtual void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const Q_DECL_OVERRIDE;

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

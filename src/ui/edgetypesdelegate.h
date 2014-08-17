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
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EDGETYPESDELEGATE_H
#define EDGETYPESDELEGATE_H

#include <KWidgetItemDelegate>
#include "libgraphtheory/typenames.h"

class QPushButton;

class  EdgeTypesDelegate : public KWidgetItemDelegate
{
    Q_OBJECT
public:
    explicit EdgeTypesDelegate(QAbstractItemView *parent);
    virtual ~EdgeTypesDelegate();
    /** reimplemented from QAbstractItemDelegate **/
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    /** reimplemented fromKWidgetItemDelegate **/
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    /** reimplemented fromKWidgetItemDelegate **/
    virtual QList<QWidget*> createItemWidgets(const QModelIndex &index) const;
    /** reimplemented fromKWidgetItemDelegate **/
    virtual void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const;

private Q_SLOTS:
    void onColorDialogOpened();
    void onColorChanged(const QColor &color);
    void onNameChanged(const QString &name);

Q_SIGNALS:
    void colorChanged(const QModelIndex &index, const QColor &color);
    void nameChanged(const QModelIndex &index, const QString &name);

private:
    QPushButton *m_removeButton;
    QModelIndex m_workaroundColorButtonIndex; // temporary store index for time when dialog is closed
    static const int m_vPadding = 2;
    static const int m_hPadding = 7;
};

#endif

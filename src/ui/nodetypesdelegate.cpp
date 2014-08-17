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

#include "nodetypesdelegate.h"
#include "libgraphtheory/models/nodetypemodel.h"
#include "libgraphtheory/nodetype.h"
#include <QPushButton>
#include <KColorButton>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QApplication>
#include <QDebug>

NodeTypesDelegate::NodeTypesDelegate(QAbstractItemView* parent)
    : KWidgetItemDelegate(parent)
    , m_removeButton(new QPushButton(0))
{

}

NodeTypesDelegate::~NodeTypesDelegate()
{
    m_removeButton->deleteLater();
}

void NodeTypesDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
}

QSize NodeTypesDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    int iconHeight = option.decorationSize.height() + (m_vPadding * 2); //icon height + padding either side
    int textHeight = option.fontMetrics.height()*2 + (m_vPadding * 2) + 10; // text height * 2 + padding + some space between the lines
    return QSize(-1,qMax(iconHeight, textHeight)); // any width, he view should give us the whole thing
}

QList< QWidget* > NodeTypesDelegate::createItemWidgets(const QModelIndex &index) const
{
    // items created by this method and added to the return-list will be
    // deleted by KWidgetItemDelegate

    KColorButton *colorButton = new KColorButton(index.data(GraphTheory::NodeTypeModel::ColorRole).value<QColor>());
    colorButton->setFlat(true);
    QLineEdit *title = new QLineEdit(index.data(GraphTheory::NodeTypeModel::TitleRole).toString());
    title->setMinimumWidth(100);

    connect(colorButton, SIGNAL(changed(QColor)), SLOT(onColorChanged(QColor)));
    connect(colorButton, SIGNAL(pressed()), SLOT(onColorDialogOpened()));
    connect(title, SIGNAL(textEdited(const QString&)), SLOT(onNameChanged(const QString&)));

    return QList<QWidget*>() << colorButton
                             << title;
}

void NodeTypesDelegate::updateItemWidgets(const QList< QWidget* > widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const
{
    // widgets:
    // ColorButton | Title

    if (!index.isValid()) {
        return;
    }

    Q_ASSERT(widgets.size() == 2);

    KColorButton *colorButton = qobject_cast<KColorButton*>(widgets.at(0));
    QLineEdit *title = qobject_cast<QLineEdit*>(widgets.at(1));

    Q_ASSERT(title);
    Q_ASSERT(colorButton);

    colorButton->setColor(index.data(GraphTheory::NodeTypeModel::ColorRole).value<QColor>());
    title->setText(index.data(GraphTheory::NodeTypeModel::TitleRole).toString());

    QRect outerRect(0, 0, option.rect.width(), option.rect.height());
    QRect contentRect = outerRect.adjusted(m_hPadding, m_vPadding, -m_hPadding, -m_vPadding);

    int colorButtonLeftMargin = contentRect.left();
    int colorButtonTopMargin = (outerRect.height() - colorButton->height()) / 2;
    colorButton->move(colorButtonLeftMargin, colorButtonTopMargin);

    int titleLeftMargin = colorButtonLeftMargin + colorButton->width() + 10;
    int titleTopMargin = (outerRect.height() - title->height()) / 2;
    title->move(titleLeftMargin, titleTopMargin);
}

void NodeTypesDelegate::onColorChanged(const QColor &color)
{
    // use temporary stored index, since focusedIndex() does not return current index
    // reason: the color dialog signal does not allow for correct index estimation
    emit colorChanged(m_workaroundColorButtonIndex, color);
}

void NodeTypesDelegate::onColorDialogOpened()
{
    m_workaroundColorButtonIndex = focusedIndex();
}

void NodeTypesDelegate::onNameChanged(const QString &name)
{
    QModelIndex index = focusedIndex();
    emit nameChanged(index, name);
}

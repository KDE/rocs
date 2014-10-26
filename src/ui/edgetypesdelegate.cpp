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

#include "edgetypesdelegate.h"
#include "libgraphtheory/models/edgetypemodel.h"
#include "libgraphtheory/edgetype.h"
#include <KColorButton>
#include <KLocalizedString>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QApplication>
#include <QDebug>

EdgeTypesDelegate::EdgeTypesDelegate(QAbstractItemView* parent)
    : KWidgetItemDelegate(parent)
    , m_removeButton(new QPushButton(0))
{

}

EdgeTypesDelegate::~EdgeTypesDelegate()
{
    m_removeButton->deleteLater();
}

void EdgeTypesDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
}

QSize EdgeTypesDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    int iconHeight = option.decorationSize.height() + (m_vPadding * 2); //icon height + padding either side
    int textHeight = option.fontMetrics.height()*2 + (m_vPadding * 2) + 10; // text height * 2 + padding + some space between the lines
    return QSize(-1,qMax(iconHeight, textHeight)); // any width, he view should give us the whole thing
}

QList< QWidget* > EdgeTypesDelegate::createItemWidgets(const QModelIndex &index) const
{
    // items created by this method and added to the return-list will be
    // deleted by KWidgetItemDelegate

    KColorButton *colorButton = new KColorButton(index.data(GraphTheory::EdgeTypeModel::ColorRole).value<QColor>());
    colorButton->setFlat(true);
    QToolButton *direction = new QToolButton();
    direction->setCheckable(true);
    direction->setToolTip(i18n("Direction of edges of edge type."));
    QLineEdit *title = new QLineEdit(index.data(GraphTheory::EdgeTypeModel::TitleRole).toString());
    title->setMinimumWidth(100);
    QLabel *idLabel = new QLabel(index.data(GraphTheory::EdgeTypeModel::IdRole).toString());
    idLabel->setToolTip(i18n("Unique ID of edge type."));

    connect(colorButton, &KColorButton::changed, this, &EdgeTypesDelegate::onColorChanged);
    connect(colorButton, &KColorButton::pressed, this, &EdgeTypesDelegate::onColorDialogOpened);
    connect(direction, &QToolButton::toggled, this, &EdgeTypesDelegate::onDirectionSwitched);
    connect(title, SIGNAL(textEdited(const QString&)), SLOT(onNameChanged(const QString&)));

    return QList<QWidget*>() << colorButton
                             << direction
                             << title
                             << idLabel;
}

void EdgeTypesDelegate::updateItemWidgets(const QList< QWidget* > widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const
{
    // widgets:
    // Color | Direction | Title | ID

    if (!index.isValid()) {
        return;
    }

    Q_ASSERT(widgets.size() == 4);

    KColorButton *colorButton = qobject_cast<KColorButton*>(widgets.at(0));
    QToolButton *directionSwitch = qobject_cast<QToolButton*>(widgets.at(1));
    QLineEdit *title = qobject_cast<QLineEdit*>(widgets.at(2));
    QLabel *id = qobject_cast<QLabel*>(widgets.at(3));

    Q_ASSERT(colorButton);
    Q_ASSERT(directionSwitch);
    Q_ASSERT(title);
    Q_ASSERT(id);

    colorButton->setColor(index.data(GraphTheory::EdgeTypeModel::ColorRole).value<QColor>());
    if (index.data(GraphTheory::EdgeTypeModel::DirectionRole).toInt() == GraphTheory::EdgeType::Unidirectional) {
        directionSwitch->setIcon(QIcon::fromTheme("rocsunidirectional"));
    } else {
        directionSwitch->setIcon(QIcon::fromTheme("rocsbidirectional"));
    }
    title->setText(index.data(GraphTheory::EdgeTypeModel::TitleRole).toString());
    id->setText(index.data(GraphTheory::EdgeTypeModel::IdRole).toString());

    QRect outerRect(0, 0, option.rect.width(), option.rect.height());
    QRect contentRect = outerRect.adjusted(m_hPadding, m_vPadding, -m_hPadding, -m_vPadding);

    int colorButtonLeftMargin = contentRect.left();
    int colorButtonTopMargin = (outerRect.height() - colorButton->height()) / 2;
    colorButton->move(colorButtonLeftMargin, colorButtonTopMargin);

    int directionSwitchLeftMargin = colorButtonLeftMargin + + colorButton->width() + 10;
    int directionSwitchTopMargin = (outerRect.height() - directionSwitch->height()) / 2;
    directionSwitch->move(directionSwitchLeftMargin, directionSwitchTopMargin);

    int titleLeftMargin = directionSwitchLeftMargin + directionSwitch->width() + 10;
    int titleTopMargin = (outerRect.height() - title->height()) / 2;
    title->move(titleLeftMargin, titleTopMargin);

    int idLeftMargin = titleLeftMargin + title->width() + 10;
    int idTopMargin = (outerRect.height() - id->height()) / 2;
    id->move(idLeftMargin, idTopMargin);
}

void EdgeTypesDelegate::onColorChanged(const QColor &color)
{
    // use temporary stored index, since focusedIndex() does not return current index
    // reason: the color dialog signal does not allow for correct index estimation
    emit colorChanged(m_workaroundColorButtonIndex, color);
}

void EdgeTypesDelegate::onColorDialogOpened()
{
    m_workaroundColorButtonIndex = focusedIndex();
}

void EdgeTypesDelegate::onNameChanged(const QString &name)
{
    QModelIndex index = focusedIndex();
    emit nameChanged(index, name);
}

void EdgeTypesDelegate::onDirectionSwitched()
{
    QModelIndex index = focusedIndex();
    GraphTheory::EdgeType::Direction direction = static_cast<GraphTheory::EdgeType::Direction>(
        index.data(GraphTheory::EdgeTypeModel::DirectionRole).toInt());
    if (direction == GraphTheory::EdgeType::Bidirectional) {
        emit directionChanged(index, GraphTheory::EdgeType::Unidirectional);
    } else {
        emit directionChanged(index, GraphTheory::EdgeType::Bidirectional);
    }
}

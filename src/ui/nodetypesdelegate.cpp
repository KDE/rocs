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
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/dialogs/nodetypeproperties.h"
#include "libgraphtheory/models/nodetypemodel.h"
#include <KColorButton>
#include <KLocalizedString>
#include <QApplication>
#include <QAbstractItemView>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QToolButton>

using namespace GraphTheory;

NodeTypesDelegate::NodeTypesDelegate(QAbstractItemView* parent)
    : KWidgetItemDelegate(parent, parent->viewport())
{

}

NodeTypesDelegate::~NodeTypesDelegate()
{

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

    KColorButton *colorButton = new KColorButton(index.data(NodeTypeModel::ColorRole).value<QColor>());
    colorButton->setFlat(true);
    QLineEdit *title = new QLineEdit(index.data(NodeTypeModel::TitleRole).toString());
    title->setMinimumWidth(100);
    QLabel *idLabel = new QLabel(index.data(NodeTypeModel::IdRole).toString());
    idLabel->setToolTip(i18n("Unique ID of the node type."));
    QToolButton *propertiesButton = new QToolButton();
    propertiesButton->setIcon(QIcon::fromTheme("document-properties"));

    connect(colorButton, &KColorButton::changed,
        this, &NodeTypesDelegate::onColorChanged);
    connect(colorButton, &KColorButton::pressed,
        this, &NodeTypesDelegate::onColorDialogOpened);
    connect(title, &QLineEdit::textEdited,
        this, &NodeTypesDelegate::onNameChanged);
    connect(propertiesButton, &QToolButton::clicked,
        this, &NodeTypesDelegate::showPropertiesDialog);

    return QList<QWidget*>() << colorButton
                             << title
                             << idLabel
                             << propertiesButton;
}

void NodeTypesDelegate::updateItemWidgets(const QList< QWidget* > widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const
{
    // widgets:
    // ColorButton | Title | ID

    if (!index.isValid()) {
        return;
    }

    Q_ASSERT(widgets.size() == 4);

    KColorButton *colorButton = qobject_cast<KColorButton*>(widgets.at(0));
    QLineEdit *title = qobject_cast<QLineEdit*>(widgets.at(1));
    QLabel *id = qobject_cast<QLabel*>(widgets.at(2));
    QToolButton *propertiesButton = qobject_cast<QToolButton*>(widgets.at(3));

    Q_ASSERT(title);
    Q_ASSERT(colorButton);
    Q_ASSERT(id);
    Q_ASSERT(propertiesButton);

    colorButton->setColor(index.data(NodeTypeModel::ColorRole).value<QColor>());
    title->setText(index.data(NodeTypeModel::TitleRole).toString());
    id->setText(index.data(NodeTypeModel::IdRole).toString());

    QRect outerRect(0, 0, option.rect.width(), option.rect.height());
    QRect contentRect = outerRect.adjusted(m_hPadding, m_vPadding, -m_hPadding, -m_vPadding);

    int colorButtonLeftMargin = contentRect.left();
    int colorButtonTopMargin = (outerRect.height() - colorButton->height()) / 2;
    colorButton->move(colorButtonLeftMargin, colorButtonTopMargin);

    int titleLeftMargin = colorButtonLeftMargin + colorButton->width() + 10;
    int titleTopMargin = (outerRect.height() - title->height()) / 2;
    title->move(titleLeftMargin, titleTopMargin);

    // construct remaining from right to left
    int propertiesLeftMargin = contentRect.right() - propertiesButton->width() - m_hPadding;
    int propertiesTopMargin = (outerRect.height() - propertiesButton->height()) / 2;
    propertiesButton->move(propertiesLeftMargin, propertiesTopMargin);

    int idLeftMargin = propertiesLeftMargin - id->width() - 10;
    int idTopMargin = (outerRect.height() - id->height()) / 2;
    id->move(idLeftMargin, idTopMargin);

    // title gets remaining space
    title->setFixedWidth(qMax(0, idLeftMargin - titleLeftMargin - 10));
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

void NodeTypesDelegate::showPropertiesDialog()
{
    QModelIndex index = focusedIndex();
    NodeType *type = qobject_cast<NodeType*>(index.data(NodeTypeModel::DataRole).value<QObject*>());
    QPointer<NodeTypeProperties> dialog = new NodeTypeProperties(0);
    dialog->setType(type->self());
    dialog->exec();
}

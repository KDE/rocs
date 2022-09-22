/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "edgetypesdelegate.h"
#include "libgraphtheory/dialogs/edgetypeproperties.h"
#include "libgraphtheory/models/edgetypemodel.h"
#include <KColorButton>
#include <KLocalizedString>
#include <QAbstractItemView>
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPointer>
#include <QToolButton>

using namespace GraphTheory;

EdgeTypesDelegate::EdgeTypesDelegate(QAbstractItemView *parent)
    : KWidgetItemDelegate(parent, parent->viewport())
{
}

EdgeTypesDelegate::~EdgeTypesDelegate()
{
}

void EdgeTypesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, nullptr);
}

QSize EdgeTypesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    int iconHeight = option.decorationSize.height() + (m_vPadding * 2); // icon height + padding either side
    int textHeight = option.fontMetrics.height() * 2 + (m_vPadding * 2) + 10; // text height * 2 + padding + some space between the lines
    return QSize(-1, qMax(iconHeight, textHeight)); // any width, he view should give us the whole thing
}

QList<QWidget *> EdgeTypesDelegate::createItemWidgets(const QModelIndex &index) const
{
    // items created by this method and added to the return-list will be
    // deleted by KWidgetItemDelegate

    KColorButton *colorButton = new KColorButton(index.data(EdgeTypeModel::ColorRole).value<QColor>());
    colorButton->setFlat(true);
    QToolButton *direction = new QToolButton();
    direction->setCheckable(true);
    direction->setToolTip(i18n("Direction of edges of edge type."));
    QLineEdit *title = new QLineEdit(index.data(EdgeTypeModel::TitleRole).toString());
    title->setMinimumWidth(100);
    QLabel *idLabel = new QLabel(index.data(EdgeTypeModel::IdRole).toString());
    idLabel->setToolTip(i18n("Unique ID of edge type."));
    QToolButton *propertiesButton = new QToolButton();
    propertiesButton->setIcon(QIcon::fromTheme("document-properties"));

    connect(colorButton, &KColorButton::changed, this, &EdgeTypesDelegate::onColorChanged);
    connect(colorButton, &KColorButton::pressed, this, &EdgeTypesDelegate::onColorDialogOpened);
    connect(direction, &QToolButton::toggled, this, &EdgeTypesDelegate::onDirectionSwitched);
    connect(title, &QLineEdit::textEdited, this, &EdgeTypesDelegate::onNameChanged);
    connect(propertiesButton, &QToolButton::clicked, this, &EdgeTypesDelegate::showPropertiesDialog);

    return {colorButton, direction, title, idLabel, propertiesButton};
}

void EdgeTypesDelegate::updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    // widgets:
    // Color | Direction | Title | ID

    if (!index.isValid()) {
        return;
    }

    Q_ASSERT(widgets.size() == 5);

    KColorButton *colorButton = qobject_cast<KColorButton *>(widgets.at(0));
    QToolButton *directionSwitch = qobject_cast<QToolButton *>(widgets.at(1));
    QLineEdit *title = qobject_cast<QLineEdit *>(widgets.at(2));
    QLabel *id = qobject_cast<QLabel *>(widgets.at(3));
    QToolButton *propertiesButton = qobject_cast<QToolButton *>(widgets.at(4));

    Q_ASSERT(colorButton);
    Q_ASSERT(directionSwitch);
    Q_ASSERT(title);
    Q_ASSERT(id);
    Q_ASSERT(propertiesButton);

    colorButton->setColor(index.data(EdgeTypeModel::ColorRole).value<QColor>());
    if (index.data(EdgeTypeModel::DirectionRole).toInt() == EdgeType::Unidirectional) {
        directionSwitch->setIcon(QIcon::fromTheme("rocsunidirectional"));
    } else {
        directionSwitch->setIcon(QIcon::fromTheme("rocsbidirectional"));
    }
    title->setText(index.data(EdgeTypeModel::TitleRole).toString());
    id->setText(index.data(EdgeTypeModel::IdRole).toString());

    QRect outerRect(0, 0, option.rect.width(), option.rect.height());
    QRect contentRect = outerRect.adjusted(m_hPadding, m_vPadding, -m_hPadding, -m_vPadding);

    int colorButtonLeftMargin = contentRect.left();
    int colorButtonTopMargin = (outerRect.height() - colorButton->height()) / 2;
    colorButton->move(colorButtonLeftMargin, colorButtonTopMargin);

    int directionSwitchLeftMargin = colorButtonLeftMargin + +colorButton->width() + 10;
    int directionSwitchTopMargin = (outerRect.height() - directionSwitch->height()) / 2;
    directionSwitch->move(directionSwitchLeftMargin, directionSwitchTopMargin);

    int titleLeftMargin = directionSwitchLeftMargin + directionSwitch->width() + 10;
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

void EdgeTypesDelegate::onColorChanged(const QColor &color)
{
    // use temporary stored index, since focusedIndex() does not return current index
    // reason: the color dialog signal does not allow for correct index estimation
    Q_EMIT colorChanged(m_workaroundColorButtonIndex, color);
}

void EdgeTypesDelegate::onColorDialogOpened()
{
    m_workaroundColorButtonIndex = focusedIndex();
}

void EdgeTypesDelegate::onNameChanged(const QString &name)
{
    QModelIndex index = focusedIndex();
    Q_EMIT nameChanged(index, name);
}

void EdgeTypesDelegate::onDirectionSwitched()
{
    QModelIndex index = focusedIndex();
    EdgeType::Direction direction = static_cast<EdgeType::Direction>(index.data(EdgeTypeModel::DirectionRole).toInt());

    const EdgeType::Direction newDirection = direction == EdgeType::Bidirectional ? EdgeType::Unidirectional : EdgeType::Bidirectional;

    Q_EMIT directionChanged(index, newDirection);
}

void EdgeTypesDelegate::showPropertiesDialog()
{
    QModelIndex index = focusedIndex();
    EdgeType *type = qobject_cast<EdgeType *>(index.data(EdgeTypeModel::DataRole).value<QObject *>());
    QPointer<EdgeTypeProperties> dialog = new EdgeTypeProperties(nullptr);
    dialog->setType(type->self());
    dialog->exec();
    delete dialog;
}

/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "propertydelegate.h"
#include "models/nodetypepropertymodel.h"
#include <KLocalizedString>
#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QPainter>
#include <QToolButton>

using namespace GraphTheory;

PropertyDelegate::PropertyDelegate(QAbstractItemView *parent)
    : KWidgetItemDelegate(parent)
{
}

PropertyDelegate::~PropertyDelegate()
{
}

void PropertyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, nullptr);
}

QSize PropertyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    int iconHeight = option.decorationSize.height() + (m_vPadding * 2); // icon height + padding either side
    int textHeight = option.fontMetrics.height() * 2 + (m_vPadding * 2) + 10; // text height * 2 + padding + some space between the lines
    return QSize(-1, qMax(iconHeight, textHeight)); // any width, he view should give us the whole thing
}

QList<QWidget *> PropertyDelegate::createItemWidgets(const QModelIndex &index) const
{
    // items created by this method and added to the return-list will be
    // deleted by KWidgetItemDelegate
    QToolButton *deleteButton = new QToolButton();
    deleteButton->setIcon(QIcon::fromTheme("edit-delete"));
    deleteButton->setToolTip(i18n("Delete this property."));
    QLineEdit *nameEdit = new QLineEdit(index.data(NodeTypePropertyModel::NameRole).toString());

    // change temporary name-input value whenever text is changed (not necessarily by user), and apply when finished is emitted
    connect(nameEdit, &QLineEdit::textChanged, this, &PropertyDelegate::onNameInputChanged);
    connect(nameEdit, &QLineEdit::editingFinished, this, &PropertyDelegate::onNameEditingFinished);
    connect(deleteButton, &QToolButton::clicked, this, &PropertyDelegate::onDelete);

    return QList<QWidget *>() << nameEdit << deleteButton;
}

void PropertyDelegate::updateItemWidgets(const QList<QWidget *> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    // widgets:
    // Name || DeleteButton

    if (!index.isValid()) {
        return;
    }

    Q_ASSERT(widgets.size() == 2);

    QLineEdit *name = qobject_cast<QLineEdit *>(widgets.at(0));
    QToolButton *deleteButton = qobject_cast<QToolButton *>(widgets.at(1));

    Q_ASSERT(name);
    Q_ASSERT(deleteButton);

    name->setText(index.data(NodeTypePropertyModel::NameRole).toString());

    QRect outerRect(0, 0, option.rect.width(), option.rect.height());
    QRect contentRect = outerRect.adjusted(m_hPadding, m_vPadding, -m_hPadding, -m_vPadding);

    int nameLeftMargin = contentRect.left();
    int nameTopMargin = (outerRect.height() - name->height()) / 2;
    name->move(nameLeftMargin, nameTopMargin);
    name->setMinimumWidth(contentRect.width() - deleteButton->width() - 10);
    name->setMaximumWidth(contentRect.width() - deleteButton->width() - 10);

    int deleteButtonLeftMargin = contentRect.right() - deleteButton->width();
    int deleteButtonTopMargin = (outerRect.height() - deleteButton->height()) / 2;
    deleteButton->move(deleteButtonLeftMargin, deleteButtonTopMargin);
}

void PropertyDelegate::onNameEditingFinished()
{
    if (!m_propertyIndex.isValid()) {
        return;
    }
    Q_EMIT nameChanged(m_propertyIndex, m_lastNameInput);
}

void PropertyDelegate::onNameInputChanged(const QString &name)
{
    m_lastNameInput = name;
    m_propertyIndex = focusedIndex();
}

void PropertyDelegate::onDelete()
{
    const QModelIndex index = focusedIndex();
    Q_EMIT deleteProperty(index);
}

#include "moc_propertydelegate.cpp"

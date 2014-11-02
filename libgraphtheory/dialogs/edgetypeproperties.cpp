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

#include "edgetypeproperties.h"
#include "edge.h"
#include "propertieswidget.h"
#include "graphdocument.h"

#include <KColorButton>
#include <KGuiItem>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace GraphTheory;

EdgeTypeProperties::EdgeTypeProperties(QWidget *parent)
    : QDialog(parent)
    , m_name(new QLineEdit(this))
    , m_id(new QSpinBox(this))
    , m_color(new KColorButton(this))
    , m_direction(new QComboBox(this))
    , m_properties(new PropertiesWidget(this))
    , m_type(EdgeTypePtr())
{
    setWindowTitle(i18nc("@title:window", "Edge Type Properties"));

    // form layout with static edge type properties
    QWidget *widget = new QWidget(this);
    QFormLayout *head = new QFormLayout(widget);
    head->addRow(i18n("Name"), m_name);
    head->addRow(i18n("Identifier"), m_id);
    head->addRow(i18n("Color"), m_color);
    m_direction->addItem(QIcon::fromTheme("rocsunidirectional"), i18n("Unidirectional"), EdgeType::Unidirectional);
    m_direction->addItem(QIcon::fromTheme("rocsbidirectional"), i18n("Bidirectional"), EdgeType::Bidirectional);
    head->addRow(i18n("Direction"), m_direction);
    widget->setLayout(head);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(widget);
    mainLayout->addWidget(m_properties);

    // dialog buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(this);

    QPushButton *okButton = new QPushButton;
    KGuiItem::assign(okButton, KStandardGuiItem::ok());
    okButton->setShortcut(Qt::Key_Return);

    QPushButton *cancelButton = new QPushButton;
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    cancelButton->setShortcut(Qt::Key_Escape);

    buttons->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(okButton, &QPushButton::clicked, this, &EdgeTypeProperties::accept);
    connect(cancelButton, &QPushButton::clicked, this, &EdgeTypeProperties::reject);

    connect(this, &QDialog::accepted, this, &EdgeTypeProperties::apply);
    setAttribute(Qt::WA_DeleteOnClose);
}

void EdgeTypeProperties::setType(EdgeTypePtr type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;

    m_name->setText(type->name());
    m_id->setValue(type->id());
    m_color->setColor(type->color());
    m_direction->setCurrentIndex(m_direction->findData(type->direction()));
    m_properties->setType(type);
}

void EdgeTypeProperties::apply()
{
    m_type->setName(m_name->text());
    m_type->setId(m_id->value()); // FIXME no check performed if ID is already in use!
    m_type->setColor(m_color->color());
    m_type->setDirection(static_cast<EdgeType::Direction>(m_direction->currentData().toInt()));
}

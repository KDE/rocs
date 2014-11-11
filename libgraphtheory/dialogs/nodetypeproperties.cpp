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

#include "nodetypeproperties.h"
#include "propertieswidget.h"
#include "node.h"
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

NodeTypeProperties::NodeTypeProperties(QWidget *parent)
    : QDialog(parent)
    , m_name(new QLineEdit(this))
    , m_id(new QSpinBox(this))
    , m_color(new KColorButton(this))
    , m_properties(new PropertiesWidget(this))
    , m_okButton(new QPushButton(this))
    , m_type(NodeTypePtr())
{
    setWindowTitle(i18nc("@title:window", "Node Type Properties"));

    // form layout with static edge type properties
    QWidget *widget = new QWidget(this);
    QFormLayout *head = new QFormLayout(widget);
    head->addRow(i18n("Name"), m_name);
    head->addRow(i18n("Identifier"), m_id);
    m_id->setMinimum(1);
    head->addRow(i18n("Color"), m_color);
    widget->setLayout(head);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(widget);
    mainLayout->addWidget(m_properties);

    // dialog buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(this);

    KGuiItem::assign(m_okButton, KStandardGuiItem::ok());
    m_okButton->setShortcut(Qt::Key_Return);

    QPushButton *cancelButton = new QPushButton;
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    cancelButton->setShortcut(Qt::Key_Escape);

    buttons->addButton(m_okButton, QDialogButtonBox::AcceptRole);
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(m_okButton, &QPushButton::clicked,
        this, &NodeTypeProperties::accept);
    connect(cancelButton, &QPushButton::clicked,
        this, &NodeTypeProperties::reject);

    connect(m_id, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &NodeTypeProperties::validateIdInput);
    connect(this, &QDialog::accepted,
        this, &NodeTypeProperties::apply);
    setAttribute(Qt::WA_DeleteOnClose);
}

void NodeTypeProperties::setType(NodeTypePtr type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;

    m_name->setText(type->name());
    m_id->setValue(type->id());
    m_color->setColor(type->color());
    m_properties->setType(type);

    // set initial color and tooltip
    validateIdInput();
}

void NodeTypeProperties::apply()
{
    m_type->setName(m_name->text());
    // note: ID is valid since otherwise OK-button would be disabled
    // no need to check here
    m_type->setId(m_id->value());
    m_type->setColor(m_color->color());
}

void NodeTypeProperties::validateIdInput()
{
    int valid = true;
    foreach (const auto &type, m_type->document()->nodeTypes()) {
        if (type != m_type && type->id() == m_id->value()) {
            valid = false;
            break;
        }
    }
    // set color
    QPalette palette = m_id->palette();
    if (valid) {
        palette.setColor(QPalette::Text, Qt::black);
        m_okButton->setEnabled(true);
        m_okButton->setToolTip(i18nc("@info:tooltip", "The selected ID for this node type."));
    } else {
        palette.setColor(QPalette::Text, Qt::red);
        m_okButton->setEnabled(false);
        m_okButton->setToolTip(i18nc("@info:tooltip", "The selected ID is already used for another node type, please select a different one."));
    }
    m_id->setPalette(palette);
}

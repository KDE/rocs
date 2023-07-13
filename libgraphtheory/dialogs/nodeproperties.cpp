/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "nodeproperties.h"
#include "node.h"
#include <graphdocument.h>

#include <KGuiItem>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QDebug>
#include <QDialogButtonBox>
#include <QPointer>
#include <QPushButton>

using namespace GraphTheory;

NodeProperties::NodeProperties(QWidget *parent)
    : QDialog(parent)
    , m_node(NodePtr())
    , m_okButton(new QPushButton)
{
    setWindowTitle(i18nc("@title:window", "Node Properties"));

    QWidget *widget = new QWidget(this);
    ui = new Ui::NodeProperties;
    ui->setupUi(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(widget);

    // buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(this);

    KGuiItem::assign(m_okButton, KStandardGuiItem::ok());
    m_okButton->setShortcut(Qt::Key_Return);

    QPushButton *cancelButton = new QPushButton;
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    cancelButton->setShortcut(Qt::Key_Escape);

    buttons->addButton(m_okButton, QDialogButtonBox::AcceptRole);
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(m_okButton, &QPushButton::clicked, this, &NodeProperties::accept);
    connect(cancelButton, &QPushButton::clicked, this, &NodeProperties::reject);

    connect(ui->id, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &NodeProperties::validateIdInput);
    connect(this, &NodeProperties::accepted, this, &NodeProperties::apply);

    setAttribute(Qt::WA_DeleteOnClose);
}

void NodeProperties::setData(NodePtr node)
{
    if (m_node == node) {
        return;
    }
    m_node = node;
    ui->color->setColor(node->color());
    ui->id->setValue(node->id());

    // set initial color and tooltip
    validateIdInput();

    // update types
    ui->type->clear();
    int selectedType = -1;
    for (int i = 0; i < node->document()->nodeTypes().count(); ++i) {
        NodeTypePtr type = node->document()->nodeTypes().at(i);
        if (type == node->type()) {
            selectedType = i;
        }
        QString typeString = QString("%1").arg(type->name());
        ui->type->addItem(typeString, QVariant(i));
    }
    ui->type->setCurrentIndex(ui->type->findData(QVariant(selectedType)));

    // dynamic properties
    ui->dynamicProperties->setColumnCount(2);
    ui->dynamicProperties->setRowCount(node->dynamicProperties().count());
    QTableWidgetItem *nameHeaderItem = new QTableWidgetItem(i18n("Name"));
    QTableWidgetItem *valueHeaderItem = new QTableWidgetItem(i18n("Value"));
    ui->dynamicProperties->setHorizontalHeaderItem(0, nameHeaderItem);
    ui->dynamicProperties->setHorizontalHeaderItem(1, valueHeaderItem);
    for (int i = 0; i < node->dynamicProperties().count(); ++i) {
        QString propertyName = node->dynamicProperties().at(i);
        QTableWidgetItem *nameItem = new QTableWidgetItem(propertyName);
        nameItem->setFlags(Qt::NoItemFlags);
        QTableWidgetItem *valueItem = new QTableWidgetItem(node->dynamicProperty(propertyName).toString());
        ui->dynamicProperties->setItem(i, 0, nameItem);
        ui->dynamicProperties->setItem(i, 1, valueItem);
    }

    ui->dynamicProperties->horizontalHeader()->setProperty("stretchLastSection", true);
}

void NodeProperties::apply()
{
    m_node->setColor(ui->color->color());
    m_node->setType(m_node->document()->nodeTypes().at(ui->type->currentIndex()));
    m_node->setId(ui->id->value());

    for (int i = 0; i < ui->dynamicProperties->rowCount(); ++i) {
        QString name = ui->dynamicProperties->item(i, 0)->data(Qt::DisplayRole).toString();
        QVariant value = ui->dynamicProperties->item(i, 1)->data(Qt::DisplayRole);
        m_node->setDynamicProperty(name, value);
    }
}

void NodeProperties::validateIdInput()
{
    if (!m_node) {
        return;
    }
    int valid = true;
    const auto nodes = m_node->document()->nodes();
    for (const auto &node : nodes) {
        if (node != m_node && node->id() == ui->id->value()) {
            valid = false;
            break;
        }
    }
    // set color
    QPalette palette = ui->id->palette();
    if (valid) {
        palette = style()->standardPalette();
        m_okButton->setEnabled(true);
        m_okButton->setToolTip(i18nc("@info:tooltip", "The selected ID for this node."));
    } else {
        palette.setColor(QPalette::Text, Qt::red);
        m_okButton->setEnabled(false);
        m_okButton->setToolTip(i18nc("@info:tooltip", "The selected ID is already used for another node, please select a different one."));
    }
    ui->id->setPalette(palette);
}

#include "moc_nodeproperties.cpp"

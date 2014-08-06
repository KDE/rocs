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

#include "nodeproperties.h"
#include "node.h"
#include <graphdocument.h>

#include <KLocalizedString>
#include <KGuiItem>
#include <KStandardGuiItem>
#include <QDialogButtonBox>
#include <QPointer>
#include <QPushButton>
#include <QDebug>

using namespace GraphTheory;

NodeProperties::NodeProperties(QWidget* parent)
    : QDialog(parent)
    , m_node(NodePtr())
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

    QPushButton *okButton = new QPushButton;
    KGuiItem::assign(okButton, KStandardGuiItem::ok());
    okButton->setShortcut(Qt::Key_Return);

    QPushButton *cancelButton = new QPushButton;
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    cancelButton->setShortcut(Qt::Key_Escape);

    buttons->addButton(okButton, QDialogButtonBox::AcceptRole);
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect(this, SIGNAL(accepted()), SLOT(apply()));

    setAttribute(Qt::WA_DeleteOnClose);
}

void NodeProperties::setData(NodePtr node)
{
    if (m_node == node) {
        return;
    }
    m_node = node;
    ui->color->setColor(node->color());

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

//     ui->_propertiesTable->setModel(model); //FIXME rewrite model
//     ui->_propertiesTable->horizontalHeader()->setProperty("stretchLastSection", true);
}

void NodeProperties::apply()
{
    m_node->setColor(ui->color->color());
    m_node->setType(m_node->document()->nodeTypes().at(ui->type->currentIndex()));
    //TODO dynamic properties
}

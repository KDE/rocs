/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DataStructurePropertiesDialog.h"

#include "DataStructurePage.h"
#include "DataTypePage.h"
#include "PointerTypePage.h"
#include <Data.h>
#include <Scene/DataItem.h>

#include <QDebug>
#include <KLocalizedString>
#include <KComboBox>
#include <KGuiItem>
#include <KStandardGuiItem>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDialog>
#include <QTabWidget>
#include <QLayout>
#include <QLabel>

DataStructurePropertiesDialog::DataStructurePropertiesDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Data Structure Properties"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QTabWidget *tabWidget = new QTabWidget(this);
    _dataStructurePage = new DataStructurePage(this);
    _dataTypePage = new DataTypePage(this);
    _pointerTypePage = new PointerTypePage(this);
    mainLayout->addWidget(tabWidget);

    tabWidget->addTab(_dataStructurePage, i18nc("@title:tab", "Data Structure"));
    tabWidget->addTab(_dataTypePage, i18nc("@title:tab", "Data Types"));
    tabWidget->addTab(_pointerTypePage, i18nc("@title:tab", "Pointer Types"));

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QPushButton *closeButton = new QPushButton;
    KGuiItem::assign(closeButton, KStandardGuiItem::close());
    closeButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    buttons->addButton(closeButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    setAttribute(Qt::WA_DeleteOnClose);
}


void DataStructurePropertiesDialog::setDataStructure(DataStructurePtr dataStructure)
{
    Q_ASSERT(dataStructure);
    _dataStructurePage->setDataStructure(dataStructure);
    _dataTypePage->setDocument(dataStructure->document());
    _pointerTypePage->setDocument(dataStructure->document());
}


void DataStructurePropertiesDialog::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10,  screenPosition.y() + 10);
}

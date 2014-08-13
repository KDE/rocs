/*
    This file is part of Rocs,
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
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

#include "DocumentTypesWidget.h"

#include "libgraphtheory/graphdocument.h"
#include "libgraphtheory/node.h"
#include "libgraphtheory/edge.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/edgetype.h"

#include <KLocalizedString>
#include <QDebug>
#include <QAction>
#include <KColorCombo>
#include <QPushButton>
#include <QWidget>

using namespace GraphTheory;

DocumentTypesWidget::DocumentTypesWidget(QWidget* parent)
    : QWidget(parent)
    , m_document()
{
    ui = new Ui::DocumentTypesWidget;
    ui->setupUi(this);
/*
    connect(&DocumentManager::self(), SIGNAL(documentRemoved(Document*)),
            this, SLOT(removeDocument()));
    connect(&DocumentManager::self(), SIGNAL(activateDocument()),
            this, SLOT(updateDocument()));*/
}

DocumentTypesWidget::~DocumentTypesWidget()
{
}

void DocumentTypesWidget::updateDocument()
{
    removeDocument();
//FIXME
//     _document = DocumentManager::self().activeDocument();
//     // create default data element setups
//     Document* document = DocumentManager::self().activeDocument();
//     foreach (int type, document->dataTypeList()) {
//         createDataTypeInformationWidget(type, _document);
//     }
//     foreach (int type, document->pointerTypeList()) {
//         createPointerTypeInformationWidget(type, _document);
//     }
/*
    // react on new data types and pointer types
    connect(_document, SIGNAL(dataTypeCreated(int)), this, SLOT(registerDataType(int)));
    connect(_document, SIGNAL(dataTypeRemoved(int)), this, SLOT(unregisterDataType(int)));
    connect(_document, SIGNAL(pointerTypeCreated(int)), this, SLOT(registerPointerType(int)));
    connect(_document, SIGNAL(pointerTypeRemoved(int)), this, SLOT(unregisterPointerType(int)));*/
}

void DocumentTypesWidget::registerDataType(int identifier)
{
//     createDataTypeInformationWidget(identifier, _document);
}

void DocumentTypesWidget::removeDocument()
{
//     if (_document == 0) {
//         return;
//     }
//
//     _document->disconnect(this);
//
//     // cleanup before filling again
//     QList<int> identifiers;
//     identifiers = _dataTypeWidgets.keys();
//     foreach (int identifier, identifiers) {
//         delete _dataTypeWidgets[identifier];
//         _dataTypeWidgets.remove(identifier);
//         _dataTypeButtons.remove(identifier);
//     }
//     identifiers = _pointerTypeWidgets.keys();
//     foreach (int identifier, identifiers) {
//         delete _pointerTypeWidgets[identifier];
//         _pointerTypeWidgets.remove(identifier);
//         _pointerTypeButtons.remove(identifier);
//     }
//     _document = 0;
}

void DocumentTypesWidget::unregisterDataType(int identifier)
{
//     if (!_dataTypeWidgets.contains(identifier)) {
//         return;
//     }
//     delete _dataTypeWidgets[identifier];
//     _dataTypeWidgets.remove(identifier);
//     _dataTypeButtons.remove(identifier);
}

void DocumentTypesWidget::registerPointerType(int identifier)
{
//     Document* document = DocumentManager::self().activeDocument();
//     createPointerTypeInformationWidget(identifier, document);
}

void DocumentTypesWidget::unregisterPointerType(int identifier)
{
//     if (!_pointerTypeWidgets.contains(identifier)) {
//         return;
//     }
//     delete _pointerTypeWidgets[identifier];
//     _pointerTypeWidgets.remove(identifier);
//     _pointerTypeButtons.remove(identifier);
}

bool DocumentTypesWidget::createDataTypeInformationWidget(int typeIdentifier, GraphDocumentPtr document)
{
//     if (!document->dataTypeList().contains(typeIdentifier)) {
//         return false;
//     }
//
//     DataTypePtr dataType = document->dataType(typeIdentifier);
//
//     // create default data element setups
//     QWidget* dataPropertyWidget = new QWidget(this);
//     QGridLayout* dataPropertyLayout = new QGridLayout(dataPropertyWidget);
//     QPushButton* dataTypeButton = new QPushButton(this);
//     dataTypeButton->setFlat(true);
//     dataTypeButton->setStyleSheet("text-align: left");
//     //FIXME add new functionality
// //     QAction* dataAction = new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), dataType, dataPropertyWidget);
// //     dataTypeButton->addAction(dataAction);
//     dataTypeButton->setIcon(dataType->icon());
//     dataTypeButton->setText(dataType->name());
// //     connect(dataTypeButton, SIGNAL(pressed()), dataAction, SLOT(trigger()));
//
//
//     QPushButton* dataTypeVisible = new QPushButton(this);
//     dataTypeVisible->setIcon(QIcon::fromTheme("rocseyeblack"));
//     dataTypeVisible->setCheckable(true);
//     dataTypeVisible->setChecked(true);
//     dataTypeVisible->setFixedWidth(24);
//
//     dataPropertyWidget->setLayout(dataPropertyLayout);
//     dataPropertyLayout->addWidget(dataTypeButton, 1, 1);
//     dataPropertyLayout->addWidget(dataTypeVisible, 1, 2);
//     ui->_dataTypes->addWidget(dataPropertyWidget);
//
//     connect(dataTypeVisible, SIGNAL(toggled(bool)), dataType.get(), SLOT(setVisible(bool)));
//     connect(dataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateDataTypeButtons()));
//     connect(dataType.get(), SIGNAL(nameChanged(QString)), this, SLOT(updateDataTypeButtons()));
//
//     _dataTypeWidgets.insert(typeIdentifier, dataPropertyWidget);
//     _dataTypeButtons.insert(typeIdentifier, dataTypeButton);

    return true;
}

bool DocumentTypesWidget::createPointerTypeInformationWidget(int typeIdentifier, GraphDocumentPtr document)
{
//     if (!document->pointerTypeList().contains(typeIdentifier)) {
//         return false;
//     }
//
//     PointerTypePtr pointerType = document->pointerType(typeIdentifier);
//
//     // create default data element setups
//     QWidget* pointerPropertyWidget = new QWidget(this);
//     QGridLayout* pointerPropertyLayout = new QGridLayout(pointerPropertyWidget);
//
//     QPushButton* pointerTypeButton = new QPushButton(this);
//     pointerTypeButton->setText(pointerType->name());
//     pointerTypeButton->setFlat(true);
//     pointerTypeButton->setStyleSheet("text-align: left");
//     //FIXME add functionality
// //     QAction* pointerAction = new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), pointerType, pointerPropertyWidget);
// //     connect(pointerTypeButton, SIGNAL(pressed()), pointerAction, SLOT(trigger()));
//
//     QPushButton* pointerTypeVisible = new QPushButton(this);
//     pointerTypeVisible->setIcon(QIcon::fromTheme("rocseyeblack"));
//     pointerTypeVisible->setCheckable(true);
//     pointerTypeVisible->setChecked(true);
//     pointerTypeVisible->setToolTip(i18nc("@info:tooltip", "Set visibility of pointers"));
//     pointerTypeVisible->setFixedWidth(24);
//
//     pointerPropertyWidget->setLayout(pointerPropertyLayout);
//     pointerPropertyLayout->addWidget(pointerTypeButton, 1, 1);
//     pointerPropertyLayout->addWidget(pointerTypeVisible, 1, 2);
//     ui->_pointerTypes->addWidget(pointerPropertyWidget);
//
//     connect(pointerTypeVisible, SIGNAL(toggled(bool)), pointerType.get(), SLOT(setVisible(bool)));
//     connect(pointerType.get(), SIGNAL(nameChanged(QString)), this, SLOT(updatePointerTypeButtons()));
//
//     _pointerTypeWidgets.insert(typeIdentifier, pointerPropertyWidget);
//     _pointerTypeButtons.insert(typeIdentifier, pointerTypeButton);

    return true;
}

void DocumentTypesWidget::updateDataTypeButtons()
{
//     QMap<int, QPushButton*>::const_iterator dataTypeWidget = _dataTypeButtons.constBegin();
//     while (dataTypeWidget != _dataTypeButtons.constEnd()) {
//         Document* activeDocument = DocumentManager::self().activeDocument();
//         DataTypePtr dataType = activeDocument->dataType(dataTypeWidget.key());
//         dataTypeWidget.value()->setIcon(dataType->icon());
//         dataTypeWidget.value()->setText(dataType->name());
//         ++dataTypeWidget;
//     }
}

void DocumentTypesWidget::updatePointerTypeButtons()
{
//     QMap<int, QPushButton*>::const_iterator pointerTypeWidget = _pointerTypeButtons.constBegin();
//     while (pointerTypeWidget != _pointerTypeButtons.constEnd()) {
//         Document* activeDocument = DocumentManager::self().activeDocument();
//         PointerTypePtr pointerType = activeDocument->pointerType(pointerTypeWidget.key());
//         pointerTypeWidget.value()->setText(pointerType->name());
//         ++pointerTypeWidget;
//     }
}

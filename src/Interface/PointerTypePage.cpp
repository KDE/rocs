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

#include "PointerTypePage.h"
#include <Data.h>
#include <DataItem.h>
#include <PointerType.h>
#include <Pointer.h>
#include <DocumentManager.h>

#include <QWidget>
#include <QString>

#include <KDebug>
#include <KLineEdit>
#include <KColorCombo>
#include <KPushButton>
#include <KComboBox>


PointerTypePage::PointerTypePage(QWidget* parent)
    : QWidget(parent)
{
    ui = new Ui::PointerTypePage;
    ui->setupUi(this);

    connect(ui->typeSelector, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setCurrentType(int)));
    connect(ui->exportNewType, SIGNAL(clicked(bool)),
            this, SLOT(createNewType()));
    connect(ui->deleteType, SIGNAL(clicked(bool)),
            this, SLOT(removeType()));
    connect(ui->typeName, SIGNAL(textChanged(QString)),
            this, SLOT(updateCurrentTypeName()));
    connect(ui->typeName, SIGNAL(textEdited(QString)),
            this, SLOT(setTypeName()));
    connect(ui->typeDefaultColor, SIGNAL(activated(QColor)),
            this, SLOT(setTypeDefaultColor()));
}


void PointerTypePage::setDocument(Document* document)
{
    Q_ASSERT(document);
    if (_document) {
        disconnect(document);
    }
    _document = document;

    // setup types
    foreach (int pointerType, _document->pointerTypeList()) {
        ui->typeSelector->addItem(_document->pointerType(pointerType)->name(), QVariant(pointerType));
    }

    ui->typeSelector->setCurrentIndex(0); // default type 0 always exists
    setCurrentType(0);
}


void PointerTypePage::setPointerType(PointerTypePtr pointerType)
{
    //FIXME current workaround: select current active document as parent document
    setDocument(DocumentManager::self()->activeDocument());
    ui->typeSelector->setCurrentIndex(ui->typeSelector->findData(QVariant(pointerType->identifier())));
}


void PointerTypePage::setCurrentType(int index)
{
    int type = ui->typeSelector->itemData(index).toInt();
    if (type==0) {
        ui->deleteType->setDisabled(true);
    } else {
        ui->deleteType->setDisabled(false);
    }

    ui->typeName->setText(_document->pointerType(type)->name());
    //TODO insert style
    ui->typeDefaultColor->setColor(_document->pointerType(type)->defaultColor());
    ui->typeIdentifier->setText(QString::number(type));
}


void PointerTypePage::setTypeName()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    _document->pointerType(type)->setName(ui->typeName->text());
}


void PointerTypePage::setTypeDefaultColor()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    _document->pointerType(type)->setDefaultColor(ui->typeDefaultColor->color());
}


void PointerTypePage::updateCurrentTypeName()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    ui->typeSelector->setItemText(ui->typeSelector->currentIndex(), _document->pointerType(type)->name());
}


void PointerTypePage::createNewType()
{
    int newType = _document->registerPointerType(QString());
    ui->typeSelector->addItem(QString(), QVariant(newType));
    ui->typeSelector->setCurrentIndex(ui->typeSelector->count()-1);
    setCurrentType(ui->typeSelector->count()-1);
    ui->typeName->setFocus();
}


void PointerTypePage::removeType()
{
    int oldType = ui->typeSelector->currentIndex();
    if (oldType==0) {
        return;
    }
    ui->typeSelector->removeItem(oldType);
    _document->removePointerType(oldType);
    ui->typeSelector->setCurrentIndex(0);
    setCurrentType(0);
}

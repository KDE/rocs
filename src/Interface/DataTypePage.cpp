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

#include "DataTypePage.h"
#include <Data.h>
#include <DataItem.h>
#include <DocumentManager.h>

#include <QWidget>
#include <QString>
#include <QSvgRenderer>
#include <QPainter>

#include <KDebug>
#include <KLineEdit>
#include <KColorCombo>
#include <KPushButton>
#include <KComboBox>

DataTypePage::DataTypePage(QWidget* parent)
    : QWidget(parent)
{
    ui = new Ui::DataTypePage;
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
    connect(ui->typeIcon, SIGNAL(activated(QString)),
            this, SLOT(setIcon()));
}


void DataTypePage::setDocument(Document* document)
{
    Q_ASSERT(document);
    if (_document) {
        disconnect(document);
    }
    _document = document;

    // setup types
    foreach (int dataType, _document->dataTypeList()) {
        ui->typeSelector->addItem(_document->dataType(dataType)->name(), QVariant(dataType));
    }

    ui->typeSelector->setCurrentIndex(0); // default type 0 always exists
    setCurrentType(0);
}


void DataTypePage::setDataType(DataTypePtr dataType)
{
    //FIXME current workaround: select current active document as parent document
    setDocument(DocumentManager::self()->activeDocument());
    ui->typeSelector->setCurrentIndex(ui->typeSelector->findData(QVariant(dataType->identifier())));
}

void DataTypePage::setTypeName()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    _document->dataType(type)->setName(ui->typeName->text());
}


void DataTypePage::setTypeDefaultColor()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    _document->dataType(type)->setDefaultColor(ui->typeDefaultColor->color());
}


void DataTypePage::setIcon()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    _document->dataType(type)->setIcon(
            ui->typeIcon->itemData(ui->typeIcon->currentIndex()).toString()
            );
}


void DataTypePage::setCurrentType(int index)
{
    int type = ui->typeSelector->itemData(index).toInt();
    if (type==0) {
        ui->deleteType->setDisabled(true);
    } else {
        ui->deleteType->setDisabled(false);
    }

    ui->typeName->setText(_document->dataType(type)->name());
    ui->typeDefaultColor->setColor(_document->dataType(type)->defaultColor());
    ui->typeIdentifier->setText(QString::number(type));

    // icon selector
    if (!_document->iconPackage().isEmpty()) {
        QFile svgFile(_document->iconPackage());
        svgFile.open(QIODevice::ReadOnly | QIODevice::Text);

        QXmlStreamReader reader(&svgFile);
        QSvgRenderer *renderer = DataItem::sharedRenderer(svgFile.fileName());
        while (!reader.atEnd()) {
            reader.readNext();
            if (!reader.attributes().hasAttribute("id")) {
                continue;
            }
            QString attribute = reader.attributes().value("id").toString();
            if (attribute.startsWith(QLatin1String("rocs_"))) {
                QImage iconImage = QImage(80, 80, QImage::Format_ARGB32);

                QPainter painter;
                painter.begin(&iconImage);
                renderer->render(&painter, attribute);
                painter.end();

                attribute.remove("rocs_");
                ui->typeIcon->addItem(KIcon(QPixmap::fromImage(iconImage)), "", QVariant(attribute));
            }
        }
        if (!_document->dataType(type)->iconName().isEmpty()) {
            QString icon = _document->dataType(type)->iconName();
            icon.remove("rocs_");
            ui->typeIcon->setCurrentIndex(ui->typeIcon->findData(icon));
        }
    }
}


void DataTypePage::updateCurrentTypeName()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    ui->typeSelector->setItemText(ui->typeSelector->currentIndex(), _document->dataType(type)->name());
}


void DataTypePage::createNewType()
{
    int newType = _document->registerDataType(ui->newTypeName->text());
    ui->newTypeName->clear();
    ui->typeSelector->addItem(_document->dataType(newType)->name(), QVariant(newType));
    ui->typeSelector->setCurrentIndex(ui->typeSelector->count()-1);
    setCurrentType(ui->typeSelector->count()-1);
}


void DataTypePage::removeType()
{
    int oldType = ui->typeSelector->currentIndex();
    if (oldType==0) {
        return;
    }
    ui->typeSelector->removeItem(oldType);
    _document->removeDataType(oldType);
    ui->typeSelector->setCurrentIndex(0);
    setCurrentType(0);
}

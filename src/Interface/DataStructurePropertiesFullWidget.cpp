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

#include "DataStructurePropertiesFullWidget.h"
#include <Data.h>
#include <DataItem.h>

#include <QWidget>
#include <QString>
#include <QSvgRenderer>
#include <QPainter>

#include <KDebug>
#include <KLineEdit>
#include <KColorCombo>
#include <KPushButton>
#include <KComboBox>
#include <KDialog>
#include <KVBox>

DataStructurePropertiesFullWidget::DataStructurePropertiesFullWidget(QWidget* parent)
    : KDialog(parent)
{
    ui = new Ui::DataStructurePropertiesFullWidget;
    ui->setupUi(mainWidget());
    setCaption(i18n("Data Structure Properties"));
    setButtons(Ok); //TODO implement changes for (Ok | Cancel)
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->buttonAddDataType, SIGNAL(clicked(bool)), this, SLOT(addDataType()));
    connect(this, SIGNAL(okClicked()), SLOT(close()));
}


void DataStructurePropertiesFullWidget::setDataStructure(DataStructurePtr dataStructure)
{
    Q_ASSERT(dataStructure);
    if (_dataStructure) {
        disconnect(_dataStructure.get());
        foreach(QWidget* oldWidget, _dataTypeWigets) {
            oldWidget->deleteLater();
        }
        _dataTypeWigets.clear();
    }
    _dataStructure = dataStructure;
    ui->dataStructureName->setText(_dataStructure->name());

    setupDataTypes();


    connect (_dataStructure.get(), SIGNAL(dataTypeRemoved(int)),
            this, SLOT(removeDataType(int)));
    connect(ui->dataStructureName, SIGNAL(textChanged(QString)), dataStructure.get(), SLOT(setName(QString)));
}


void DataStructurePropertiesFullWidget::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10,  screenPosition.y() + 10);
}


void DataStructurePropertiesFullWidget::addDataType()
{
    int dataTypeIdentifier = _dataStructure->registerDataType(i18n("type"));
    ui->dataTypes->layout()->addWidget(createDataTypeWidget(dataTypeIdentifier));
}


void DataStructurePropertiesFullWidget::removeDataType(int dataType)
{
    if (_dataTypeWigets.contains(dataType) && _dataTypeWigets[dataType]) {
        ui->dataTypes->layout()->removeWidget(_dataTypeWigets[dataType]);
        _dataTypeWigets[dataType]->deleteLater();
    }
    _dataTypeWigets.remove(dataType);
}


void DataStructurePropertiesFullWidget::setupDataTypes()
{
    foreach (int dataTypeIdentifier, _dataStructure->dataTypeList()) {
        ui->dataTypes->layout()->addWidget(createDataTypeWidget(dataTypeIdentifier));
    }
}


QWidget* DataStructurePropertiesFullWidget::createDataTypeWidget(int dataType)
{
    // create default data element setups
    QWidget* dataTypeWidget = new QWidget(this);
    dataTypeWidget->setLayout(new QHBoxLayout);
    QLabel* dataTypeIdentifier = new QLabel(QString::number(dataType), dataTypeWidget);
    dataTypeIdentifier->setToolTip(i18n("Unique identifier for data type."));
    dataTypeWidget->layout()->addWidget(dataTypeIdentifier);

    KLineEdit* dataTypeName = new KLineEdit(_dataStructure->dataType(dataType)->name(), dataTypeWidget);
    dataTypeWidget->layout()->addWidget(dataTypeName);
    connect(dataTypeName, SIGNAL(textEdited(QString)),
            _dataStructure->dataType(dataType).get(), SLOT(setName(QString)));

    KColorCombo* dataTypeColor = new KColorCombo(dataTypeWidget);
    dataTypeColor->setFixedWidth(50);
    dataTypeColor->setColor(_dataStructure->dataType(dataType)->defaultColor());
    dataTypeWidget->layout()->addWidget(dataTypeColor);
    connect(dataTypeColor, SIGNAL(activated(QColor)),
            _dataStructure->dataType(dataType).get(), SLOT(setDefaultColor(QColor)));

    KComboBox* dataTypeIcon = new KComboBox(dataTypeWidget);
    if (!_dataStructure->iconPackage().isEmpty()) {
        QFile svgFile(_dataStructure->iconPackage());
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
                dataTypeIcon->addItem(KIcon(QPixmap::fromImage(iconImage)), attribute);
            }
        }
        if (!_dataStructure->dataType(dataType)->icon().isEmpty()) {
            QString icon = _dataStructure->dataType(dataType)->icon();
            icon.remove("rocs_");
            dataTypeIcon->setCurrentItem(icon);
        }
        dataTypeWidget->layout()->addWidget(dataTypeIcon);
        connect(dataTypeIcon, SIGNAL(currentIndexChanged(QString)),
                _dataStructure->dataType(dataType).get(), SLOT(setIcon(QString)));
    }

    // default data type can not be deleted
    if (dataType!=0) {
        KPushButton* dataTypeDelete = new KPushButton("x", dataTypeWidget);
        dataTypeWidget->layout()->addWidget(dataTypeDelete);
        connect(dataTypeDelete, SIGNAL(clicked(bool)), _dataStructure->dataType(dataType).get(), SLOT(remove()));
    }

    _dataTypeWigets.insert(dataType, dataTypeWidget);
    return dataTypeWidget;
}

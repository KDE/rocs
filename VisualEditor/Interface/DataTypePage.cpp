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
#include "Scene/DataItem.h"
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


// DataType Property Model

class DataTypePropertyModel : public QAbstractTableModel {
    QList<QString> _propertyList;
    DataTypePtr _dataType;

public:
    DataTypePropertyModel(QObject* parent = 0)
        : QAbstractTableModel(parent)
    {
        emit layoutChanged();
    }

    QString propertyInRow(int row) {
        return _propertyList.at(row);
    }

    void setDataType(DataTypePtr dataType) {
        _dataType = dataType;
        _propertyList = dataType->properties();
        emit layoutChanged();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        // if the role is not for displaying anything, return a default empty value.
        if (role != Qt::DisplayRole) {
            return QVariant();
        }

        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return i18n("Name");
            case 1: return i18n("Default Value");
            case 2: return i18n("Display");
            }
        }
        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (index.isValid() && index.column() == 2) {
            return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
        }
        if (index.isValid()) {
            return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
        }
        return Qt::ItemIsEnabled;
    }

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const {
        if(!parent.isValid()) {
            return _propertyList.count();
        }
        return 0;
    }

    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const {
        if(!parent.isValid()) {
            return 3;
        }
        return 0;
    }

    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const {
        if (role == Qt::DisplayRole) {
            switch(index.column()) {
                case 0: return _propertyList.at(index.row()); break;
                case 1: return _dataType->propertyDefaultValue(_propertyList.at(index.row())); break;
                default: return QVariant();
            }
        }
        if (role == Qt::EditRole) {
            switch(index.column()) {
                case 0: return _propertyList.at(index.row()); break;
                case 1: return _dataType->propertyDefaultValue(_propertyList.at(index.row())); break;
                default: return QVariant();
            }
        }
        if (role == Qt::CheckStateRole && index.column() == 2) {
            if (_dataType->isPropertyVisible(_propertyList.at(index.row()))) {
                return QVariant(Qt::Checked);
            } else {
                return QVariant(Qt::Unchecked);
            }
        }
        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        // checkboxes
        if (index.isValid() && role == Qt::CheckStateRole && index.column() == 2) {
            bool visible = _dataType->isPropertyVisible(_propertyList.at(index.row()));
            _dataType->setPropertyVisible(_propertyList.at(index.row()), !visible);
            emit dataChanged(index,index);
            return true;
        }

        // QVariant
        if (index.isValid() && role == Qt::EditRole) {
            switch (index.column()) {
                case 0: {
                    _dataType->renameProperty(_propertyList.at(index.row()), value.toString());
                    setDataType(_dataType); // trigger reload
                    break;
                    }
                case 1: _dataType->setPropertyDefaultValue(_propertyList.at(index.row()), value); break;
                default: return false;
            }
            return true;
        }
        return false;
    }
};


// DataType Page

DataTypePage::DataTypePage(QWidget* parent)
    : QWidget(parent)
{
    ui = new Ui::DataTypePage;
    ui->setupUi(this);

    // buttons
    ui->addPropertyButton->setIcon(KIcon("rocsnew"));
    ui->removePropertyButton->setIcon(KIcon("rocsdelete"));
    ui->addType->setIcon(KIcon("rocsnew"));
    ui->deleteType->setIcon(KIcon("rocsdelete"));

    // property table
    _model = new DataTypePropertyModel(this);
    ui->propertyList->setModel(_model);
    ui->propertyList->horizontalHeader()->setProperty("stretchLastSection", true);

    connect(ui->typeSelector, SIGNAL(activated(int)),
            this, SLOT(setCurrentType(int)));
    connect(ui->addType, SIGNAL(clicked(bool)),
            this, SLOT(addType()));
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
    connect(ui->addPropertyButton, SIGNAL(clicked(bool)),
            this, SLOT(addProperty()));
    connect(ui->removePropertyButton, SIGNAL(clicked(bool)),
            this, SLOT(removeProperty()));
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
        QString item = i18nc(
            "@item:inlistbox",
            "%1 (ID %2)",
            _document->dataType(dataType)->name(),
            _document->dataType(dataType)->identifier());
        ui->typeSelector->addItem(item, QVariant(dataType));
    }

    ui->typeSelector->setCurrentIndex(0); // default type 0 always exists
    setCurrentType(0);
}


void DataTypePage::setDataType(DataTypePtr dataType)
{
    //FIXME current workaround: select current active document as parent document
    setDocument(DocumentManager::self().activeDocument());
    ui->typeSelector->setCurrentIndex(ui->typeSelector->findData(QVariant(dataType->identifier())));
    _model->setDataType(dataType);
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

void DataTypePage::addProperty()
{
    int typeID = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    DataTypePtr type = _document->dataType(typeID);
    int counter = 1;
    QString newProperty = i18nc("Property identifier, only a-Z and no spaces", "property%1", counter);
    while (type->properties().contains(newProperty)) {
        newProperty = i18nc("Property identifier, only a-Z and no spaces", "property%1", ++counter);
    }
    type->addProperty(newProperty, "");
    _model->setDataType(type);
}


void DataTypePage::removeProperty()
{
    int typeID = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    DataTypePtr type = _document->dataType(typeID);
    int row = ui->propertyList->currentIndex().row();
    if (row < 0) {
        return;
    }
    type->removeProperty(_model->propertyInRow(row));
    _model->setDataType(type); // update table
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

    // icon selector
    ui->typeIcon->clear();
    if (!_document->iconPackage().isEmpty()) {
        QFile svgFile(_document->iconPackage());
        svgFile.open(QIODevice::ReadOnly | QIODevice::Text);

        QXmlStreamReader reader(&svgFile);
        QSvgRenderer *renderer = DocumentManager::self().sharedRenderer(svgFile.fileName());

        while (!reader.atEnd()) {
            reader.readNext();
            if (!reader.attributes().hasAttribute("id")) {
                continue;
            }
            QString attribute = reader.attributes().value("id").toString();
            if (attribute.startsWith(QLatin1String("rocs_"))) {
                QImage iconImage = QImage(80, 80, QImage::Format_ARGB32);
                iconImage.fill(Qt::transparent);

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
    int typeID = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    DataTypePtr type = _document->dataType(typeID);
    QString item = i18nc(
        "@item:inlistbox",
        "%1 (ID %2)",
        type->name(),
        type->identifier());
    ui->typeSelector->setItemText(ui->typeSelector->currentIndex(), item);
}


void DataTypePage::addType()
{
    int newTypeID = _document->registerDataType(i18nc("@item:inlistbox", "Data Type"));
    DataTypePtr newType = _document->dataType(newTypeID);
    QString item = i18nc(
        "@item:inlistbox",
        "%1 (ID %2)",
        newType->name(),
        newType->identifier());
    ui->typeSelector->addItem(item, QVariant(newTypeID));
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

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


// PointerType Property Model

class PointerTypePropertyModel : public QAbstractTableModel {
    QList<QString> _propertyList;
    PointerTypePtr _pointerType;

public:
    PointerTypePropertyModel(QObject* parent = 0)
        : QAbstractTableModel(parent)
    {
        emit layoutChanged();
    }

    QString propertyInRow(int row) {
        return _propertyList.at(row);
    }

    void setPointerType(PointerTypePtr pointerType) {
        _pointerType = pointerType;
        _propertyList = pointerType->properties();
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
                case 1: return _pointerType->propertyDefaultValue(_propertyList.at(index.row())); break;
                case 2: return _pointerType->isPropertyVisible(_propertyList.at(index.row())); break;
                default: return QVariant();
            }
        }
        if (role == Qt::EditRole) {
            switch(index.column()) {
                case 0: return _propertyList.at(index.row()); break;
                case 1: return _pointerType->propertyDefaultValue(_propertyList.at(index.row())); break;
                case 2: return _pointerType->isPropertyVisible(_propertyList.at(index.row())); break;
                default: return QVariant();
            }
        }
        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (index.isValid() && role == Qt::EditRole) {
            switch (index.column()) {
                case 0: break;
                case 1: _pointerType->setPropertyDefaultValue(_propertyList.at(index.row()), value); break;
                case 2: _pointerType->setPropertyVisible(_propertyList.at(index.row()), value.toBool()); break;
                default: return false;
            }
            return true;
        }
        return false;
    }
};


// PointerType Page

PointerTypePage::PointerTypePage(QWidget* parent)
    : QWidget(parent)
{
    ui = new Ui::PointerTypePage;
    ui->setupUi(this);

    // buttons
    ui->addPropertyButton->setIcon(KIcon("rocsnew"));
    ui->removePropertyButton->setIcon(KIcon("rocsdelete"));
    ui->addType->setIcon(KIcon("rocsnew"));
    ui->deleteType->setIcon(KIcon("rocsdelete"));

    // property table
    _model = new PointerTypePropertyModel(this);
    ui->propertyList->setModel(_model);
    ui->propertyList->horizontalHeader()->setProperty("stretchLastSection", true);

    // create line style selector
    ui->typeLineStyle->addItem(i18nc("@item:inlistbox", "solid line"), Qt::SolidLine);
    ui->typeLineStyle->addItem(i18nc("@item:inlistbox", "dash line"), Qt::DashLine);
    ui->typeLineStyle->addItem(i18nc("@item:inlistbox", "dot line "), Qt::DotLine);
    ui->typeLineStyle->addItem(i18nc("@item:inlistbox", "dash dot line"), Qt::DashDotLine);

    // create pointer direction selector
    ui->typePointerDirection->addItem(i18nc("@item:inlistbox", "Unidirectional"), PointerType::Unidirectional);
    ui->typePointerDirection->addItem(i18nc("@item:inlistbox", "Bidirectional"), PointerType::Bidirectional);

    connect(ui->typeSelector, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setCurrentType(int)));
    connect(ui->addType, SIGNAL(clicked(bool)),
            this, SLOT(addType()));
    connect(ui->deleteType, SIGNAL(clicked(bool)),
            this, SLOT(removeType()));
    connect(ui->typeName, SIGNAL(textChanged(QString)),
            this, SLOT(updateCurrentTypeName()));
    connect(ui->typePointerDirection, SIGNAL(activated(int)),
            this, SLOT(updateCurrentTypeDirection()));
    connect(ui->typeLineStyle, SIGNAL(activated(int)),
            this, SLOT(updateCurrentTypeLineStyle()));
    connect(ui->typeName, SIGNAL(textEdited(QString)),
            this, SLOT(setTypeName()));
    connect(ui->typeDefaultColor, SIGNAL(activated(QColor)),
            this, SLOT(setTypeDefaultColor()));
    connect(ui->addPropertyButton, SIGNAL(clicked(bool)),
            this, SLOT(addProperty()));
    connect(ui->removePropertyButton, SIGNAL(clicked(bool)),
            this, SLOT(removeProperty()));
}


void PointerTypePage::setDocument(Document* document)
{
    Q_ASSERT(document);
    if (_document) {
        disconnect(document);
    }
    _document = document;

    // setup types
    foreach (int pointerTypeID, _document->pointerTypeList()) {
        PointerTypePtr pointerType = _document->pointerType(pointerTypeID);
        QString item = i18nc(
            "@item:inlistbox",
            "%1 (ID %2)",
            pointerType->name(),
            pointerType->identifier());
        ui->typeSelector->addItem(item, QVariant(pointerTypeID));
    }

    ui->typeSelector->setCurrentIndex(0); // default type 0 always exists
    setCurrentType(0);
}


void PointerTypePage::setPointerType(PointerTypePtr pointerType)
{
    //FIXME current workaround: select current active document as parent document
    setDocument(DocumentManager::self()->activeDocument());
    ui->typeSelector->setCurrentIndex(ui->typeSelector->findData(QVariant(pointerType->identifier())));
    _model->setPointerType(pointerType);
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
    ui->typeLineStyle->setCurrentIndex(
        ui->typeLineStyle->findData(_document->pointerType(type)->lineStyle()));
    ui->typePointerDirection->setCurrentIndex(
        ui->typePointerDirection->findData(_document->pointerType(type)->direction()));
    ui->typeDefaultColor->setColor(_document->pointerType(type)->defaultColor());
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
    int typeID = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    PointerTypePtr type = _document->pointerType(typeID);
    QString item = i18nc(
        "@item:inlistbox",
        "%1 (ID %2)",
        type->name(),
        type->identifier());
    ui->typeSelector->setItemText(ui->typeSelector->currentIndex(), item);
}


void PointerTypePage::updateCurrentTypeDirection()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    int direction = ui->typePointerDirection->itemData(ui->typePointerDirection->currentIndex()).toInt();
    _document->pointerType(type)->setDirection(PointerType::Direction(direction));
}


void PointerTypePage::updateCurrentTypeLineStyle()
{
    int type = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    int style = ui->typeLineStyle->itemData(ui->typeLineStyle->currentIndex()).toInt();
    _document->pointerType(type)->setLineStyle(Qt::PenStyle(style));
}


void PointerTypePage::addProperty()
{
    int typeID = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    PointerTypePtr type = _document->pointerType(typeID);
    int counter = 1;
    QString newProperty = i18nc("Property identifier, only a-Z and no spaces", "property%1", counter);
    while (type->properties().contains(newProperty)) {
        newProperty = i18nc("Property identifier, only a-Z and no spaces", "property%1", ++counter);
    }
    type->addProperty(newProperty, "");
    _model->setPointerType(type);
}


void PointerTypePage::removeProperty()
{
    int typeID = ui->typeSelector->itemData(ui->typeSelector->currentIndex()).toInt();
    PointerTypePtr type = _document->pointerType(typeID);
    int row = ui->propertyList->currentIndex().row();
    if (row < 0) {
        return;
    }
    type->removeProperty(_model->propertyInRow(row));
    _model->setPointerType(type); // update table
}

void PointerTypePage::addType()
{
    int newTypeID = _document->registerPointerType(i18nc("@item:inlistbox", "Connection"));
    PointerTypePtr newType = _document->pointerType(newTypeID);
    QString item = i18nc(
        "@item:inlistbox",
        "%1 (ID %2)",
        newType->name(),
        newType->identifier());
    ui->typeSelector->addItem(item, QVariant(newTypeID));
    ui->typeSelector->setCurrentIndex(ui->typeSelector->count()-1);
    setCurrentType(ui->typeSelector->count()-1);
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

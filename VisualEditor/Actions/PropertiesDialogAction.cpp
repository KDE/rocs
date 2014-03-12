/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "PropertiesDialogAction.h"


#include "Interface/DataStructurePropertiesDialog.h"
#include "Interface/DocumentPropertiesDialog.h"
#include "Interface/DataPropertiesWidget.h"
#include "Interface/DataTypePage.h"
#include "Interface/PointerPropertiesWidget.h"
#include "Interface/PointerTypePage.h"
#include <DocumentManager.h>
#include <Document.h>

#include <KGuiItem>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QPointer>
#include <QDebug>
#include <QIcon>
#include <QDialog>
#include <QDialogButtonBox>

PropertiesDialogAction::PropertiesDialogAction(QString text, Document* document, QObject* parent)
    : QAction(text, parent)
{
    _document = document;
    _dialogType = DOCUMENT;
    this->setIcon(QIcon::fromTheme("document-properties"));
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, DataStructurePtr dataStructure, QObject* parent)
    : QAction(text, parent)
{
    _dataStructure = dataStructure;
    _dialogType = DATASTRUCTURE;
    this->setIcon(QIcon::fromTheme("document-properties"));
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, DataPtr data, QObject* parent)
    : QAction(text, parent)
{
    _data = data;
    _dialogType = DATA;
    this->setIcon(QIcon::fromTheme("document-properties"));
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, PointerPtr pointer, QObject* parent)
    : QAction(text, parent)
{
    _pointer = pointer;
    _dialogType = POINTER;
    this->setIcon(QIcon::fromTheme("document-properties"));
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, DataTypePtr dataType, QObject* parent)
    : QAction(text, parent)
{
    _dataType = dataType;
    _dialogType = DATATYPE;
    this->setIcon(QIcon::fromTheme("document-properties"));
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}


PropertiesDialogAction::PropertiesDialogAction(QString text, PointerTypePtr pointerType, QObject* parent)
    : QAction(text, parent)
{
    _pointerType = pointerType;
    _dialogType = POINTERTYPE;
    this->setIcon(QIcon::fromTheme("document-properties"));
    connect(this, SIGNAL(triggered()), this, SLOT(showDialog()));
}

void PropertiesDialogAction::showDialog()
{
    switch (_dialogType) {
    case DOCUMENT: {
        if (!_document) {
            return;
        }
        QPointer<DocumentPropertiesDialog> dialog = new DocumentPropertiesDialog;
        dialog->setDocument(_document);
        if (!_screenPosition.isNull()) {
            dialog->setPosition(_screenPosition);
        }
        dialog->exec();
        break;
    }
    case DATASTRUCTURE: {
        if (!_dataStructure) {
            return;
        }
        QPointer<DataStructurePropertiesDialog> dialog = new DataStructurePropertiesDialog;
        dialog->setDataStructure(_dataStructure);
        if (!_screenPosition.isNull()) {
            dialog->setPosition(_screenPosition);
        }
        dialog->exec();
        break;
    }
    case DATA: {
        if (!_data) {
            return;
        }
        QPointer<DataPropertiesWidget> dialog = new DataPropertiesWidget(_data);
        if (!_screenPosition.isNull()) {
            dialog->setPosition(_screenPosition);
        }
        dialog->exec();
        break;
    }
    case POINTER: {
        if (!_pointer) {
            return;
        }
        QPointer<PointerPropertiesWidget> dialog = new PointerPropertiesWidget(_pointer);
        if (!_screenPosition.isNull()) {
            dialog->setPosition(_screenPosition);
        }
        dialog->exec();
        break;
    }
    case DATATYPE: {
        if (!_dataType) {
            return;
        }
        QPointer<QDialog> dialog = new QDialog;
        DataTypePage *typePage = new DataTypePage(dialog);
        typePage->setDataType(_dataType);
        dialog->setWindowTitle(i18nc("@title:window", "Data Type Properties"));
        QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
        dialog->setLayout(mainLayout);
        mainLayout->addWidget(typePage);

        QDialogButtonBox *buttons = new QDialogButtonBox(dialog);
        QPushButton *closeButton = new QPushButton;
        KGuiItem::assign(closeButton, KStandardGuiItem::close());
        closeButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        buttons->addButton(closeButton, QDialogButtonBox::RejectRole);
        mainLayout->addWidget(buttons);
        connect(closeButton, SIGNAL(clicked()), dialog, SLOT(reject()));

        dialog->setAttribute(Qt::WA_DeleteOnClose);
        if (!_screenPosition.isNull()) {
            dialog->move(_screenPosition.x() + 10, _screenPosition.y() + 10);
        }
        dialog->exec();
        break;
    }
    case POINTERTYPE: {
        if (!_pointerType) {
            return;
        }
        QPointer<QDialog> dialog = new QDialog;
        PointerTypePage* typePage = new PointerTypePage(dialog);
        typePage->setPointerType(_pointerType);
        dialog->setWindowTitle(i18nc("@title:window", "Pointer Type Properties"));
        QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
        dialog->setLayout(mainLayout);
        mainLayout->addWidget(typePage);

        QDialogButtonBox *buttons = new QDialogButtonBox(dialog);
        QPushButton *closeButton = new QPushButton;
        KGuiItem::assign(closeButton, KStandardGuiItem::close());
        closeButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        buttons->addButton(closeButton, QDialogButtonBox::RejectRole);
        mainLayout->addWidget(buttons);
        connect(closeButton, SIGNAL(clicked()), dialog, SLOT(reject()));

        dialog->setAttribute(Qt::WA_DeleteOnClose);
        if (!_screenPosition.isNull()) {
            dialog->move(_screenPosition.x() + 10, _screenPosition.y() + 10);
        }
        dialog->exec();
        break;
    }
    default:
        break;
    }
}


void PropertiesDialogAction::setPosition(QPointF screenPosition)
{
    _screenPosition = screenPosition;
}

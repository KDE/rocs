/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "AssignValuesWidget.h"
#include "ui_AssignValuesWidget.h"

#include "Document.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "Pointer.h"
#include "Data.h"
#include "Modifiers/ValueModifier.h"

#include <limits.h>
#include <KLocalizedString>
#include <KComboBox>
#include <KGuiItem>
#include <KStandardGuiItem>
#include <QDialog>
#include <QPushButton>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QMap>
#include <QPair>
#include <QDebug>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

AssignValuesWidget::AssignValuesWidget(Document *graphDoc, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Assign Values"));

    QWidget *widget = new QWidget(this);
    ui = new Ui::AssignValuesWidget;
    ui->setupUi(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(widget);

    // set ui
    ui->buttonShowAdvanced->setIcon(QIcon::fromTheme("rocsadvancedsetup"));

    // set types
    Document* document = DocumentManager::self().activeDocument();
    ui->dataType->addItem(i18n("All Data Types"), -1);
    foreach (int typeId, document->dataTypeList()) {
        ui->dataType->addItem(document->dataType(typeId)->name(), typeId);
    }
    ui->connectionType->addItem(i18n("All Connection Types"), -1);
    foreach (int typeId, document->pointerTypeList()) {
        ui->connectionType->addItem(document->pointerType(typeId)->name(), typeId);
    }

    // set selection combos only enabled if they are used for assignment
    ui->dataType->setEnabled(ui->applyToDataElements->isChecked());
    ui->connectionType->setEnabled(ui->applyToConnections->isChecked());
    connect(ui->applyToConnections, SIGNAL(toggled(bool)), ui->connectionType, SLOT(setEnabled(bool)));
    connect(ui->applyToDataElements, SIGNAL(toggled(bool)), ui->dataType, SLOT(setEnabled(bool)));

    // set all available properties as possible completes
    KCompletion *complete = ui->propertyName->completionObject();
    foreach (int typeId, document->dataTypeList()) {
        complete->insertItems(document->dataType(typeId)->properties());
    }
    foreach (int typeId, document->pointerTypeList()) {
        complete->insertItems(document->pointerType(typeId)->properties());
    }
    ui->propertyName->setContextMenuPolicy(Qt::DefaultContextMenu);
    ui->propertyName->setPlaceholderText(i18n("Enter Property Name"));
    connect(ui->propertyName, SIGNAL(textChanged(QString)), this, SLOT(updateApplyButtonStates()));

    // set random seeds
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    uint badRandomSeed = qHash(currentTime) % 99999;
    badRandomSeed = (badRandomSeed == 0) ? 1 : badRandomSeed;
    ui->spinBoxIntegerGeneratorSeed->setValue(badRandomSeed);
    ui->spinBoxFloatGeneratorSeed->setValue(badRandomSeed);

    // set visibility for advanced options
    // TODO move to containers for easier handling
    ui->checkBoxOverwriteValues->setVisible(false);
    ui->label->setVisible(false);
    ui->spinBoxIntegerGeneratorSeed->setVisible(false);
    ui->label_7->setVisible(false);
    ui->spinBoxFloatGeneratorSeed->setVisible(false);
    ui->label_9->setVisible(false);
    ui->LabelInitialString->setVisible(false);
    ui->LineEditInitialString->setVisible(false);

    // add controls
    QDialogButtonBox *buttons = new QDialogButtonBox(this);

    m_applyButton = new QPushButton(this);
    KGuiItem::assign(m_applyButton, KStandardGuiItem::apply());
    buttons->addButton(m_applyButton, QDialogButtonBox::ApplyRole);
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(assignValues()));

    QPushButton *cancelButton = new QPushButton(this);
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    m_okButton = new QPushButton(this);
    KGuiItem::assign(m_okButton, KStandardGuiItem::ok());
    m_okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    buttons->addButton(m_okButton, QDialogButtonBox::AcceptRole);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(assignValues()));

    mainLayout->addWidget(buttons);
    updateApplyButtonStates();
}


AssignValuesWidget::~AssignValuesWidget()
{
    delete ui;
}


void AssignValuesWidget::addDataStructures(const QStringList& dsNames)
{
    ui->dataStructuresCombo->insertItems(0, dsNames);
}


void AssignValuesWidget::updateApplyButtonStates()
{
    if (Document::isValidIdentifier(ui->propertyName->text())) {
        m_applyButton->setEnabled(true);
        m_okButton->setEnabled(true);
    }
    else {
        m_applyButton->setEnabled(false);
        m_okButton->setEnabled(false);
    }
}


void AssignValuesWidget::assignValues()
{
    DataStructurePtr ds;
    QList< DataStructurePtr > dsList = DocumentManager::self().activeDocument()->dataStructures();

    // no data structures present at active document
    if (ui->dataStructuresCombo->count() == 0) {
        return;
    }

    ds = dsList[ui->dataStructuresCombo->currentIndex()];
    QString property = ui->propertyName->text();
    if (!ds || property.isEmpty()) {
        return;
    }

    bool overrideValues = ui->checkBoxOverwriteValues->isChecked();

    // select all pointer lists and (if necessary) add specified property to them
    DataList dataList;
    PointerList pointerList;
    if (ui->applyToDataElements) {
        int typeSelection = ui->dataType->itemData(ui->dataType->currentIndex()).toInt();
        // case: all properties are selected
        if (typeSelection == -1) {
            foreach (int typeId, ds->document()->dataTypeList()) {
                if (!ds->document()->dataType(typeId)->properties().contains(property)) {
                    ds->document()->dataType(typeId)->addProperty(property);
                }
                dataList << ds->dataList(typeId);
            }
        }
        if (typeSelection >= 0) {
            if (!ds->document()->dataType(typeSelection)->properties().contains(property)) {
                ds->document()->dataType(typeSelection)->addProperty(property);
            }
            dataList = ds->dataList(typeSelection);
        }
    }
    if (ui->applyToConnections) {
        int typeSelection = ui->connectionType->itemData(ui->connectionType->currentIndex()).toInt();
        // case: all properties are selected
        if (typeSelection == -1) {
            foreach (int typeId, ds->document()->pointerTypeList()) {
                if (!ds->document()->pointerType(typeId)->properties().contains(property)) {
                    ds->document()->pointerType(typeId)->addProperty(property);
                }
                pointerList << ds->pointers(typeId);
            }
        }
        if (typeSelection >= 0) {
            if (!ds->document()->pointerType(typeSelection)->properties().contains(property)) {
                ds->document()->pointerType(typeSelection)->addProperty(property);
            }
            pointerList = ds->pointers(typeSelection);
        }
    }

    // assign values
    ValueModifier modifier;
    switch ((AssignMethod) ui->comboBoxMethod->currentIndex()) {
    case ID: {
        int start = ui->spinBoxIDStartValue->value();

        if (ui->applyToDataElements->isChecked()) {
	    modifier.enumerate(dataList, property, start, "", overrideValues);
        }
        if (ui->applyToConnections->isChecked()) {
	    modifier.enumerate(pointerList, property, start, "", overrideValues);
        }
        break;
    }
    case ALPHA: {
        QString start = ui->LineEditInitialString->text();

        if (ui->applyToDataElements->isChecked()) {
            modifier.enumerateAlpha(dataList, property, start, overrideValues);
        }
        if (ui->applyToConnections->isChecked()) {
            modifier.enumerateAlpha(pointerList, property, start, overrideValues);
        }
        break;
    }
    case ID_ALPHA: {
        int start = ui->SpinBoxAlphaNumericIDStart->value();
        QString prefix = ui->LineEditAlphaNumericPrefix->text();

        if (ui->applyToDataElements->isChecked()) {
            modifier.enumerate(dataList, property, start, prefix, overrideValues);
        }
        if (ui->applyToConnections->isChecked()) {
            modifier.enumerate(pointerList, property, start, prefix, overrideValues);
        }
        break;
    }
    case UNIFORM_INTEGER: {
        int seed = ui->spinBoxIntegerGeneratorSeed->value();
        int lowerLimit = ui->spinBoxIntegerLowerLimit->value();
        int upperLimit = ui->spinBoxIntegerUpperLimit->value();

        if (ui->applyToDataElements->isChecked()) {
            modifier.assignRandomIntegers(dataList, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        if (ui->applyToConnections->isChecked()) {
            modifier.assignRandomIntegers(pointerList, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        break;
    }
    case UNIFORM_FLOAT: {
        int seed = ui->spinBoxFloatGeneratorSeed->value();
        qreal lowerLimit = ui->spinBoxFloatLowerLimit->value();
        qreal upperLimit = ui->spinBoxFloatUpperLimit->value();

        if (ui->applyToDataElements->isChecked()) {
            modifier.assignRandomReals(dataList, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        if (ui->applyToConnections->isChecked()) {
            modifier.assignRandomReals(pointerList, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        break;
    }
    case CONSTANT: {
        QString constant = ui->lineEditConstantValue->text();

        if (ui->applyToDataElements->isChecked()) {
            modifier.assignConstantValue(dataList, property, constant, overrideValues);
        }
        if (ui->applyToConnections->isChecked()) {
            modifier.assignConstantValue(pointerList, property, constant, overrideValues);
        }
    }
    }
}

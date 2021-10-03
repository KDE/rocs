/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "assignvalueswidget.h"
#include "graphdocument.h"
#include "modifiers/valueassign.h"
#include "logging_p.h"

#include <limits.h>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QMap>
#include <QPair>
#include <QDialogButtonBox>
#include <QDateTime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

using namespace GraphTheory;

AssignValuesWidget::AssignValuesWidget(GraphDocumentPtr document, QWidget *parent)
    : QDialog(parent)
    , m_document(document)
    , m_okButton(nullptr)
    , m_applyButton(nullptr)
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

    if (!m_document) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No document given, aborting.";
        return;
    }

    // set types
    ui->nodeType->addItem(i18n("All Data Types"), -1);
    const auto nodeTypes = m_document->nodeTypes();
    for (const NodeTypePtr &type : nodeTypes) {
        ui->nodeType->addItem(type->name());
    }
    ui->edgeType->addItem(i18n("All Edge Types"), -1);
    const auto edgeTypes = document->edgeTypes();
    for (const EdgeTypePtr &type : edgeTypes) {
        ui->edgeType->addItem(type->name());
    }

    connect(ui->buttons, &QDialogButtonBox::accepted, this, &AssignValuesWidget::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected, this, &AssignValuesWidget::reject);

    // set selection combos only enabled if they are used for assignment
    ui->nodeType->setEnabled(ui->applyToNodes->isChecked());
    ui->edgeType->setEnabled(ui->applyToEdges->isChecked());
    connect(ui->applyToNodes, &QRadioButton::toggled, ui->nodeType, &QComboBox::setEnabled);
    connect(ui->applyToEdges, &QRadioButton::toggled, ui->edgeType, &QComboBox::setEnabled);

    // set all available properties as possible completes
    KCompletion *complete = ui->propertyName->completionObject();
    for (const NodeTypePtr &type : nodeTypes) {
        complete->insertItems(type->dynamicProperties());
    }
    for (const EdgeTypePtr &type : edgeTypes) {
        complete->insertItems(type->dynamicProperties());
    }
    ui->propertyName->setContextMenuPolicy(Qt::DefaultContextMenu);
    ui->propertyName->setPlaceholderText(i18n("Enter Property Name"));
    connect(ui->propertyName, &QLineEdit::textChanged, this, &AssignValuesWidget::updateApplyButtonStates);

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
    connect(m_applyButton, &QPushButton::clicked, this, &AssignValuesWidget::assignValues);

    QPushButton *cancelButton = new QPushButton(this);
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    m_okButton = new QPushButton(this);
    KGuiItem::assign(m_okButton, KStandardGuiItem::ok());
    m_okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    buttons->addButton(m_okButton, QDialogButtonBox::AcceptRole);
    connect(m_okButton, &QPushButton::clicked, this, &AssignValuesWidget::assignValues);

    mainLayout->addWidget(buttons);
    updateApplyButtonStates();
}

AssignValuesWidget::~AssignValuesWidget()
{
    m_document.clear();
    delete ui;
}

void AssignValuesWidget::updateApplyButtonStates()
{
    if (!ui->propertyName->text().isEmpty()) {
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
    bool overrideValues = ui->checkBoxOverwriteValues->isChecked();

    // select all pointer lists and (if necessary) add specified property to them
    NodeList nodes;
    EdgeList edges;
    QString property = ui->propertyName->text();

    if (ui->applyToNodes) {
        int typeSelection = ui->nodeType->itemData(ui->nodeType->currentIndex()).toInt();
        // case: all properties are selected
        if (typeSelection == -1) {
            nodes = m_document->nodes();
            const auto nodeTypes = m_document->nodeTypes();
            for (const NodeTypePtr &type : nodeTypes) {
                type->addDynamicProperty(property);
            }
        }
        if (typeSelection >= 0) {
            m_document->nodeTypes().at(typeSelection)->addDynamicProperty(property);
            nodes = m_document->nodes(m_document->nodeTypes().at(typeSelection));
        }
    }
    if (ui->applyToEdges) {
        int typeSelection = ui->edgeType->itemData(ui->edgeType->currentIndex()).toInt();
        // case: all properties are selected
        if (typeSelection == -1) {
            edges = m_document->edges();
            const auto edgeTypes = m_document->edgeTypes();
            for (const EdgeTypePtr& type : edgeTypes) {
                type->addDynamicProperty(property);
            }
        }
        if (typeSelection >= 0) {
            m_document->edgeTypes().at(typeSelection)->addDynamicProperty(property);
            edges = m_document->edges(m_document->edgeTypes().at(typeSelection));
        }
    }

    // assign values
    ValueAssign modifier;
    switch ((AssignMethod) ui->comboBoxMethod->currentIndex()) {
    case ID: {
        int start = ui->spinBoxIDStartValue->value();

        if (ui->applyToNodes->isChecked()) {
            modifier.enumerate(nodes, property, start, "", overrideValues);
        }
        if (ui->applyToEdges->isChecked()) {
            modifier.enumerate(edges, property, start, "", overrideValues);
        }
        break;
    }
    case ALPHA: {
        QString start = ui->LineEditInitialString->text();

        if (ui->applyToNodes->isChecked()) {
            modifier.enumerateAlpha(nodes, property, start, overrideValues);
        }
        if (ui->applyToEdges->isChecked()) {
            modifier.enumerateAlpha(edges, property, start, overrideValues);
        }
        break;
    }
    case ID_ALPHA: {
        int start = ui->SpinBoxAlphaNumericIDStart->value();
        QString prefix = ui->LineEditAlphaNumericPrefix->text();

        if (ui->applyToNodes->isChecked()) {
            modifier.enumerate(nodes, property, start, prefix, overrideValues);
        }
        if (ui->applyToEdges->isChecked()) {
            modifier.enumerate(edges, property, start, prefix, overrideValues);
        }
        break;
    }
    case UNIFORM_INTEGER: {
        int seed = ui->spinBoxIntegerGeneratorSeed->value();
        int lowerLimit = ui->spinBoxIntegerLowerLimit->value();
        int upperLimit = ui->spinBoxIntegerUpperLimit->value();

        if (ui->applyToNodes->isChecked()) {
            modifier.assignRandomIntegers(nodes, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        if (ui->applyToEdges->isChecked()) {
            modifier.assignRandomIntegers(edges, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        break;
    }
    case UNIFORM_FLOAT: {
        int seed = ui->spinBoxFloatGeneratorSeed->value();
        qreal lowerLimit = ui->spinBoxFloatLowerLimit->value();
        qreal upperLimit = ui->spinBoxFloatUpperLimit->value();

        if (ui->applyToNodes->isChecked()) {
            modifier.assignRandomReals(nodes, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        if (ui->applyToEdges->isChecked()) {
            modifier.assignRandomReals(edges, property, lowerLimit, upperLimit, seed, overrideValues);
        }
        break;
    }
    case CONSTANT: {
        QString constant = ui->lineEditConstantValue->text();

        if (ui->applyToNodes->isChecked()) {
            modifier.assignConstantValue(nodes, property, constant, overrideValues);
        }
        if (ui->applyToEdges->isChecked()) {
            modifier.assignConstantValue(edges, property, constant, overrideValues);
        }
    }
    }
}

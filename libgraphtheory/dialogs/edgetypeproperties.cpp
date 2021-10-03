/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "edgetypeproperties.h"
#include "edge.h"
#include "edgetypestyle.h"
#include "propertieswidget.h"
#include "graphdocument.h"

#include <KColorButton>
#include <KGuiItem>
#include <KLocalizedString>
#include <KStandardGuiItem>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>

using namespace GraphTheory;

EdgeTypeProperties::EdgeTypeProperties(QWidget *parent)
    : QDialog(parent)
    , m_name(new QLineEdit(this))
    , m_id(new QSpinBox(this))
    , m_color(new KColorButton(this))
    , m_direction(new QComboBox(this))
    , m_visible(new QCheckBox(i18n("Edges"), this))
    , m_propertyNamesVisible(new QCheckBox(i18n("Property Names"), this))
    , m_properties(new PropertiesWidget(this))
    , m_okButton(new QPushButton(this))
    , m_type(EdgeTypePtr())
{
    setWindowTitle(i18nc("@title:window", "Edge Type Properties"));

    // form layout with static edge type properties
    QWidget *widget = new QWidget(this);
    QFormLayout *head = new QFormLayout(widget);
    head->addRow(i18n("Name"), m_name);
    head->addRow(i18n("Identifier"), m_id);
    m_id->setMinimum(1);
    head->addRow(i18n("Color"), m_color);
    m_direction->addItem(QIcon::fromTheme("rocsunidirectional"), i18n("Unidirectional"), EdgeType::Unidirectional);
    m_direction->addItem(QIcon::fromTheme("rocsbidirectional"), i18n("Bidirectional"), EdgeType::Bidirectional);
    head->addRow(i18n("Direction"), m_direction);
    // set visibilities row
    QWidget *visibilityWidget = new QWidget(this);
    QVBoxLayout *visibilityForm = new QVBoxLayout(visibilityWidget);
    visibilityForm->addWidget(m_visible);
    visibilityForm->addWidget(m_propertyNamesVisible);
    visibilityWidget->setLayout(visibilityForm);
    head->addRow(i18n("Visibility"), visibilityWidget);
    // set layout
    widget->setLayout(head);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->addWidget(widget);
    mainLayout->addWidget(m_properties);

    // dialog buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(this);

    KGuiItem::assign(m_okButton, KStandardGuiItem::ok());
    m_okButton->setShortcut(Qt::Key_Return);

    QPushButton *cancelButton = new QPushButton;
    KGuiItem::assign(cancelButton, KStandardGuiItem::cancel());
    cancelButton->setShortcut(Qt::Key_Escape);

    buttons->addButton(m_okButton, QDialogButtonBox::AcceptRole);
    buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(m_okButton, &QPushButton::clicked,
        this, &EdgeTypeProperties::accept);
    connect(cancelButton, &QPushButton::clicked,
        this, &EdgeTypeProperties::reject);

    connect(m_id, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &EdgeTypeProperties::validateIdInput);
    connect(this, &QDialog::accepted,
        this, &EdgeTypeProperties::apply);
    setAttribute(Qt::WA_DeleteOnClose);
}

void EdgeTypeProperties::setType(EdgeTypePtr type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;

    m_name->setText(type->name());
    m_id->setValue(type->id());
    m_color->setColor(type->style()->color());
    m_direction->setCurrentIndex(m_direction->findData(type->direction()));
    m_visible->setChecked(type->style()->isVisible());
    m_propertyNamesVisible->setChecked(type->style()->isPropertyNamesVisible());
    m_properties->setType(type);

    // set initial color and tooltip
    validateIdInput();
}

void EdgeTypeProperties::apply()
{
    m_type->setName(m_name->text());
    // note: ID is valid since otherwise OK-button would be disabled
    // no need to check here
    m_type->setId(m_id->value());
    m_type->style()->setColor(m_color->color());
    m_type->setDirection(static_cast<EdgeType::Direction>(m_direction->currentData().toInt()));
    m_type->style()->setVisible(m_visible->isChecked());
    m_type->style()->setPropertyNamesVisible(m_propertyNamesVisible->isChecked());
}

void EdgeTypeProperties::validateIdInput()
{
    int valid = true;
    const auto types = m_type->document()->edgeTypes();
    for (const auto &type : types) {
        if (type != m_type && type->id() == m_id->value()) {
            valid = false;
            break;
        }
    }
    // set color
    QPalette palette = m_id->palette();
    if (valid) {
        palette.setColor(QPalette::Text, Qt::black);
        m_okButton->setEnabled(true);
        m_okButton->setToolTip(i18nc("@info:tooltip", "The selected ID for this edge type."));
    } else {
        palette.setColor(QPalette::Text, Qt::red);
        m_okButton->setEnabled(false);
        m_okButton->setToolTip(i18nc("@info:tooltip", "The selected ID is already used for another edge type, please select a different one."));
    }
    m_id->setPalette(palette);
}

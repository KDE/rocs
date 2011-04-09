#include "ConfigureDefaultProperties.h"
#include "ui_ConfigureDefaultProperties.h"
#include <settings.h>

ConfigureDefaultProperties::ConfigureDefaultProperties( QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ConfigureDefaultProperties)
{
    ui->setupUi(this);

    readConfig();
    ui->comboDefaultName->setCurrentIndex(_displayPositionName);
    ui->comboDefaultValue->setCurrentIndex(_displayPositionValue);
}

ConfigureDefaultProperties::~ConfigureDefaultProperties()
{
    delete ui;
}

void ConfigureDefaultProperties::readConfig() {
    _displayPositionName = Settings::dataNameDisplay();
    _displayPositionValue = Settings::dataValueDisplay();
}

void ConfigureDefaultProperties::saveConfig() {
    Settings::setDataNameDisplay(_displayPositionName);
    Settings::setDataValueDisplay(_displayPositionValue);
}

void ConfigureDefaultProperties::setDisplayPositionName(int position) {
    if (position<0) return;
    _displayPositionName = position;
    emit changed(true);
}

void ConfigureDefaultProperties::setDisplayPositionValue(int position) {
    if (position<0) return;
    _displayPositionValue = position;
    emit changed(true);
}
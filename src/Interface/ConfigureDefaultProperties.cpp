#include "ConfigureDefaultProperties.h"
#include "ui_ConfigureDefaultProperties.h"

ConfigureDefaultProperties::ConfigureDefaultProperties( QWidget* parent, Qt::WindowFlags f) :
    QWidget(parent),
    ui(new Ui::ConfigureDefaultProperties)
{
    ui->setupUi(this);
}

ConfigureDefaultProperties::~ConfigureDefaultProperties()
{
    delete ui;
}

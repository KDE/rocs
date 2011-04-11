#include "ConfigureDefaultProperties.h"
#include "ui_ConfigureDefaultProperties.h"
#include <settings.h>
#include "Interface/GraphVisualEditor.h"


ConfigureDefaultProperties::ConfigureDefaultProperties( QWidget* parent ) :
    QWidget(parent),
    ui(new Ui::ConfigureDefaultProperties)
{
    ui->setupUi(this);

    readConfig();
    ui->comboDefaultNode->setCurrentIndex(_displayPositionNode);
    ui->comboDefaultEdge->setCurrentIndex(_displayPositionEdge);
}

ConfigureDefaultProperties::~ConfigureDefaultProperties()
{
    delete ui;
}

void ConfigureDefaultProperties::readConfig() {
    _displayPositionNode = Settings::dataNodeDisplay();
    _displayPositionEdge = Settings::dataEdgeDisplay();
}

void ConfigureDefaultProperties::saveConfig() {
    Settings::setDataNodeDisplay(_displayPositionNode);
    Settings::setDataEdgeDisplay(_displayPositionEdge);

    GraphVisualEditor::self()->setViewStyleDataNode(_displayPositionNode);
    GraphVisualEditor::self()->setViewStyleDataEdge(_displayPositionEdge);
}

void ConfigureDefaultProperties::setDisplayPositionNode(int position) {
    if (position<0) return;
    _displayPositionNode = position;
    emit changed(true);
}

void ConfigureDefaultProperties::setDisplayPositionEdge(int position) {
    if (position<0) return;
    _displayPositionEdge = position;
    emit changed(true);
}
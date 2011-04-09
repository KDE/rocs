#ifndef CONFIGUREDEFAULTPROPERTIES_H
#define CONFIGUREDEFAULTPROPERTIES_H

#include <QWidget>
#include "ui_ConfigureDefaultProperties.h"

namespace Ui {
    class ConfigureDefaultProperties;
}

class ConfigureDefaultProperties : 
    public QWidget
// , Ui::ConfigureDefaultProperties
{
    Q_OBJECT

public:
    ConfigureDefaultProperties( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ~ConfigureDefaultProperties();

private:
    Ui::ConfigureDefaultProperties *ui;
};

#endif // CONFIGUREDEFAULTPROPERTIES_H

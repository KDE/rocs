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
    ConfigureDefaultProperties( QWidget* parent = 0 );
    ~ConfigureDefaultProperties();

public slots:
    void readConfig();
    void saveConfig();
    void setDisplayPositionName(int position);
    void setDisplayPositionValue(int position);
//     void saveSettings();
// private slots:
//     void includeChanged();

private:
    Ui::ConfigureDefaultProperties *ui;
    int _displayPositionName;
    int _displayPositionValue;

signals:
    void changed(bool);
};

#endif // CONFIGUREDEFAULTPROPERTIES_H

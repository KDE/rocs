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

        enum {
            CENTER,
            BELOW,
            ABOVE
        } DISPLAY_POSITION;
            

    public slots:
        void readConfig();
        void saveConfig();
        void setDisplayPositionNode(int position);
        void setDisplayPositionEdge(int position);

    private:
        Ui::ConfigureDefaultProperties *ui;
        int _displayPositionNode;
        int _displayPositionEdge;

    signals:
        void changed(bool);
};

#endif // CONFIGUREDEFAULTPROPERTIES_H

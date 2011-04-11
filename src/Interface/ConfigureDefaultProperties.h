#ifndef CONFIGUREDEFAULTPROPERTIES_H
#define CONFIGUREDEFAULTPROPERTIES_H

#include <QWidget>

class GraphVisualEditor;

namespace Ui {
    class ConfigureDefaultProperties;
}

class ConfigureDefaultProperties : 
    public QWidget
// , Ui::ConfigureDefaultProperties
{
    Q_OBJECT

    public:
        ConfigureDefaultProperties( QWidget* parent = 0, GraphVisualEditor* graphVisualeditor=0 );
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
        GraphVisualEditor * _graphVisualEditor;

    signals:
        void changed(bool);
};

#endif // CONFIGUREDEFAULTPROPERTIES_H

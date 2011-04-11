/* This file is part of Rocs,
    Copyright (C) 2011 by:
    Andreas Cord-Landwehr <cola@uni-paderborn.de>

    Rocs is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Rocs is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rocs; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef CONFIGUREDEFAULTPROPERTIES_H
#define CONFIGUREDEFAULTPROPERTIES_H

#include <QWidget>

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

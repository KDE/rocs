/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIGUREDEFAULTPROPERTIES_H
#define CONFIGUREDEFAULTPROPERTIES_H

#include <QWidget>

namespace Ui
{
class ConfigureDefaultProperties;
}

class ConfigureDefaultProperties : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigureDefaultProperties(QWidget* parent = 0);
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
    void setExecutionModeDebugVisible(bool visible);
    void setExecutionModeOneStepVisible(bool visible);

private:
    Ui::ConfigureDefaultProperties *ui;
    int _displayPositionNode;
    int _displayPositionEdge;
    bool _executionModeDebugVisible;
    bool _executionModeOneStepVisible;

signals:
    /**
     * Signal is emitted if values are changed, but before
     * changed values are saved.
     */
    void changed(bool);

    /**
     * Signal is emitted when value is saved.
     */
    void showExecuteModeDebugChanged(bool visible);

    /**
     * Signal is emitted when value is saved.
     */
    void showExecuteModeOneStepChanged(bool visible);
};

#endif // CONFIGUREDEFAULTPROPERTIES_H

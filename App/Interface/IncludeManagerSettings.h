/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef INCLUDEMANAGERSETTINGS_H
#define INCLUDEMANAGERSETTINGS_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <kurlrequester.h>

class IncludeManagerSettings : public QWidget
{
    Q_OBJECT
public:

    explicit IncludeManagerSettings(QWidget* parent = 0, Qt::WindowFlags f = 0);

    virtual ~IncludeManagerSettings();

public slots:
    void readConfig();
    void saveSettings();
private slots:
    void includeChanged();
    void insertUrl();
//     void editItem(QListWidgetItem*);
    /** Remove the selected path (url) from list of places where Rocs will search for plugins. */
    void removeURL();
private:
    KUrlRequester* _url;
    QListWidget* _list_View;
signals:
    void changed(bool);
};

#endif // INCLUDEMANAGERSETTINGS_H

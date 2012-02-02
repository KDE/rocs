/*
    This file is part of Rocs.
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>

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


#include "PossibleIncludes.h"
#include <QGridLayout>
#include <QListWidget>
#include <KGlobal>
#include <settings.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <QDir>

PossibleIncludes::PossibleIncludes(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  QGridLayout * lay = new QGridLayout(this);
  m_list = new QListWidget(this);

  lay->addWidget(m_list);
  setLayout(lay);
  updateIncludeList();
  resize(450,200);
}



void PossibleIncludes::updateIncludeList()
{
    KGlobal::dirs()->findDirs("appdata", "examples");

    QStringList list = KGlobal::dirs()->findDirs("appdata", "examples") + Settings::includePath();

    list.removeDuplicates();
    foreach (const QString &str, list){
        QDir dir (str);
        foreach(const QString &file, dir.entryList(QStringList() << "*.js")){
          m_list->addItem(dir.absoluteFilePath(file));
        }
    }
}


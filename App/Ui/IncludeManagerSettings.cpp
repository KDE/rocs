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

#include "IncludeManagerSettings.h"
#include <QGridLayout>
#include <settings.h>
#include <QListView>
#include <KIconButton>
#include <kurlrequester.h>
#include <KPushButton>
#include <QIcon>

IncludeManagerSettings::IncludeManagerSettings(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{

    _list_View = new QListWidget(this);
    QGridLayout * lay = new QGridLayout(this);
    _url = new KUrlRequester(this);
    _url->setMode(KFile::Directory | KFile::LocalOnly | KFile::ExistingOnly);;
    KPushButton * add = new KPushButton(QIcon("list-add"), QString(), this);
    KPushButton * del = new KPushButton(QIcon("list-remove"), QString(),  this);

    lay->addWidget(_url, 0, 0);
    lay->addWidget(_list_View, 1, 0);
    lay->addWidget(add, 0, 1);
    lay->addWidget(del, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    add->setDefault(true);
    this->setLayout(lay);
    readConfig();

//     editing = 0;
    connect(add, SIGNAL(clicked(bool)), this, SLOT(insertUrl()));
    connect(del, SIGNAL(clicked(bool)), this, SLOT(removeURL()));
//     connect(_list_View, SIGNAL(currentTextChanged(QString)), this, SLOT(includeChanged()));
//     connect(_list_View, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(editItem(QListWidgetItem*)));

}


IncludeManagerSettings::~IncludeManagerSettings()
{

}

// void IncludeManagerSettings::editItem(QListWidgetItem* item )
// {
//   _list_View->editItem(item);
//
// //     editing = item;
// //     _url->setText(item->text());
// }


void IncludeManagerSettings::readConfig()
{

    QStringList list(Settings::includePath());
    for (int i = 0 ; i < list.count(); ++i) {
        QListWidgetItem * item = new QListWidgetItem(list.at(i), _list_View);
        _list_View->addItem(item);
    }
}
void IncludeManagerSettings::saveSettings()
{
    QStringList list;
    for (int i = 0 ; i < _list_View->count(); ++i) {
        list.append(_list_View->item(i)->text());
    }

    Settings::setIncludePath(list);
}

void IncludeManagerSettings::insertUrl()
{
    if (_url->text().isEmpty()) {
//         if (editing != 0){
//           _list_View->takeItem(_list_View->row(editing));
//           delete editing;
//           editing = 0;
//         }
        return;
    }

    QString text = _url->text().startsWith(QDir::rootPath())
                   ? _url->text()
                   : QDir::homePath() + '/' + _url->text();

    if (!text.endsWith('/')) {
        text.append('/');
    }

    if (_list_View->findItems(text, Qt::MatchExactly).count() != 0) {
        return; // Already have this item.
    }

    QListWidgetItem * item = new QListWidgetItem(text, _list_View);
//     item->setFlags(Qt::ItemIsEditable);
    _list_View->addItem(item);
    _url->clear();
    includeChanged();

}

void IncludeManagerSettings::removeURL()
{
    QListWidgetItem * itm = _list_View->takeItem(_list_View->currentRow());
    delete itm;
    includeChanged();

}


void IncludeManagerSettings::includeChanged()
{

    emit changed(true);
}

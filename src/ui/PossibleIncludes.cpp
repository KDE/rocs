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
#include <settings.h>

#include <QListWidget>
#include <KLocalizedString>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <KGuiItem>
#include <KStandardGuiItem>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>

PossibleIncludes::PossibleIncludes(QWidget* parent, Qt::WindowFlags f):
    QDialog(parent, f)
{
    setWindowTitle(i18nc("@title:window", "Possible Includes for Script Engine"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    m_list = new QListWidget(this);
    updateIncludeList();
    mainLayout->addWidget(m_list);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QPushButton *closeButton = new QPushButton;
    KGuiItem::assign(closeButton, KStandardGuiItem::close());
    closeButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    buttons->addButton(closeButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttons);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
}


void PossibleIncludes::updateIncludeList()
{
    QStringList dirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "examples", QStandardPaths::LocateDirectory);
    QStringList list = dirs + Settings::includePath();

    list.removeDuplicates();
    foreach(const QString & str, list) {
        QDir dir(str);
        foreach(const QString &file, dir.entryList(QStringList() << "*.js")) {
            m_list->addItem(dir.absoluteFilePath(file));
        }
    }
}


/*
    This file is part of Rocs.
    Copyright 2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef PARAMETER_H
#define PARAMETER_H

#include <QObject>
#include <QList>
#include <QStringList>

class Parameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString info READ info)
    Q_PROPERTY(QString typeLink READ typeLink)

public:
    explicit Parameter(QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);
    QString type() const;
    void setType(const QString &type);
    QString info() const;
    void setInfo(const QString &info);
    QString typeLink() const;
    void setTypeLink(const QString &link);

private:
    Q_DISABLE_COPY(Parameter)
    QString _name;
    QString _type;
    QString _info;
    QString _typeLink;
};

#endif

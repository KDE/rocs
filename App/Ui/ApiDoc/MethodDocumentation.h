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

#ifndef METHOD_DOCUMENTATION_H
#define METHOD_DOCUMENTATION_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <KUrl>


class MethodDocumentation : public QObject
{
    Q_OBJECT

public:
    typedef struct {
        QString name;
        QString type;
        QString info;
    } Parameter;

    explicit MethodDocumentation(QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);
    QStringList description() const;
    void setDescription(const QStringList &description);
    QString returnType() const;
    void setReturnType(const QString &type);
    QList<Parameter> parameters() const;
    void addParameter(const QString &name, const QString &type, const QString &info);

private:
    Q_DISABLE_COPY(MethodDocumentation)
    QString _name;
    QStringList _description;
    QString _returnType;
    QList<Parameter> _parameters;
};

#endif // PROPERTY_DOCUMENTATION_H

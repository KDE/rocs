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
#include <QVariantList>

class ParameterDocumentation;

class MethodDocumentation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QVariantList description READ description)
    Q_PROPERTY(QVariant parameters READ parametersVar)
    Q_PROPERTY(QString returnType READ returnType)
    Q_PROPERTY(QString returnTypeLink READ returnTypeLink)
    Q_PROPERTY(QString documentAnchor READ apiDocumentAnchor)

public:
    explicit MethodDocumentation(QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);
    QVariantList description() const;
    void setDescription(const QStringList &description);
    QString returnType() const;
    void setReturnType(const QString &type);
    QVariant parametersVar() const;
    QList<ParameterDocumentation*> parameters() const;
    void addParameter(const QString &name, const QString &type, const QString &info, const QString &typeLink);
    QString apiDocumentAnchor();
    QString returnTypeLink() const;
    void setReturnTypeLink(const QString &link);

private:
    Q_DISABLE_COPY(MethodDocumentation)
    QString _name;
    QStringList _description;
    QString _returnType;
    QString _returnTypeLink;
    QList<ParameterDocumentation*> _parameters;
};

Q_DECLARE_METATYPE(QList<ParameterDocumentation*>);
Q_DECLARE_METATYPE(ParameterDocumentation*);

#endif // METHOD_DOCUMENTATION_H

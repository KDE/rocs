/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef PARAMETER_H
#define PARAMETER_H

#include <QList>
#include <QObject>
#include <QStringList>

class Parameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString info READ info)
    Q_PROPERTY(QString typeLink READ typeLink)

public:
    explicit Parameter(QObject *parent = nullptr);

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
    QString m_name;
    QString m_type;
    QString m_info;
    QString m_typeLink;
};

#endif

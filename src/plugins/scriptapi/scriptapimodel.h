/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef SCRIPTAPIMODEL_H
#define SCRIPTAPIMODEL_H

#include <QAbstractItemModel>

class Object;
class Item;

class ScriptApiModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles { TitleRole = Qt::UserRole + 1, DocumentRole, AnchorRole };

    explicit ScriptApiModel(QList<Object *> dataList, QObject *parent = nullptr);
    ~ScriptApiModel() override;

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    void setupModelData(QList<Object *> dataList, Item *parent);
    Item *rootItem;
};

#endif

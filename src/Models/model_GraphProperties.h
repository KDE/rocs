#ifndef MODEL_GRAPHPROPERTIES_H
#define MODEL_GRAPHPROPERTIES_H

#include <QObject>
#include <QAbstractTableModel>
#include <QMetaObject>
#include <QModelIndex>
#include <QVariant>
#include <QMetaProperty>

class GraphPropertiesModel : public QAbstractTableModel
{
  public:
    explicit GraphPropertiesModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setDataSource(QObject *dataSource);
  
  private:
    QObject *_dataSource;
    const QMetaObject *_metaObject;
};

#endif
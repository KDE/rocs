#ifndef MODEL_GRAPHLAYERS_H
#define MODEL_GRAPHLAYERS_H

#include <QAbstractListModel>

class QModelIndex;

namespace libgraph{
  class GraphDocument;
}

class GraphLayersModel : public QAbstractListModel
{
  Q_OBJECT
  public:
      GraphLayersModel(libgraph::GraphDocument *document, QObject *parent = 0);
      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      QVariant data(const QModelIndex &index, int role) const;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags(const QModelIndex& index) const;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

      bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
      bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

  private:
    libgraph::GraphDocument *_document;
  
};

#endif
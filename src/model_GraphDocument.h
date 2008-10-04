#ifndef MODEL_GRAPHDOCUMENT_H
#define MODEL_GRAPHDOCUMENT_H

#include <QAbstractListModel>
#include <QList>

class QModelIndex;

namespace libgraph{
  class GraphDocument;
}

class GraphDocumentModel : public QAbstractListModel
{
  Q_OBJECT
  public:
      GraphDocumentModel(QList<libgraph::GraphDocument*>* documents, QObject *parent = 0);
      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      QVariant data(const QModelIndex &index, int role) const;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags(const QModelIndex& index) const;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

      bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
      bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

      libgraph::GraphDocument *at(const QModelIndex& index);
  private:
    QList<libgraph::GraphDocument*>& _documents;
  
};

#endif
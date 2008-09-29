#ifndef GRAPHCOLLECTION_H
#define GRAPHCOLLECTION_H

#include <QList>
#include <QObject>

class QString;

namespace libgraph
{

class Graph;

class GraphDocument : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(qreal width READ width WRITE setWidth)
  Q_PROPERTY(qreal height READ height WRITE setHeight)

public:
  /*! Some Simple Enum */
  enum{ simple, multi, digraph };

  /*! Default Constructor */
  GraphDocument();
  
  /*! Default Destructor */
  ~GraphDocument();

  /*! Sets the current file name of the Graph Collection */
  void setName(const QString& name);
  
  /*! Gets the Name of the Collection of Graphs */
  QString name() const;

  /*! set the height of the working area */
  void setHeight(qreal height);

  /*! gets the heigth of the working area */
  qreal height() const;

  /*! set the width of the working area */
  void setWidth(qreal width);

  /*! get the width of the working area */
  qreal width() const;

  /*! Gets the size of the graph Collection */
  int size(); 

  /*! Gets the Graph at position 'i' on the _graphs list */
  Graph* operator[](int i); 

  /*! Append a grpah on the graph's list */
  void operator<<(Graph *g);

  /*! Append a graph on the Graph's list */
  void append(Graph *g);

  /*! Prepend a graph on the Graph's list */
  void prepend(Graph *g);

  /*! Creates a new Graph */
  Graph *addGraph(int type);
  
  /*! Removes the graph at position 'i' */
  void removeAt(int i = 0); 

  /*! Remove the first instance of the graph g */
  void removeOne(Graph *g);

  /*! returns the index of the graph or -1 if not found */
  int indexOf(Graph *g);

  /*! destroy all the graphs */
  void clear();

signals:
  void graphCreated(Graph *g);
  void graphRemoved(int i);
  void nameChanged(QString name);
  void heightChanged(qreal height);
  void widthChanged(qreal width);

private:
  QList<Graph*> _graphs;
  QString _name;
  qreal _width;
  qreal _height;
};

};
#endif

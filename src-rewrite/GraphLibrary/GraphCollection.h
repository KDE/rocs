#ifndef GRAPHCOLLECTION_H
#define GRAPHCOLLECTION_H

#include <QList>
#include <QObject>

class Graph;
class QString;

class GraphCollection : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName)

public:
  /*! Some Simple Enum */
  enum{ simple, multi, digraph };

  /*! Default Constructor */
  GraphCollection();
  
  /*! Default Destructor */
  ~GraphCollection();

  /*! Sets the current file name of the Graph Collection */
  void setName(const QString& name);
  
  /*! Gets the Name of the Collection of Graphs */
  QString name() const;

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

  /*! destroy all the graphs */
  void clear();

signals:
  void graphCreated(Graph *g);
  void graphRemoved(int i);
  void nameChanged(QString name);

private:
  QList<Graph*> _graphs;
  QString _name;
};

#endif

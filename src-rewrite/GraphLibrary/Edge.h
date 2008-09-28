#ifndef EDGE_H
#define EDGE_h

#include <QObject>
#include <QColor>

class Node;
class Graph;

class Edge : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool visited READ visited WRITE setVisited RESET unsetVisited)
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(qreal length READ length WRITE setLength)
  Q_PROPERTY(QColor color READ color WRITE setColor)


  public:
    Edge(Node *from, Node *to, Graph *parent);
    ~Edge();

    bool visited() const;
    void setVisited(bool);
    void unsetVisited();

    QString name() const;
    void setName(const QString& name);
    
    qreal length() const;
    void setLength(const qreal& length);

    QColor color() const;
    void setColor(QColor color);

    Node* from() const;
    Node* to() const;
    
    void connectNodes();
    void disconnectNodes();

  signals:
    void nameChanged(QString name);
    void lengthChanged(qreal length);
    void visitedChanged(bool visited);
    void colorChanged(QColor color);

  private:
    Node* _from;
    Node* _to;
    Node* _from_disconnected;
    Node* _to_disconnected;
    bool _visited;
    QString _name;
    qreal _length;
    QColor _color;

};
#endif

#ifndef GRAPHGROUPS_H
#define GRAPHGROUPS_H

#include <QObject>
#include <QList>

class Node;

class GraphGroup : public QObject, public QList< ::Node*>{
Q_OBJECT

public:
	GraphGroup();
	GraphGroup(const GraphGroup& g);

public slots:
	QString name() const;
	void setName(const QString&  name);

private:
	QString _name;
};
#endif
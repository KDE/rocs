#include "graphGroups.h"
#include "node.h"

GraphGroup::GraphGroup() : QObject(0), QList< ::Node*>(){}

GraphGroup::GraphGroup(const GraphGroup& g) : QObject(0), QList< ::Node*>(){
	foreach( ::Node *n, g){
		append(n);
	}
	_name = g.name();
}
QString GraphGroup::name() const{ return _name; }

void GraphGroup::setName(const QString& name)  { _name = name; }
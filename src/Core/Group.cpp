 
#include "Group.h"

Group::Group(QObject *parent) : QObject(parent) {}
void Group::setName(const QString& name){ Q_UNUSED(name);}
const QString& Group::name() const{ return _name; }
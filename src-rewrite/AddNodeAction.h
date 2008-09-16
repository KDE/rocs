#ifndef ADDNODEACTION_H
#define ADDNODEACTION_H

#include <KAction>
#include <QObject>

class addNodeAction : public KAction{
public:
	addNodeAction(QObject *parent);
};

#endif
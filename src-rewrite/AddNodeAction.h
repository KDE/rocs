#ifndef ADDNODEACTION_H
#define ADDNODEACTION_H

#include "AbstractAction.h"
#include <QObject>

class AddNodeAction : public AbstractAction{
public:
	AddNodeAction(QObject *parent = 0);
	void changeGraphScene(GraphScene *graphScene);
};

#endif
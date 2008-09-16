#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <KAction>
#include <QObject>

class GraphScene;

class AbstractAction : public KAction
{
	public: 
		AbstractAction(QObject *parent) : KAction(parent){}
		virtual void changeGraphScene(GraphScene *graphScene) = 0;
	protected:
		GraphScene *_graphScene;

};

#endif
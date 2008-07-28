#ifndef GROUP_H
#define GROUP_H

#include <QList>
#include <QRectF>
#include <QGraphicsItem>
#include "DiagramNode.h"
class DiagramScene;

class Group : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	private:
		DiagramScene *diagramScene;
		QRectF *box;
		QList<Node*> nodeList;	
	public:
		Group(DiagramScene *scene, QList<Node*> nodeList);
		~Group();
		void rePaint();
		void removeNode(Node *n);
	
	public slots:
	    QScriptValue nodes();
	};

#endif

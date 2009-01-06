#include "graphDocumentHandler.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "graphGroups.h"
#include <KSaveFile>
#include <QDebug>
#include <QFile>

GraphDocumentHandler::GraphDocumentHandler(){}

bool GraphDocumentHandler::saveAsInternalFormat(QList<Graph*> graph, const QString& filename){
	buf.clear();
	KSaveFile saveFile;
	saveFile.setFileName( filename );
	qDebug() << filename;

 	if (!saveFile.open()){
 		qDebug() << "Error: File Not Open";
 		return false;
 	}
	
	QTextStream stream(&saveFile);
	
	foreach(Graph *g, graph){
		buf += " \n \n ############ Graph  ########### \n \n";
		buf += "[Graph ";
		buf += g->property("name").toString();
		buf += "] \n";

		savePropertiesInternalFormat(g);

		buf += " \n \n ############ NODES ########### \n \n";
		foreach( Node *n, g->nodes()){
			buf += "[Node ";
			buf += n->property("name").toString();
			buf += "] \n";
			
			savePropertiesInternalFormat(n);
		}

		buf += " \n \n ############ EDGES ########### \n \n";
		foreach( Edge *e, g->edges()){
			buf += QString("[Edge ");
			buf += e->from()->property("name").toString();
			buf += "->";
			buf += e->to()->property("name").toString();
			buf += QString("] \n");
			savePropertiesInternalFormat(e);
		}

		buf += " \n \n ############ GROUPS ########### \n \n";
		foreach( GraphGroup *gg, g->groups()){
			buf += "[Group ";
			buf += gg->name();
			buf += "] \n";

			foreach( Node *n, g->nodes() ){
				buf += n->property("name").toString();
				buf += "\n";
			}
		}
	}
	qDebug() << buf.toAscii();

	stream << buf.toAscii();
	if (!saveFile.finalize()){
		qDebug() << "Error, file not saved.";
	}
	return true;
}

void GraphDocumentHandler::savePropertiesInternalFormat(QObject *o){
	const QMetaObject *metaObject = o->metaObject();
	int propertyCount = metaObject->propertyCount();

	for( int i = 0; i < propertyCount; ++i){
		QMetaProperty metaProperty = metaObject->property(i);
		const char *name = metaProperty.name();

		if ( QString("objectName") == QString(name) ){
			continue;
		}

		QVariant value = o->property(name);
		buf +=  name;
		buf += " : ";
		buf += value.toString();
		buf += "\n";
	}
}

QList<Graph*> GraphDocumentHandler::loadFromInternalFormat(const QString& filename){
	QList<Graph*> g;
	
	QFile f(filename);
	if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ){
		qDebug  () << "File not open " << filename.toAscii();
		return g;
	}
	
	Graph *tmpGraph = 0;
	Node *tmpNode  = 0;
	Edge *tmpEdge = 0;
	GraphGroup *tmpGroup = 0;
	QObject *tmpObject = 0;


	while(!f.atEnd()){
		QString str = f.readLine();
		str = str.simplified();
	
		if (str.startsWith("#")){ //! Ignore it, commented line.
				continue;
		}

		else if (str.startsWith("[Graph")){
			QString gName = str.section(" ",1,1);
			gName.remove(']');
	
			tmpGraph = new Graph();
			tmpGraph->setProperty("name", gName.toAscii());
			tmpObject = tmpGraph;
			g.append(tmpGraph);
		}

		else if (str.startsWith("[Node")){
			QString nName = str.section(" ",1,1);
			nName.remove(']');

			tmpNode = tmpGraph->addNode(nName);
			tmpNode->setProperty("name", nName);
			tmpObject = tmpNode;
		}

		else if (str.startsWith("[Edge")){
			QString eName = str.section(" ",1,1);
			eName.remove(']');
			
			QString nameFrom = eName.section("->", 0,0);
			QString nameTo = eName.section("->", 1,1);

			tmpEdge = tmpGraph->addEdge(tmpGraph->node(nameFrom), tmpGraph->node(nameTo));
			tmpObject = tmpEdge;

		}
		else if (str.startsWith("[Group")){
			QString gName = str.section(" ",1,1);
			gName.remove(']');
			tmpGroup = tmpGraph->addGroup(gName);
		}
		else if (str.contains(":")){
			QString propertyName = str.section(":",0,0);
			QString propertyValue = str.section(":",1,1);
			tmpObject->setProperty( propertyName.toAscii() , propertyValue.toAscii() );
		}
		else{
			tmpGroup->append( tmpGraph->node(str));
		}
	}

	return g;
}
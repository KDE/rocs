#ifndef GRAPHDOCUMENTHANDLER_H
#define GRAPHDOCUMENTHANDLER_H

#include <QList>
#include <QString>
#include "graph.h"

class Graph;
class QObject;

class GraphDocumentHandler{
	private:
		QString buf;
		void  savePropertiesInternalFormat(QObject *o);

public:
	GraphDocumentHandler();
	bool saveAsInternalFormat(QList<Graph*> graph, const QString& filename);
  QList<Graph*> loadFromInternalFormat(const QString& filename);
	
};

#endif
#ifndef GRAPHDOCUMENTHANDLER_H
#define GRAPHDOCUMENTHANDLER_H

#include <QList>
#include <QString>
#include "graph.h"
#include "graphDocument.h"

class Graph;

class QObject;

class GraphDocumentHandler{
	private:
		QString buf;
		void  savePropertiesInternalFormat(QObject *o);

public:
	GraphDocumentHandler();
	bool saveAsInternalFormat(const GraphDocument& graph, const QString& filename);
	GraphDocument loadFromInternalFormat(const QString& filename);
	
};

#endif
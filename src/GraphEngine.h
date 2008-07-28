#ifndef GRAPH_ENGINE_H
#define GRAPH_ENGINE_H

#include <QtScript>

class DiagramScene;

class GraphEngine : public QScriptEngine
{
public:
	GraphEngine(DiagramScene *scene);
	void createNodeList();
	void createEdgeList();
	void createGroupsList();
	void createSelectedNodesList();
	void createSelectedEdgesList();
	void loadDefaultFile();
private:
	DiagramScene *scene;

};
#endif

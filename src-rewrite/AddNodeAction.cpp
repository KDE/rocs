#include "AddNodeAction.h"
#include <KLocale>
#include "UI_GraphScene.h"

AddNodeAction::AddNodeAction(QObject *parent) : AbstractAction(parent)
{
	_graphScene = NULL;
	setText(i18n ( "Add Node" ));
	setToolTip ( i18n ( "Creates a new node at the click position on the drawing area." ) );
	setIcon ( KIcon ( "pointer" ) );
	setCheckable ( true );
	setChecked ( false );
	setProperty ( "rocs_action", GraphScene::AddNode );
}

void AddNodeAction::changeGraphScene(GraphScene* graphScene)
{
	if (_graphScene != NULL)
	{
		disconnect(this, 0, _graphScene, 0);
	}

	_graphScene = graphScene;
	connect(this, SIGNAL(triggered()), _graphScene, SLOT(beginAddNode()));
}
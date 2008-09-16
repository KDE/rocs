#include "AddNodeAction.h"
#include <KLocale>
#include "UI_GraphScene.h"

addNodeAction::addNodeAction(QObject *parent) : KAction(parent){
	setText(i18n ( "Add Node" ));
	setToolTip ( i18n ( "Creates a new node at the click position on the drawing area." ) );
	setIcon ( KIcon ( "pointer" ) );
	setCheckable ( true );
	setChecked ( false );
	setProperty ( "rocs_action", GraphScene::AddNode );
}

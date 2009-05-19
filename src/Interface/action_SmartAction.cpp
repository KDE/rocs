#include "action_SmartAction.h"
#include "action_AddEdge.h"
#include "action_AddNode.h"
#include "action_MoveNode.h"
#include "action_Select.h"

#include <KAction>
#include <KLocale>
SmartAction::SmartAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Smart Tool" ));
    setToolTip ( i18n ( "Tries to do what it thinks that's good for the soul." ) );
    setIcon ( KIcon ( "smart-tool" ) );
}

void SmartAction::executePress(QPointF) {

}

void SmartAction::executeMove(QPointF) {

}

void SmartAction::executeRelease(QPointF) {

}

void SmartAction::executeKeyPress(QKeyEvent *keyEvent) {

}

void SmartAction::executeKeyRelease(QKeyEvent *keyEvent) {

}

void SmartAction::setAddEdgeAction(KAction *e) {
    _addEdgeAction = qobject_cast<AddEdgeAction*>(e);
}

void SmartAction::setAddNodeAction(KAction *e) {
    _addNodeAction = qobject_cast<AddNodeAction*>(e);
}

void SmartAction::setMoveAction(KAction *e) {
    _moveAction = qobject_cast<MoveNodeAction*>(e);
}

void SmartAction::setSingleSelectAction(KAction *e) {
    _SelectAction = qobject_cast<SelectAction*>(e);
}

#ifndef ACTION_SMARTACTION_H
#define ACTION_SMARTACTION_H

#include "action_AbstractAction.h"
#include <QObject>
class AddEdgeAction;
class AddNodeAction;
class MoveNodeAction;
class SingleSelectAction;
class KAction;

class SmartAction : public AbstractAction {
public:
    explicit SmartAction( GraphScene *scene, QObject *parent = 0);
    void setAddEdgeAction(KAction *e);
    void setAddNodeAction(KAction *e);
    void setMoveAction(KAction *e);
    void setSingleSelectAction(KAction *e);

public slots:
    virtual void executePress(QPointF pos);
    virtual void executeMove(QPointF pos);
    virtual void executeRelease(QPointF pos);
    virtual void executeKeyPress(QKeyEvent *keyEvent);
    virtual void executeKeyRelease(QKeyEvent *keyEvent);

private:
    AddNodeAction *_addNodeAction;
    AddEdgeAction *_addEdgeAction;
    SingleSelectAction *_singleSelectAction;
    MoveNodeAction *_moveAction;
};

#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QtScript>
#include <QScriptEngine>
#include "ui_mainwindow.h"
#include "DiagramScene.h"
#include "DiagramEdge.h"
#include "DiagramNode.h"

class QMainWindow;
class QScriptEngine;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT
  private:
    DiagramScene *scene;
    void runScript();
    void createActions();
  
  /**************************************************************
    * JavaScript Engine. CHANGE IT TO KROSS AS SOON AS POSSIBLE!  *
    ***************************************************************/

  private slots:
  
  //! Botões da barra de ferramentas
  void on_btnAddNode_clicked()      { scene -> action = scene -> InsertNode;       }
  void on_btnAddEdge_clicked()      { scene -> action = scene -> InsertEdge;       }
  void on_btnSelection_clicked()    { scene -> action = scene -> SelectItem;       }
  void on_btnSquareSelect_clicked() { scene -> action = scene -> SquareSelect;     }
  void on_btnMove_clicked()         { scene -> action = scene -> MoveItem;       }
  void on_btnAddEdgeArrow1_clicked(){ scene -> action = scene -> InsertEdgeArrow1; }
  void on_btnAddEdgeArrow2_clicked(){ scene -> action = scene -> InsertEdgeArrow2; }
  
  void on_btnGroup_clicked()           { scene -> group();           }
  void on_btnUngroup_clicked()         { scene -> ungroup();         }
  void on_btnRemoveFromGroup_clicked() { scene -> removeFromGroup(); }
  void on_btnRemove_clicked()          { scene -> removeSelection(); }


  void on_btnRunScript_clicked() { runScript();  }

  /* #################
     #! Node Control #
     #################*/
  
  // Changes the beginning and end of the graph
  void on_btnStartNode_clicked()    { scene -> setStartNode();   }
  void on_btnEndNode_clicked()      { scene -> setEndNode();     }
  
  // Allignment Options
  void on_btnAllignVLeft_clicked()  { scene -> allignVLeft();           }
  void on_btnAllignVRight_clicked() { scene -> allignVRight();          }
  void on_btnAllignVCenter_clicked(){ scene -> allignVCenter();         }
  void on_btnAllignHTop_clicked()   { scene -> allignHTop();            }  
  void on_btnAllignHBottom_clicked(){ scene -> allignHBottom();         }
  void on_btnAllignHCenter_clicked(){ scene -> allignHCenter();         }
  void on_btnNodeIndex_clicked()    { scene -> toogleIndexVisibility();  }
  void on_btnNodeColor_clicked();
    
  public:
   
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void connectEdgeSignals   (Edge *e);
  void disconnectEdgeSignals(Edge *e);
  void connectNodeSignals   (Node *d);
  void disconnectNodeSignals(Node *d);
    
  void showNodeControls(bool show);
  void showNodeAllign(bool show)  ;
  void showEdgeControls(bool show);
  void resetSpinEdgeLength(bool showSpin, qreal value);

  void debug(const QString& s);
  
};

#endif



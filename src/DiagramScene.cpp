/*!




**************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>

#include <QtScript>
#include "DiagramScene.h"
#include "DiagramNode.h"
#include "DiagramEdge.h"
#include "MainWindow.h"
#include "GraphEngine.h"

DiagramScene::DiagramScene( QObject *parent ) : QGraphicsScene( parent )
{
  /*!Default constructor, just initializes some basic stuff. 
     tmpItem is the temporary holder for drawings like the line when inserting an edge,
   and the Square while selecting.
   startNode is the currently node set as `start`
     same for endNode. */
  mainWindow = qobject_cast< MainWindow* >( parent );
  bottom = 768;
  right = 1024;
  setSceneRect( 0, 0, right, bottom );
  tmpItem   = 0;
  startNode = 0;
  engine    = 0;
  endNode   = 0;
}

DiagramScene::~DiagramScene( )
{
  /*!Default Destructor. 
  clears all the arrays, 
  groups, nodes, edges, selectedNodes, selectedEdges.
  then removes all nodes, getting all edges to go with them.
  ( edges are removed on nodes destructors. )
  */
  selectedNodes.clear( );
  selectedEdges.clear( );
  
  foreach( Group *g, groups )
  {
    delete g;

    g = 0;
  }
  
  foreach( Node *n, nodes )
  {
    delete n;
    n = 0;
  }
  
  nodes.clear( );
  edges.clear( );
  groups.clear( );
}

void DiagramScene::group( )
{
  /*!gets the currently selected nodes and creates a group around them,
  then add the group to the item list to the scene, so it can be drawn. 
  and appends it on the groups list for the scripting interface.
  */
  if( selectedNodes.size( ) != 0 )
  {
    Group *g = new Group( this, selectedNodes );
    addItem( g );  
    groups.append( g );
  }
}

void DiagramScene::ungroup( )
{
  /*!if the currently first item on the selection has a group, 
  this group will be disbanded. */
  if (groups.size() == 0) return;
  if (selectedNodes.size() == 0) return;
  
  Node* n = selectedNodes.first();
  Group *g = n->getGroup();
  delete g;
}

void DiagramScene::removeFromGroup(){
  /*!removes the currently selected node from the group, preserving it.
  if the group size is 0, it will me removed too. */
}
void DiagramScene::mousePressEvent( QGraphicsSceneMouseEvent *mouseEvent )
{
  /*!when choosing an option on MainWindow toolbar, `action` will be changed, 
     this method is invocked when you click on the scene. 
     it will choose an option based on the currently action set by mainWindow,
     then pass to the same method on the super class. 
 */  
  if ( mouseEvent -> button( ) != Qt::LeftButton ) return;
  
  QPointF pos = mouseEvent  ->  scenePos( );
  switch ( action )
  {
    case InsertNode       : insertNode( pos );           break;
    case InsertEdge       : beginInsertEdge( pos );      break;
    case InsertEdgeArrow1 : beginInsertEdge( pos, 1 );   break;
    case InsertEdgeArrow2 : beginInsertEdge( pos, 2 );   break;
    case SelectItem       : selectItem( pos );           break;
    case SquareSelect     : beginSquareSelect( pos );    break;
    case InsertKGraph     : createKGraph(pos);           break;
    case InsertCGraph     : createCGraph(pos);           break;
  }
  QGraphicsScene::mousePressEvent( mouseEvent );
}

void DiagramScene::insertNode( QPointF pos )
{
  /*!inserts a new node on position pos. 
   then adds it to the scene so it can be draw.
  and append it on the nodes list for the scripting interface 
  */
  Node *node = new Node( this );
  node  ->  setPos( pos );
  addItem( node );
  nodes.append( node );
  node -> index = nodes.size()-1;
}

QScriptValue DiagramScene::insertNode( double x1, double y1 )
{
  /*!inserts a new node on position pos. 
   then adds it to the scene so it can be draw.
  and append it on the nodes list for the scripting interface 
  
  this method is mainly for use inside the script interface, 
  and that`s why it returns a ScriptValue. 
  */
  Node *node = new Node( this );
  node  ->  setPos( x1, y1 );
  addItem( node );
  nodes.append( node );
  node -> index = nodes.size()-1;
  if ( !engine ) return QScriptValue( );
  
  return  engine -> newQObject( node );
}

void DiagramScene::beginInsertEdge( QPointF pos, int arrowType )
{
  /*!this is the first click after pressing any 'insert edge' buttom.
   it will draw a line between the clicked point and the current mouse position.
  when the mouse has a release button event, it will try to insert the edge.
  
  this will initialize tmpItem, since the line that we are drawing is temporary, 
  and will only be there till we release the mouse, changed after that for a 
  proper edge.
  */
  
  this -> arrowType = arrowType; // 0 = NoArrow, 1 = singleArrow, 2 = doubleArrow.
  QGraphicsLineItem *i = new QGraphicsLineItem( QLineF( pos,pos ) );
  i  ->  setPen( QPen( Qt::black, 1 ) ); //! if this is not set, it will draw a white line.
  tmpItem = i;
  addItem( tmpItem );
}

void DiagramScene::beginSquareSelect( QPointF pos )
{
  /*!this is the first click after pressing the 'square select' buttom.
   it will draw a square with 0 size, and them the mouseMoveEvent will
  handle the rest. 
  initializes the tmpItem so to draw the selection square. */
  
   QGraphicsRectItem *i = new QGraphicsRectItem( 0, 0, 0, 0 );
   i -> setPos( pos );
   i  ->  setPen( QPen( Qt::black, 1 ) );
   tmpItem = i;
   addItem( tmpItem );
}

void DiagramScene::endSquareSelect( )
{
  /*!the selection is done, the mouse is released, it will try to get
   any items inside the square area and insert it into the selection list. */
  
  /*!gets the list and removes the first item IF the item is the selection retangle */
  QGraphicsRectItem *i = qgraphicsitem_cast<QGraphicsRectItem*>( tmpItem );
  QList<QGraphicsItem*> itemList = items( i -> sceneBoundingRect( ) );
  if ( itemList.count( ) && itemList.first( ) == i ) itemList.removeFirst( );

  /*!iterates over the list of items inside the selection 
  area and puts them inside of the selection list */
  foreach( QGraphicsItem *i, itemList )
  {
    Node *n = qgraphicsitem_cast<Node*>( i );  // converting from GraphicsItem to Node
    if ( n == 0 ) continue;           // if the item is not a node ( edge, for example ), continue.
    if ( selectedNodes.contains( n ) ) continue; // if the item is already selected,  continue.
    selectedNodes.append( n );                 
    n -> selected = true;
    mainWindow  ->  connectNodeSignals( n );     //! connect some signals to the node, those signals are the  top buttons on the mainWindow.
    n -> update( );                             //! redraw the node, now with an little retanglearound it.
  }
  
  /*!hides or shows the mainWindows Buttons*/
  toogleWindowButtons( );
}

void DiagramScene::setStartNode( )
{
  /*!Sets the currently selected node as 'start', 
   and if there`s another start, it`s lost.
   note that a node can be start and end at the same time.
 */
  if ( startNode )
  {
    startNode -> isStartPoint = false;
    startNode -> update( );
  }
  startNode = selectedNodes.first( );
    
  if ( startNode != NULL )
  {
    startNode -> isStartPoint = true;
    startNode -> update( );
  }
}
  
void DiagramScene::setEndNode( )
{
  /*!Sets the currently selected node as 'end', 
   and if there`s another start, it`s lost.
   note that a node can be start and end at the same time.
 */
  if ( endNode )
  {
    endNode -> isEndPoint = false;
    endNode -> update( );
  }
  endNode = selectedNodes.first( );

  if ( endNode != NULL )
  {
    endNode -> isEndPoint = true;
    endNode -> update( );
  }
}



void DiagramScene::mouseMoveEvent( QGraphicsSceneMouseEvent *mouseEvent ){
  /*!do something based on current action when mouse is moving */
  if ( tmpItem != 0 )
  {
    switch( action )
    {
     /*!if action is insertEdge, continue drawing the line from 
     the start position to the actual mouse position. */
       case InsertEdge: 
       case InsertEdgeArrow1:  
       case InsertEdgeArrow2:
         if ( qgraphicsitem_cast<QGraphicsLineItem*>( tmpItem ) ) 
         {
            QGraphicsLineItem *i = qgraphicsitem_cast<QGraphicsLineItem*>( tmpItem );
            QLineF newLine( i -> line( ).p1( ), mouseEvent -> scenePos( ) );
            i  ->  setLine( newLine );
         }
       break;
       /*!if the action is a square selection, continue drawing the rectangle over the points. */
       case SquareSelect:
         if ( qgraphicsitem_cast<QGraphicsRectItem*>( tmpItem ) )
         {
            QGraphicsRectItem *i = qgraphicsitem_cast<QGraphicsRectItem*>( tmpItem );
            double x2 = mouseEvent  ->  scenePos( ).x( ) - i -> pos( ).x( );
            double y2 = mouseEvent  ->  scenePos( ).y( ) - i -> pos( ).y( );
            i -> setRect( QRect( 0,0,x2,y2 ) );
         }
         break;
         /*!if is moving the item, invoke the superclass, it handles everything =D */
    }
  }
  if(action == MoveItem) QGraphicsScene::mouseMoveEvent( mouseEvent );
}

void DiagramScene::selectItem( QPointF pos )
{
  /*!single select item at the currently mouse position. 
     it`s possible to select multiple items clicking on them, 
     but only multiple nodes or multiple edges, it`s not possible 
     to select nodes AND edges currently.
  */

  /*!if the item is a node, selects the node.
     puts the node on the selected nodes list
     updates it so the little square around it is drawn. 
    
     if no node is selected, clean the selected nodes array.
  */
  QGraphicsItem *item = itemAt( pos );
  Node *node = qgraphicsitem_cast<Node*>( item );
  
  if ( node != NULL )
  {
    if ( selectedNodes.contains( node ) ) return;
    selectedNodes.append( node );
    node -> selected = true;
    mainWindow  ->  connectNodeSignals( node );
    node -> update( );
  } 
  else while( !selectedNodes.isEmpty( ) )
  {
    Node *n = selectedNodes.first( );
    n  -> selected = false;
    mainWindow  -> disconnectNodeSignals( n );
    n  ->  update( );
    selectedNodes.removeFirst( );
  }

 /*!same goes for edges, 
    and also reset the little box with the length of it, with the actual edge distance.
 */

  Edge *edge = qgraphicsitem_cast<Edge*>( item );
  if ( edge != NULL )
  {
    if ( selectedEdges.contains( edge ) ) return;
  
    selectedEdges.append( edge );
    edge -> selected = true;
    mainWindow  ->  connectEdgeSignals( edge );
    edge -> update( );
    mainWindow -> resetSpinEdgeLength( edge -> isShowLengthEnabled( ), edge -> getLength( ) );
  }
  else while( !selectedEdges.isEmpty( ) )
  {
    Edge *e = selectedEdges.first( );
    e  ->  selected = false;
    mainWindow  ->  disconnectEdgeSignals( e );
    e  ->  update( );
    selectedEdges.removeFirst( );
  }
  
 /*!shows or hide the buttons on the mainWindow */
  toogleWindowButtons( );
}

void DiagramScene::toogleWindowButtons( )
{
  /*!shows or hide the buttons on the mainwindow depending on the situation. */
  switch( selectedNodes.size( ) )
  {
    case 0:  mainWindow -> showNodeControls( false ); mainWindow -> showNodeAllign( false ); break;
    case 1:  mainWindow -> showNodeControls( true );  mainWindow -> showNodeAllign( false ); break;
    default: mainWindow -> showNodeControls( false ); mainWindow -> showNodeAllign( true );break;
  }  
  mainWindow -> showEdgeControls( selectedEdges.isEmpty( ) ? false: true );
}

void DiagramScene::removeNode( Node *n )
{
  // remove a node from the scene.
  /*!this function is called from the node 
  destructor, so no delete should be done here. */
  if ( !n ) return;
  if ( n -> hasGroup( ) )              n -> removeGroup( );
  if ( nodes.contains( n ) ){
   int pos = nodes.indexOf(n);
   nodes.removeAt( nodes.indexOf( n ) );
   int length = nodes.size();
  
  for(; pos < length; pos++)
  {
    nodes[pos]->index = pos;
    nodes[pos]->update();
  }
  }

  if ( selectedNodes.contains( n ) )  selectedNodes.removeAt( selectedNodes.indexOf( n ) );
  toogleWindowButtons();
}

void DiagramScene::removeEdge( Edge *e )
{
  //! removes the edge from the scene. 
  /*!this function is called from the edge 
      destructor, so no delete should be done here. */
    if ( edges.contains( e ) )   edges.removeAt( edges.indexOf( e ) );
    if ( selectedEdges.contains( e ) ) selectedEdges.removeAt( selectedEdges.indexOf( e ) );
    toogleWindowButtons();
}

void DiagramScene::removeGroup(Group *g)
{
  if ( groups.contains( g ) ) groups.removeAt(groups.indexOf( g ));
  toogleWindowButtons();
}

void DiagramScene::removeSelection( )
{
  //! delete everything inside of the selection. yeah baby, yeah.
  if ( selectedNodes.size( ) != 0 ) foreach( Node *n, selectedNodes ) delete n;
  if ( selectedEdges.size( ) != 0 ) foreach( Edge *e, selectedEdges ) delete e;
}

void DiagramScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *mouseEvent )
{
 /*!if there`s a tmpItem, it means that we are doing something. so, if it`s InsertEdge, finish the inserting. 
    if it`s the Square Selection, finish the selection. */
 if ( tmpItem != 0 ) switch( action )
 {
     case InsertEdge: 
     case InsertEdgeArrow1:  
     case InsertEdgeArrow2:  if  ( qgraphicsitem_cast<QGraphicsLineItem*>( tmpItem ) ) insertEdgeWithLine( );  break;
     case SquareSelect:      if  ( qgraphicsitem_cast<QGraphicsRectItem*>( tmpItem ) ) endSquareSelect( );     break;
 }
  
 /*!release the tmpItem from it`s evil holder. */
  if( tmpItem )
  {
    removeItem( tmpItem );
    delete tmpItem;
    tmpItem = 0;  
  }

  //! If you`re just released an node that`s part of a group, refresh the group drawing.
  QList<QGraphicsItem*> p_items = items( mouseEvent -> scenePos( ) );
  if ( p_items.size( ) != 0 )
  {
    Node *n = qgraphicsitem_cast<Node*>( p_items.first( ) );
    if ( n && n  ->  hasGroup( ) )  n  ->  getGroup( )  ->  rePaint( );
  }

  /*!throws the job for the superclass. */
  QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

void DiagramScene::insertEdge( Node *begin, Node *end, int arrowType )
{
  /*!Insert an edge between two nodes. */
  
  Edge *newEdge = new Edge( begin, end, arrowType, 0, false, this );
  newEdge -> setZValue( -1000 ); // puts the edge behind the node. 
  addItem( newEdge );
  edges.append( newEdge );
}

void DiagramScene::insertEdgeWithLine( )
{
  //! insert the edge between the first `pos` that was the first mouse click.
  // and the second pos, that`s the position when the mouse release the buttom.
  /*!if there`s a node on both positions, a Edge will be inserted. */

  /*!first, tryes to get the items on the first point of the line and the second.
    then removes the `this` line from the selected items (no racism, it`s just, we don`t really need a line right now. )
  */
  QGraphicsLineItem *i = qgraphicsitem_cast<QGraphicsLineItem*>( tmpItem );
    QList<QGraphicsItem*> startItems = items( i -> line( ).p1( ) );
    QList<QGraphicsItem*> endItems   = items( i -> line( ).p2( ) );
    
    if ( startItems.count( ) && startItems.first( ) == i ) startItems.removeFirst( );
    if ( endItems.count( )   && endItems.first( )   == i ) endItems.removeFirst( );
    
   /*!checks if there`s a node at the same position on both points. */
    if ( startItems.count( ) > 0 
    && endItems.count( )    > 0 
    && startItems.first( ) -> type( ) == Node::Type 
    && endItems.first( )   -> type( ) == Node::Type )
    {
      Node *firstItem  = qgraphicsitem_cast<Node*>( startItems.first( ) );
      Node *secondItem = qgraphicsitem_cast<Node*>( endItems.first( ) );
      insertEdge( firstItem, secondItem, arrowType ); //! if there`s, adds the node happly.
    }
}

/*!this goes for all allign functions.:
 gets the selected nodes, get the topWathever (top, bottom, left, right, hcenter, vcenter)
  and set the positions of each selected nodes for the new one. */
void DiagramScene::allignVLeft( )
{
  qreal topLeft = selectedNodes.first( ) -> scenePos( ).x( );
  foreach( Node *n, selectedNodes ){
    if ( topLeft > n -> scenePos( ).x( ) )   topLeft = n -> scenePos( ).x( );
  }

  foreach( Node *n, selectedNodes )   n -> setPos( topLeft, n -> scenePos( ).y( ) );
}

void DiagramScene::allignVRight( )
{
  qreal topRigth = selectedNodes.first( ) -> scenePos( ).x( );
  foreach( Node *n, selectedNodes ){
   if ( topRigth < n -> scenePos( ).x( ) ) topRigth = n -> scenePos( ).x( );
  }
  foreach( Node *n, selectedNodes )  n -> setPos( topRigth, n -> scenePos( ).y( ) );
}

void DiagramScene::allignVCenter( )
{
  qreal topRigth = selectedNodes.first( ) -> scenePos( ).x( );
  qreal topLeft = topRigth;
  foreach( Node *n, selectedNodes ){
    if ( topRigth > n -> scenePos( ).x( ) )   topRigth = n -> scenePos( ).x( );
    if( topLeft < n -> scenePos( ).x( ) )     topLeft = n -> scenePos( ).x( );    
  }
  qreal center = ( topRigth + topLeft ) / 2;
  foreach( Node *n, selectedNodes )   n -> setPos( center, n -> scenePos( ).y( ) );
}

void DiagramScene::allignHTop( )
{
  qreal topTop = selectedNodes.first( ) -> scenePos( ).y( );
  foreach( Node *n, selectedNodes )
  {
    if ( topTop > n -> scenePos( ).y( ) )  topTop = n -> scenePos( ).y( );
  }
  foreach( Node *n, selectedNodes )  n -> setPos( n -> scenePos( ).x( ), topTop );
}

void DiagramScene::allignHBottom( )
{
  qreal topBottom = selectedNodes.first( ) -> scenePos( ).y( );
  foreach( Node *n, selectedNodes ) 
  {
   if ( topBottom < n -> scenePos( ).y( ) )  topBottom = n -> scenePos( ).y( );
  }
  foreach( Node *n, selectedNodes )  n -> setPos( n -> scenePos( ).x( ), topBottom );
}

void DiagramScene::allignHCenter( )
{
  qreal topTop = selectedNodes.first( ) -> scenePos( ).y( );
  qreal topBottom = topTop;
  foreach( Node *n, selectedNodes ){
    if ( topTop    > n  ->  scenePos( ).y( ) ) topTop = n -> scenePos( ).y( );
    if ( topBottom < n  ->  scenePos( ).y( ) ) topBottom = n -> scenePos( ).y( );
  }
  qreal center = ( topTop + topBottom ) / 2;
  foreach( Node *n, selectedNodes ){
    n -> setPos( n -> scenePos( ).x( ), center );
  }
}

void DiagramScene::createScriptFunctions( const QString& program ){
  //! Create The Script engine, gets the text on the script area and runs it. 
  if ( engine != NULL ){
    delete engine;
    engine = NULL;  
  }

  engine = new GraphEngine( this );
  mainWindow -> debug( engine -> evaluate( program ).toString( ) );
  
  delete engine;
  engine = NULL;
}

//! shows or hides the index inside of the nodes.
void DiagramScene::toogleIndexVisibility(){
  foreach(Node *n, nodes){
    n->toogleIndexVisibility();
  }
}

//! creates a KGraph.
int DiagramScene::createConcentricNodes(int k, qreal radius,  QPointF pos ){
   
   //! sees if there's space on the canvas
  //! sees if there's space on the canvas
  if ((pos.x() - radius) < 0){ pos.setX(radius+20); }
  if ((pos.y() - radius) < 0){ pos.setY(radius+20); }
  if ( (pos.x() + radius) > right){ pos.setX(right - radius - 20);   }
  if ( (pos.y() + radius) > bottom){ pos.setY( bottom - radius - 20); }
  
  qreal x = pos.x();
  qreal y = pos.y();

  int beginOfGraph = nodes.size();
  for (int i = 1; i <= k; i++) {  
    const qreal angle =  i * (2*M_PI / k);    
    insertNode(QPoint(radius * cos(angle) + x, radius * sin(angle) + y)) ; 
  }
  return beginOfGraph;
}

void DiagramScene::createKGraph(QPointF pos)
{
   int k = mainWindow->spinNodes->value();
   qreal radius = mainWindow -> spinRadius->value();

   int posBegin = createConcentricNodes(k, radius, pos);
   int s = nodes.size();

   for(int i = posBegin; i < (s - 1); i++)
   {
     for(int e = posBegin + 1; e < s; e++){
        if (i == e) continue;
        insertEdge(nodes[i], nodes[e], 0);
     }
  }
}

void DiagramScene::createCGraph(QPointF pos)
{
   int k = mainWindow->spinNodes->value();
   qreal radius = mainWindow -> spinRadius->value();

   int posBegin = createConcentricNodes(k, radius, pos);
   int s = nodes.size();
   int i = 0;
   for(i = posBegin; i < (s - 1); i++)
   {
     insertEdge(nodes[i], nodes[i+1], 0);
   }
   insertEdge(nodes[i], nodes[posBegin]);
}
//! you can now be happy changing your node`s color.
void DiagramScene::changeSelectedNodeColor( QColor c ){ foreach( Node *n, selectedNodes ) n -> setColour( c ); }

//! throw some garbage on the mainWindow.
void DiagramScene::debug( const QString& s ){ mainWindow -> debug( s ); }

//! some really dirty get functions.
QList<Node*> DiagramScene::getSelectedNodes( ) const {  return selectedNodes; }

QList<Edge*> DiagramScene::getSelectedEdges( ) const {  return selectedEdges; }

QList<Node*> DiagramScene::getNodes( ) const {  return nodes; }

QList<Edge*> DiagramScene::getEdges( ) const {  return edges; }

QList<Group*> DiagramScene::getGroups( ) const {  return groups; }


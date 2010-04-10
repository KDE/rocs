/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef GRAPHDOCUMENT_H
#define GRAPHDOCUMENT_H

#include <QList>
#include <QObject>
#include "rocslib_export.h"
#include <ToolsPluginInterface.h>

class Graph;

/*! \brief the graph document
  long explanation needed.
*/
class ROCSLIB_EXPORT GraphDocument : public QObject, public QList<Graph*>
{
    Q_OBJECT

public:
    enum Type {Simple = 0, Oriented};
    /*! Default Constructor
      \param name sets the name of the document.
    */
    GraphDocument(const QString name, int width = 800, int heigth = 600);

    /*! default copy constructor */
    GraphDocument(const GraphDocument& gd);

    /*! Default Destructor */
    ~GraphDocument();

    /*! verifies if this document is changed */
    bool isModified();

    /*! Sets the current file name of the Graph Collection
      \param name changes the name of the document.
    */
    void setName(const QString& name);

    /*! \return the Name of the Collection of Graphs */
    QString name() const;

    /*! set the height of the working area
        \param height the new height of the document.
    */
    void setHeight(qreal height);

    /*! \return the heigth of the working area */
    qreal height() const;

    /*! set the width of the working area
      \param width the new width of the working area.
    */
    void setWidth(qreal width);

    /*! \return the width of the working area */
    qreal width() const;

    /* */
    void savedDocumentAt(const QString& fileName);

    const QString& documentPath() const ;

    bool saveAsInternalFormat(const QString& filename);
    void loadFromInternalFormat(const QString& filename);
public slots:
    /*! Creates a new Graph
      \param name the name of the graph
      \param type the the of the graph.
    */
    Graph *addGraph(QString name = "untitled");
    Graph *activeGraph(){ return _activeGraph; }
    void setActiveGraph(Graph *g);
    
    /** Run the tool in this GraphDocument.
    @param tool Tool plugin to be runned. */
    void runnTool(Rocs::ToolsPluginInterface * plugin);
    
    
signals:
    /*! emited when a new graph is created
      \param g the created graph */
    void graphCreated( Graph *g);

    /*! emitted when a graph is removed.
      \param i the index of the removed graph */
    void graphRemoved(int i);

    /*! emitted when the document changes it's name.
      \param name the new name of the document */
    void nameChanged(QString name);

    /*! emitted when the document changes it's height
      \param height the new height of the document */
    void heightChanged(qreal height);

    /*!  emitted when the document changes it's width
      \param width the new width of the document */
    void widthChanged(qreal width);

    void activeGraphChanged(Graph* g);
private:
    QString buf;
    void  savePropertiesInternalFormat(QObject *o);
    QString _lastSavedDocumentPath;
    QString _name;
    qreal _width;
    qreal _height;
    bool _modified;
    bool _saved;
    Graph *_activeGraph;
};

#endif

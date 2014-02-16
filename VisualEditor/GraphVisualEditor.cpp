/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GraphVisualEditor.h"
#include "Scene/GraphScene.h"
#include "Scene/DataItem.h"
#include "Scene/PointerItem.h"

#include "Document.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include <CoreTypes.h>
#include "Interface/EditorToolbar.h"

#include "Actions/PropertiesDialogAction.h"
#include "Actions/AlignAction.h"

#include <KLocale>
#include <QVBoxLayout>
#include <QGraphicsItem>
#include <KDebug>
#include <KComboBox>
#include <QGraphicsView>
#include <QLabel>
#include <QPainter>
#include <QSlider>
#include <QSpacerItem>
#include <QToolButton>
#include <QFrame>
#include <DocumentManager.h>
#include <KPushButton>
#include <QButtonGroup>
#include <QLayout>
#include <KActionCollection>

// load catalog for library
// static const KCatalogLoader loader("rocsvisualeditor"); //FIXME add translation


// class GraphVisualEditorPrivate
class GraphVisualEditorPrivate
{
public:
    GraphVisualEditorPrivate()
        : _editorToolbar(0)
    {
        _scene = 0;
        _document = 0;
    }

    ~GraphVisualEditorPrivate()
    { }

    GraphScene *_scene;
    EditorToolbar *_editorToolbar;

    KComboBox *_documentSelectorCombo;
    QToolButton *_documentPropertiesButton;
    KComboBox *_dataStructureSelectorCombo;
    QToolButton *_dataStructurePropertiesButton;
    KPushButton *_removeDataStructureButton;

    QSlider *_zoomSlider;
    Document *_document;
    DataStructurePtr _dataStructure;

    QGraphicsView *_graphicsView;
};


GraphVisualEditor::GraphVisualEditor(QWidget *parent)
    : QWidget(parent)
    , d(new GraphVisualEditorPrivate())
{
    setupWidgets();
}

GraphVisualEditor::~GraphVisualEditor()
{
    // d-pointer is scoped pointer and need not to be deleted
}

GraphVisualEditor * GraphVisualEditor::_self = 0;

GraphVisualEditor * GraphVisualEditor::self()
{
    if (!_self) {
        _self = new GraphVisualEditor(0);
    }
    return _self;
}

void GraphVisualEditor::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    // resize whiteboard to editor size
    d->_scene->resize();
}

void GraphVisualEditor::setupWidgets()
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    d->_scene = new GraphScene(this);
    d->_graphicsView = new QGraphicsView(this);
    d->_graphicsView->setRenderHints(QPainter::Antialiasing);
    d->_graphicsView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    d->_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    d->_graphicsView->setScene(d->_scene);

    // add controls for graph scene
    d->_zoomSlider = new QSlider(Qt::Horizontal, this);
    d->_zoomSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    d->_zoomSlider->setMaximum(100);
    d->_zoomSlider->setMinimum(-100);
    updateZoomSlider(d->_scene->zoomFactor());

    // at first setup document does not exists
    // this is later set when MainWindow::setActiveDocument() is called
    PropertiesDialogAction *propertiesAction = new PropertiesDialogAction(i18nc("@action:button", "Properties"), DocumentManager::self().activeDocument(), this);
    d->_documentPropertiesButton = new QToolButton(this);
    d->_documentPropertiesButton->setDefaultAction(propertiesAction);
    d->_documentPropertiesButton->setIcon(KIcon("document-properties"));

    // scene controls for top line
    vLayout->addWidget(sceneToolbar());
    vLayout->addWidget(d->_graphicsView);
    setLayout(vLayout);

    // listen for document manager changes
    connect(&DocumentManager::self(), SIGNAL(documentRemoved(Document*)),
            this, SLOT(releaseDocument()));
    connect(&DocumentManager::self(), SIGNAL(documentListChanged()),
            this, SLOT(updateGraphDocumentList()));
}

QWidget * GraphVisualEditor::sceneToolbar()
{
    QWidget *sceneControls = new QWidget(this);

    // document selection
    d->_documentSelectorCombo = new KComboBox(this);
    d->_documentSelectorCombo->setMinimumWidth(100);
    sceneControls->setLayout(new QHBoxLayout(this));
    sceneControls->layout()->addWidget(new QLabel(i18nc("@label:listbox", "Graph Document:")));
    sceneControls->layout()->addWidget(d->_documentSelectorCombo);
    sceneControls->layout()->addWidget(d->_documentPropertiesButton);

    // control separator
    QFrame* separator = new QFrame(this);
    separator->setFrameStyle(QFrame::VLine);
    sceneControls->layout()->addWidget(separator);

    // data structure selection
    sceneControls->layout()->addWidget(new QLabel(i18n("Data Structure:"), this));
    d->_dataStructureSelectorCombo = new KComboBox(this);
    d->_dataStructureSelectorCombo->setMinimumWidth(100);
    sceneControls->layout()->addWidget(d->_dataStructureSelectorCombo);
    d->_dataStructurePropertiesButton = new QToolButton(this);
    d->_dataStructurePropertiesButton->setMaximumWidth(24);
    d->_dataStructurePropertiesButton->setIcon(KIcon("document-properties"));
    sceneControls->layout()->addWidget(d->_dataStructurePropertiesButton);
    // create add data structure button
    KPushButton* addDataStructureButton = new KPushButton(this);
    addDataStructureButton->setIcon(KIcon("rocsnew"));
    addDataStructureButton->setToolTip(i18nc("@info:tooltip", "Add a new data structure."));
    addDataStructureButton->setMaximumWidth(24);
    sceneControls->layout()->addWidget(addDataStructureButton);
    // create remove data structure button
    d->_removeDataStructureButton = new KPushButton(this);
    d->_removeDataStructureButton->setIcon(KIcon("rocsdelete"));
    d->_removeDataStructureButton->setToolTip(i18nc("@info:tooltip", "Remove selected data structure."));
    d->_removeDataStructureButton->setMaximumWidth(24);
    sceneControls->layout()->addWidget(d->_removeDataStructureButton);

    QSpacerItem *spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    sceneControls->layout()->addItem(spacerItem);
    sceneControls->layout()->addWidget(d->_zoomSlider);

    // connections for buttons
    connect(d->_documentSelectorCombo, SIGNAL(activated(int)),
            &DocumentManager::self(), SLOT(changeDocument(int)));
    connect(addDataStructureButton, SIGNAL(clicked()),
            this, SLOT(addDataStructure()));
    connect(d->_removeDataStructureButton, SIGNAL(clicked()),
            this, SLOT(removeDataStructure()));

    // add connections for zoom slider
    connect(d->_zoomSlider, SIGNAL(valueChanged(int)),
            this, SLOT(zoomTo(int)));
    connect(d->_scene, SIGNAL(zoomFactorChanged(qreal)),
            this, SLOT(updateZoomSlider(qreal)));

    return sceneControls;
}

void GraphVisualEditor::updateGraphDocumentList()
{
    d->_documentSelectorCombo->clear();
    foreach(Document* document, DocumentManager::self().documentList()) {
        d->_documentSelectorCombo->addItem(document->name());
    }
}

void GraphVisualEditor::setActiveDocument()
{
    if (d->_document != 0 && d->_document != DocumentManager::self().activeDocument()) {
        disconnect(d->_document);
        d->_document->disconnect(d->_dataStructureSelectorCombo);
        releaseDocument();
    }
    d->_document = DocumentManager::self().activeDocument();
    d->_scene->setActiveDocument();

    // set button for document properties
    if (d->_documentPropertiesButton->defaultAction()) {
        d->_documentPropertiesButton->defaultAction()->deleteLater();
        PropertiesDialogAction *propertiesAction = new PropertiesDialogAction(i18nc("@action:button", "Properties"),
                                                                              d->_document,
                                                                              this);
        d->_documentPropertiesButton->setDefaultAction(propertiesAction);
        d->_documentPropertiesButton->setIcon(KIcon("document-properties"));
    }
    updateDataStructureList();

    connect(d->_dataStructureSelectorCombo, SIGNAL(activated(int)),
            d->_document, SLOT(setActiveDataStructure(int)));
    connect(d->_document, SIGNAL(activeDataStructureChanged(DataStructurePtr)),
            this, SLOT(updateActiveDataStructure(DataStructurePtr)));
    connect(d->_document, SIGNAL(dataStructureCreated(DataStructurePtr)),
            this, SLOT(updateDataStructureList()));
    connect(d->_document, SIGNAL(dataStructureCreated(DataStructurePtr)),
            d->_scene, SLOT(createItems(DataStructurePtr)));
    connect(d->_document, SIGNAL(dataStructureListChanged()),
            this, SLOT(updateDataStructureList()));

    // Graphical Data Structure Editor toolbar
    d->_editorToolbar->setActiveDocument(d->_document);
}

void GraphVisualEditor::releaseDocument()
{
    if (!d->_document) {
        return;
    }
    d->_scene->clear();
    foreach(DataStructurePtr ds, d->_document->dataStructures()) {
        ds->disconnect(this);
    }
    d->_document->disconnect(this);
    d->_document = 0;
}

void GraphVisualEditor::setupActions(KActionCollection* collection)
{
    // create editor toolbar
    if (d->_editorToolbar == 0) {
        d->_editorToolbar = new EditorToolbar(this);
    }
    d->_editorToolbar->setup(d->_scene, collection);

    // create alignment menu
    collection->addAction("align-hbottom", new AlignAction(i18nc("@action:intoolbar Alignment", "Base"),  AlignAction::Bottom, d->_scene));
    collection->addAction("align-hcenter", new AlignAction(i18nc("@action:intoolbar Alignment", "Center"), AlignAction::HCenter, d->_scene));
    collection->addAction("align-htop", new AlignAction(i18nc("@action:intoolbar Alignment", "Top"),   AlignAction::Top, d->_scene));
    collection->addAction("align-vleft", new AlignAction(i18nc("@action:intoolbar Alignment", "Left"),  AlignAction::Left, d->_scene));
    collection->addAction("align-vcenter", new AlignAction(i18nc("@action:intoolbar Alignment", "Center"), AlignAction::VCenter, d->_scene));
    collection->addAction("align-vright", new AlignAction(i18nc("@action:intoolbar Alignment", "Right"), AlignAction::Right, d->_scene));
    collection->addAction("align-circle", new AlignAction(i18nc("@action:intoolbar Alignment", "Circle"),  AlignAction::Circle, d->_scene));
    collection->addAction("align-tree", new AlignAction(i18nc("@action:intoolbar Alignment", "Minimize Crossing Edges"), AlignAction::MinCutTree, d->_scene));
}

void GraphVisualEditor::updateActiveDataStructure(DataStructurePtr g)
{
    d->_dataStructure = g;
    d->_scene->setActiveGraph(g);

    // set property to edit current data structure
    PropertiesDialogAction *dsProperty = new PropertiesDialogAction(i18nc("@action:button", "Properties"),
                                                                    DocumentManager::self().activeDocument()->activeDataStructure(),
                                                                    this);
    d->_dataStructurePropertiesButton->defaultAction()->deleteLater();
    d->_dataStructurePropertiesButton->setDefaultAction(dsProperty);
}

void GraphVisualEditor::updateDataStructureList()
{
    d->_dataStructureSelectorCombo->clear();
    foreach(DataStructurePtr ds, DocumentManager::self().activeDocument()->dataStructures()) {
        d->_dataStructureSelectorCombo->addItem(ds->name());
    }
    d->_dataStructureSelectorCombo->setCurrentIndex(d->_document->dataStructures().indexOf(d->_document->activeDataStructure()));

    // set property to edit current data structure
    PropertiesDialogAction *dsProperty = new PropertiesDialogAction(i18nc("@action:button", "Properties"),
                                                                    DocumentManager::self().activeDocument()->activeDataStructure(),
                                                                    this);
    d->_dataStructurePropertiesButton->defaultAction()->deleteLater();
    d->_dataStructurePropertiesButton->setDefaultAction(dsProperty);
}

void GraphVisualEditor::addDataStructure()
{
    DocumentManager::self().activeDocument()->addDataStructure();
    d->_removeDataStructureButton->setEnabled(true);
}

void GraphVisualEditor::removeDataStructure()
{
    DocumentManager::self().activeDocument()->activeDataStructure()->remove();
    if (DocumentManager::self().activeDocument()->dataStructures().isEmpty()) {
        d->_removeDataStructureButton->setDisabled(true);
    }
}

QList<DataItem*> GraphVisualEditor::selectedNodes() const
{
    QList<DataItem*> tmpList;
    QList<QGraphicsItem*> l = d->_scene->selectedItems();
    foreach(QGraphicsItem * i, l) {
        if (qgraphicsitem_cast<DataItem*>(i)) {
            tmpList.append(qgraphicsitem_cast<DataItem*>(i));
        }
    }
    return tmpList;
}

void GraphVisualEditor::updateZoomSlider(qreal zoomFactor)
{
    int sliderValue = 100*(zoomFactor-1);
    d->_zoomSlider->setToolTip(i18nc("@info:tooltip current zoom factor for graph editor", "Zoom: %1\%", zoomFactor*100));
    d->_zoomSlider->setValue(sliderValue);
}

void GraphVisualEditor::zoomTo(int sliderValue)
{
    qreal zoomFactor = sliderValue/100.0 + 1;
    d->_scene->zoomTo(zoomFactor);
}

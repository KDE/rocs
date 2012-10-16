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
static const KCatalogLoader loader("rocsvisualeditor");

GraphVisualEditor::GraphVisualEditor(QWidget *parent) :
    QWidget(parent),
    _editorToolbar(0)
{
    _scene = 0;
    _document = 0;
    setupWidgets();
}

GraphVisualEditor* GraphVisualEditor::_self = 0;

GraphVisualEditor* GraphVisualEditor::self()
{
    if (!_self) {
        _self = new GraphVisualEditor(0);
    }
    return _self;
}

void GraphVisualEditor::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    _scene->updateMinSize(size().width(), size().height());
}

void GraphVisualEditor::setupWidgets()
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    _scene = new GraphScene(this);
    _graphicsView = new QGraphicsView(this);
    _graphicsView->setRenderHints(QPainter::Antialiasing);
    _graphicsView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    _graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    _graphicsView->setScene(_scene);

    // add controls for graph scene
    _zoomSlider = new QSlider(Qt::Horizontal, this);
    _zoomSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    _zoomSlider->setMaximum(100);
    _zoomSlider->setMinimum(-100);
    updateZoomSlider(_scene->zoomFactor());

    // at first setup document does not exists
    // this is later set when MainWindow::setActiveDocument() is called
    PropertiesDialogAction *propertiesAction = new PropertiesDialogAction(i18nc("@action:button", "Properties"), DocumentManager::self()->activeDocument(), this);
    _documentPropertiesButton = new QToolButton(this);
    _documentPropertiesButton->setDefaultAction(propertiesAction);
    _documentPropertiesButton->setIcon(KIcon("document-properties"));

    // scene controls for top line
    vLayout->addWidget(sceneToolbar());
    vLayout->addWidget(_graphicsView);
    setLayout(vLayout);

    // listen for document manager changes
    connect(DocumentManager::self(), SIGNAL(documentRemoved(Document*)),
            this, SLOT(releaseDocument()));
    connect(DocumentManager::self(), SIGNAL(documentListChanged()),
            this, SLOT(updateGraphDocumentList()));
}

QWidget* GraphVisualEditor::sceneToolbar()
{
    QWidget *sceneControls = new QWidget(this);

    // document selection
    _documentSelectorCombo = new KComboBox(this);
    _documentSelectorCombo->setMinimumWidth(100);
    sceneControls->setLayout(new QHBoxLayout(this));
    sceneControls->layout()->addWidget(new QLabel(i18nc("@label:listbox", "Graph Document:")));
    sceneControls->layout()->addWidget(_documentSelectorCombo);
    sceneControls->layout()->addWidget(_documentPropertiesButton);

    // control separator
    QFrame* separator = new QFrame(this);
    separator->setFrameStyle(QFrame::VLine);
    sceneControls->layout()->addWidget(separator);

    // data structure selection
    sceneControls->layout()->addWidget(new QLabel(i18n("Data Structure:"), this));
    _dataStructureSelectorCombo = new KComboBox(this);
    _dataStructureSelectorCombo->setMinimumWidth(100);
    sceneControls->layout()->addWidget(_dataStructureSelectorCombo);
    _dataStructurePropertiesButton = new QToolButton(this);
    _dataStructurePropertiesButton->setMaximumWidth(24);
    _dataStructurePropertiesButton->setIcon(KIcon("document-properties"));
    sceneControls->layout()->addWidget(_dataStructurePropertiesButton);
    // create add data structure button
    KPushButton* addDataStructureButton = new KPushButton(this);
    addDataStructureButton->setIcon(KIcon("rocsnew"));
    addDataStructureButton->setToolTip(i18nc("@info:tooltip", "Add a new data structure."));
    addDataStructureButton->setMaximumWidth(24);
    sceneControls->layout()->addWidget(addDataStructureButton);
    // create remove data structure button
    KPushButton* removeDataStructureButton = new KPushButton(this);
    removeDataStructureButton->setIcon(KIcon("rocsdelete"));
    removeDataStructureButton->setToolTip(i18nc("@info:tooltip", "Remove selected data structure."));
    removeDataStructureButton->setMaximumWidth(24);
    sceneControls->layout()->addWidget(removeDataStructureButton);

    QSpacerItem *spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    sceneControls->layout()->addItem(spacerItem);
    sceneControls->layout()->addWidget(_zoomSlider);

    // connections for buttons
    connect(_documentSelectorCombo, SIGNAL(activated(int)),
            DocumentManager::self(), SLOT(changeDocument(int)));
    connect(addDataStructureButton, SIGNAL(clicked()),
            this, SLOT(addDataStructure()));
    connect(removeDataStructureButton, SIGNAL(clicked()),
            this, SLOT(removeDataStructure()));

    // add connections for zoom slider
    connect(_zoomSlider, SIGNAL(valueChanged(int)),
            this, SLOT(zoomTo(int)));
    connect(_scene, SIGNAL(zoomFactorChanged(qreal)),
            this, SLOT(updateZoomSlider(qreal)));

    return sceneControls;
}

void GraphVisualEditor::updateGraphDocumentList()
{
    _documentSelectorCombo->clear();
    foreach(Document * document, DocumentManager::self()->documentList()) {
        _documentSelectorCombo->addItem(document->name());
    }
}

void GraphVisualEditor::setActiveDocument()
{
    if (_document != 0 && _document != DocumentManager::self()->activeDocument()) {
        disconnect(_document);
        _document->disconnect(_dataStructureSelectorCombo);
        releaseDocument();
    }
    _document = DocumentManager::self()->activeDocument();
    _scene->setActiveDocument();

    // set button for document properties
    if (_documentPropertiesButton->defaultAction()) {
        _documentPropertiesButton->defaultAction()->deleteLater();
        PropertiesDialogAction *propertiesAction = new PropertiesDialogAction(i18nc("@action:button", "Properties"),
                                                                              _document,
                                                                              this);
        _documentPropertiesButton->setDefaultAction(propertiesAction);
        _documentPropertiesButton->setIcon(KIcon("document-properties"));
    }
    updateDataStructureList();

    connect(_dataStructureSelectorCombo, SIGNAL(activated(int)),
            _document, SLOT(setActiveDataStructure(int)));
    connect(_document, SIGNAL(activeDataStructureChanged(DataStructurePtr)),
            this, SLOT(updateActiveDataStructure(DataStructurePtr)));
    connect(_document, SIGNAL(dataStructureCreated(DataStructurePtr)),
            this, SLOT(updateDataStructureList()));
    connect(_document, SIGNAL(dataStructureListChanged()),
            this, SLOT(updateDataStructureList()));

    // Graphical Data Structure Editor toolbar
    _editorToolbar->setActiveDocument(_document);
}

void GraphVisualEditor::releaseDocument()
{
    if (!_document) {
        return;
    }
    _scene->clear();
    foreach(DataStructurePtr ds, _document->dataStructures()) {
        ds->disconnect(this);
    }
    _document->disconnect(this);
    _document = 0;
}

void GraphVisualEditor::setupActions(KActionCollection* collection)
{
    // create editor toolbar
    if (_editorToolbar == 0) {
        _editorToolbar = new EditorToolbar(this);
    }
    _editorToolbar->setup(_scene, collection);

    // create alignment menu
    collection->addAction("align-hbottom", new AlignAction(i18nc("@action:intoolbar Alignment", "Base"),  AlignAction::Bottom, _scene));
    collection->addAction("align-hcenter", new AlignAction(i18nc("@action:intoolbar Alignment", "Center"), AlignAction::HCenter, _scene));
    collection->addAction("align-htop", new AlignAction(i18nc("@action:intoolbar Alignment", "Top"),   AlignAction::Top, _scene));
    collection->addAction("align-vleft", new AlignAction(i18nc("@action:intoolbar Alignment", "Left"),  AlignAction::Left, _scene));
    collection->addAction("align-vcenter", new AlignAction(i18nc("@action:intoolbar Alignment", "Center"), AlignAction::VCenter, _scene));
    collection->addAction("align-vright", new AlignAction(i18nc("@action:intoolbar Alignment", "Right"), AlignAction::Right,  _scene));
    collection->addAction("align-circle", new AlignAction(i18nc("@action:intoolbar Alignment", "Circle"),  AlignAction::Circle, _scene));
    collection->addAction("align-tree", new AlignAction(i18nc("@action:intoolbar Alignment", "Minimize Crossing Edges"), AlignAction::MinCutTree, _scene));
}

void GraphVisualEditor::updateActiveDataStructure(DataStructurePtr g)
{
    _dataStructure = g;
    _scene->setActiveGraph(g);

    // set property to edit current data structure
    PropertiesDialogAction *dsProperty = new PropertiesDialogAction(i18nc("@action:button", "Properties"),
                                                                    DocumentManager::self()->activeDocument()->activeDataStructure(),
                                                                    this);
    _dataStructurePropertiesButton->defaultAction()->deleteLater();
    _dataStructurePropertiesButton->setDefaultAction(dsProperty);
}

void GraphVisualEditor::updateDataStructureList()
{
    _dataStructureSelectorCombo->clear();
    foreach(DataStructurePtr ds, DocumentManager::self()->activeDocument()->dataStructures()) {
        _dataStructureSelectorCombo->addItem(ds->name());
    }
    _dataStructureSelectorCombo->setCurrentIndex(_document->dataStructures().indexOf(_document->activeDataStructure()));

    // set property to edit current data structure
    PropertiesDialogAction *dsProperty = new PropertiesDialogAction(i18nc("@action:button", "Properties"),
                                                                    DocumentManager::self()->activeDocument()->activeDataStructure(),
                                                                    this);
    _dataStructurePropertiesButton->defaultAction()->deleteLater();
    _dataStructurePropertiesButton->setDefaultAction(dsProperty);
}

void GraphVisualEditor::addDataStructure()
{
    DocumentManager::self()->activeDocument()->addDataStructure();
}

void GraphVisualEditor::removeDataStructure()
{
    DocumentManager::self()->activeDocument()->activeDataStructure()->remove();
}

QList<DataItem*> GraphVisualEditor::selectedNodes() const
{
    QList<DataItem*> tmpList;
    QList<QGraphicsItem*> l = _scene->selectedItems();
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
    _zoomSlider->setToolTip(i18nc("@info:tooltip current zoom factor for graph editor", "Zoom: %1\%", zoomFactor*100));
    _zoomSlider->setValue(sliderValue);
}

void GraphVisualEditor::zoomTo(int sliderValue)
{
    qreal zoomFactor = sliderValue/100.0 + 1;
    _scene->zoomTo(zoomFactor);
}

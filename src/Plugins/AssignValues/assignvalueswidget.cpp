/*
    Rocs-Tools-Plugin: Automatic assign of values to edges/nodes
    Copyright (C) 2011  Andreas Cord-Landwehr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "assignvalueswidget.h"
#include "ui_assignvalueswidget.h"

#include <limits.h>
#include <KLocale>

#include <QtGui/QComboBox>
#include <QtGui/QDesktopWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtCore/QMap>
#include <QtCore/QPair>

#include <Document.h>
#include <DataStructure.h>
#include <DocumentManager.h>
#include "../DataStructure/Graph/GraphStructure.h"
#include <Pointer.h>
#include <Data.h>


//TODO output usefull error message
namespace boost { void throw_exception( std::exception const & ) {} } // do noop on exception
using namespace Rocs;

class QPushButton;

AssignValuesWidget::AssignValuesWidget(Document* graphDoc, QWidget* parent)
    :   QWidget(parent)
{
    ui = new Ui::AssignValuesWidget;
	ui->setupUi(this);
    graphDoc_ = graphDoc;
    
    // put widget at center of screen
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.screenNumber(parent)); 
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    move(center);
}


AssignValuesWidget::~AssignValuesWidget()
{
    delete ui;
}


void AssignValuesWidget::addDataStructures(QStringList dsNames)
{
    ui->dataStructuresCombo->insertItems(0, dsNames);
}


void AssignValuesWidget::assignValues()
{
    DataStructure* ds;
    QList<DataStructure*> dsList = DocumentManager::self()->activeDocument()->dataStructures();
// 
    // no graph datastructures given at document
    if (ui->dataStructuresCombo->count()==0)
        return;

    ds = dsList[ui->dataStructuresCombo->currentIndex()];
    if (!ds)
        return;
        
    switch((AssignMethod) ui->comboBoxMethod->currentIndex()) 
    {
        case ID: {
            int start = ui->spinBoxIDStartValue->value();
            qDebug() << "switch ID";
            if (ui->checkBoxAssignNodes->isChecked())  assignIDsToNodes(ds, start);
            if (ui->checkBoxAssignEdges->isChecked())  assignIDsToEdges(ds, start);
            break;
        }
    }
// 
//     if( ui->radioButtonMakeComplete->isChecked() )
//         makeComplete( graph );
//     if( ui->radioButtonEraseEdges->isChecked() )
//         removeAllEdges( graph );
//     if( ui->radioButtonReverseEdges->isChecked() )
//         reverseAllEdges( graph );
//     if( ui->radioButtonMakeSpanningtree->isChecked() )
//         makeSpanningTree( graph );

    close();
}

void AssignValuesWidget::assignIDsToNodes(DataStructure* ds, int start)
{
    QList<Data*> vertices = ds->dataList();

    for (int i=0; i<vertices.size(); i++)
    {
        vertices[i]->setValue(QString::number(start++));
    }
}

void AssignValuesWidget::assignIDsToEdges(DataStructure* ds, int start)
{
    QList<Pointer*> edges = ds->pointers();
    
    for (int i=0; i<edges.size(); i++)
        edges[i]->setValue(QString::number(start++));
}

#include "assignvalueswidget.moc"

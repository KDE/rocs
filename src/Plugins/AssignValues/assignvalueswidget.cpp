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

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>


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
        
    bool overrideValues = ui->checkBoxOverwriteValues->isChecked();
        
    switch((AssignMethod) ui->comboBoxMethod->currentIndex()) 
    {
        case ID: {
            int start = ui->spinBoxIDStartValue->value();

            if (ui->checkBoxAssignNodes->isChecked())  assignIDsToNodes(ds, start, overrideValues);
            if (ui->checkBoxAssignEdges->isChecked())  assignIDsToEdges(ds, start, overrideValues);
            break;
        }
        case UNIFORM_INTEGER: {
            int seed = ui->spinBoxIntegerGeneratorSeed->value();
            int lowerLimit = ui->spinBoxIntegerLowerLimit->value();
            int upperLimit = ui->spinBoxIntegerUpperLimit->value();

            if (ui->checkBoxAssignNodes->isChecked())  assignIntegersToNodes(ds, lowerLimit, upperLimit, seed, overrideValues);
            if (ui->checkBoxAssignEdges->isChecked())  assignIntegersToEdges(ds, lowerLimit, upperLimit, seed, overrideValues);
            break;
        }
        case UNIFORM_FLOAT: {
            int seed = ui->spinBoxIntegerGeneratorSeed->value();
            qreal lowerLimit = ui->spinBoxFloatLowerLimit->value();
            qreal upperLimit = ui->spinBoxFloatUpperLimit->value();

            if (ui->checkBoxAssignNodes->isChecked())  assignFloatsToNodes(ds, lowerLimit, upperLimit, seed, overrideValues);
            if (ui->checkBoxAssignEdges->isChecked())  assignFloatsToEdges(ds, lowerLimit, upperLimit, seed, overrideValues);
            break;
        }
    }

    close();
}


void AssignValuesWidget::assignIDsToNodes(DataStructure* ds, int start, bool overrideValues)
{
    QList<Data*> vertices = ds->dataList();

    for (int i=0; i<vertices.size(); i++)
    {
        if (!overrideValues && !vertices[i]->value().isNull())
            return;
        vertices[i]->setValue(QString::number(start++));
    }
}


void AssignValuesWidget::assignIDsToEdges(DataStructure* ds, int start, bool overrideValues)
{
    QList<Pointer*> edges = ds->pointers();
    
    for (int i=0; i<edges.size(); i++) {
        if (!overrideValues && !edges[i]->value().trimmed().isEmpty())
            return;
        edges[i]->setValue(QString::number(start++));
    }
}


void AssignValuesWidget::assignIntegersToNodes(DataStructure* ds, int lowerLimit, int upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit)
        return;

    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));
    
    boost::uniform_int<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    QList<Data*> vertices = ds->dataList();

    for (int i=0; i<vertices.size(); i++)
    {
        if (!overrideValues && !vertices[i]->value().isNull())
            return;
        vertices[i]->setValue(QString::number( die() ));
    }
}


void AssignValuesWidget::assignIntegersToEdges(DataStructure* ds, int lowerLimit, int upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit)
        return;

    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));
    
    boost::uniform_int<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    QList<Pointer*> edges = ds->pointers();
    
    for (int i=0; i<edges.size(); i++) {
        if (!overrideValues && !edges[i]->value().trimmed().isEmpty())
            return;
        edges[i]->setValue(QString::number( die() ));
    }
}


void AssignValuesWidget::assignFloatsToNodes(DataStructure* ds, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit)
        return;

    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));
    
    boost::uniform_real<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(gen, distribution);
    
    QList<Data*> vertices = ds->dataList();

    for (int i=0; i<vertices.size(); i++)
    {
        if (!overrideValues && !vertices[i]->value().isNull())
            return;
        vertices[i]->setValue(QString::number( die() ));
    }
}


void AssignValuesWidget::assignFloatsToEdges(DataStructure* ds, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit)
        return;

    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));
    
    boost::uniform_real<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(gen, distribution);
    
    QList<Pointer*> edges = ds->pointers();
    
    for (int i=0; i<edges.size(); i++)
    {
        if (!overrideValues && !edges[i]->value().trimmed().isEmpty())
            return;
        edges[i]->setValue(QString::number( die() ));
    }
}


#include "assignvalueswidget.moc"

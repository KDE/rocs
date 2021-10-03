/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2012 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ROCS1FILEFORMAT_H
#define ROCS1FILEFORMAT_H

#include "fileformats/fileformatinterface.h"

namespace GraphTheory
{

class Rocs1FileFormatPrivate;

/** \brief class RocsGraphFileFormatPlugin: Import and Export Plugin for internal graph format.
 *
 * A Document Internal format may look like this:
 *
 * #                                '#'s are comments.
 *
 * [Document Properties]            # Canvas Size and Data Structure initialization.
 * Width  : [integer]
 * Height : [integer]
 * DataStructurePlugin : [string]
 *
 * # data types
 * [DataType Identifier1]
 * Name     : [string]
 * IconName : [string]
 * Color    : [RGB in hex]
 *
 * # pointer types
 * [PointerType Identifier1]
 * Name  : [string]
 * Color : [RGB in hex]
 *
 * # Data Structure Definitions.
 * # Data Structures are layered, so there can be N of them.
 * # Every Data and Pointer below a declaration of a DataStructure
 * # belongs to that data structure.
 *
 * [DataStructure Name1]
 *
 * DataColor    : [RGB in hex]  # Color of newly created Data elements ( Nodes )
 * Pointercolor : [RBG in Hex]  # Color of newly created Data elements ( Nodes )
 * name         : [string]      # Name of the data structure accessible in the scripting interface.
 *
 * visibility : [bool]           # is this DataStructure visible?
 *
 * ShowNamesInData      : [bool] # should the canvas show the name of the data on the screen?
 * ShowNamesInNPointers : [bool] # ↑
 * ShowValuesInData     : [bool] # ↑
 * ShowValuesInPointers : [bool] # ↑
 *
 * PluginDefinedProperty1 : propertyValue; # the plugins can define other properties for the data structure.
 * PluginDefinedProperty1 : propertyValue;
 * PluginDefinedProperty1 : propertyValue;
 *
 * UserDefinedProperty1 : propertyValue1 # the user can define other properties for the data structure.
 * UserDefinedProperty2 : propertyValue2
 * UserDefinedProperty3 : propertyValue3
 *
 * [Data 1]
 * type  : [int]
 * x     : [real]
 * y     : [real]
 * color : [string in "0xFFFFFF" format]
 * name  : [string]
 * value : [string]
 * size  : [float]
 *
 * property1 : propertyValue1
 * property2 : propertyValue2
 * property3 : propertyValue3
 *
 * [Data 2] <- same as above.
 * type  : [int]
 * x     : [integer]
 * y     : [integer]
 * color : [string in "0xFFFFFF" format]
 * name  : [string]
 * value : [string]
 * size  : [float]
 *
 * property1 : propertyValue1
 * property2 : propertyValue2
 * property3 : propertyValue3
 *
 * [Pointer 1 -> 2]
 * type     : [int]
 * name     : [string]
 * value    : [string]
 * style    : [string]
 * color    : [string in "0xFFFFFF" format]
 * width    : [float]
 *
 * property1 : propertyValue1
 * property2 : propertyValue2
 * property3 : propertyValue3
 */



class Rocs1FileFormat : public FileFormatInterface
{
    Q_OBJECT

public:
    explicit Rocs1FileFormat(QObject *parent, const QList< QVariant >&);
    ~Rocs1FileFormat() override;

    /**
     * File extensions that are common for this file type.
     */
    const QStringList extensions() const override;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graphDocument to be serialized
     */
    void writeFile(GraphDocumentPtr graph) override;

    /**
     * Open given file and imports it into internal format.
     * \param file is url of a local file
     */
    void readFile() override;

private:
    QString serialize(GraphDocumentPtr document);
    void serializeProperties(QObject *o);

    Rocs1FileFormatPrivate *d;
};
}

#endif

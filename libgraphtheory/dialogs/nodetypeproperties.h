/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NODETYPEPROPERTIES_H
#define NODETYPEPROPERTIES_H

#include <QDialog>
#include "typenames.h"
#include "libgraphtheoryexport.h"

class QLineEdit;
class QSpinBox;
class QComboBox;
class KColorButton;

namespace GraphTheory {

class PropertiesWidget;

class GRAPHTHEORY_EXPORT NodeTypeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit NodeTypeProperties(QWidget *parent = 0);
    void setType(NodeTypePtr type);

private Q_SLOTS:
    void apply();
    void validateIdInput();

private:
    QLineEdit *m_name;
    QSpinBox *m_id;
    KColorButton *m_color;
    PropertiesWidget *m_properties;
    QPushButton *m_okButton;
    NodeTypePtr m_type;
};
}

#endif
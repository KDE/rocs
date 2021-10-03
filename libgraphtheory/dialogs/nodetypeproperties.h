/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODETYPEPROPERTIES_H
#define NODETYPEPROPERTIES_H

#include <QDialog>
#include "typenames.h"
#include "graphtheory_export.h"

class KColorButton;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QSpinBox;

namespace GraphTheory {

class PropertiesWidget;

class GRAPHTHEORY_EXPORT NodeTypeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit NodeTypeProperties(QWidget *parent = nullptr);
    void setType(NodeTypePtr type);

private Q_SLOTS:
    void apply();
    void validateIdInput();

private:
    QLineEdit *m_name;
    QSpinBox *m_id;
    KColorButton *m_color;
    QCheckBox *m_visible;
    QCheckBox *m_propertyNamesVisible;
    PropertiesWidget *m_properties;
    QPushButton *m_okButton;
    NodeTypePtr m_type;
};
}

#endif

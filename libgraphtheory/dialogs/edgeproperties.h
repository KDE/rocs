/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGEPROPERTIES_H
#define EDGEPROPERTIES_H

#include <QDialog>
#include "typenames.h"
#include "ui_edgeproperties.h"

namespace GraphTheory {

class EdgeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit EdgeProperties(QWidget *parent = nullptr);
    void setData(EdgePtr edge);

private Q_SLOTS:
    void apply();

private:
    EdgePtr m_edge;
    Ui::EdgeProperties *ui;
};
}

#endif

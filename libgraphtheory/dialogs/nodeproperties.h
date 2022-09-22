/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include "typenames.h"
#include "ui_nodeproperties.h"
#include <QDialog>

namespace GraphTheory
{

class NodeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit NodeProperties(QWidget *parent = nullptr);
    void setData(NodePtr node);

private Q_SLOTS:
    void apply();
    void validateIdInput();

private:
    NodePtr m_node;
    QPushButton *m_okButton;
    Ui::NodeProperties *ui;
};
}

#endif

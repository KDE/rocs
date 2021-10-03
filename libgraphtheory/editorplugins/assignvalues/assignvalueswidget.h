/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef ASSIGNVALUESWIDGET_H
#define ASSIGNVALUESWIDGET_H

#include "ui_assignvalueswidget.h"
#include "typenames.h"
#include <KLineEdit>
#include <QDialog>

class QGridLayout;
class Document;
class DataStructure;

namespace GraphTheory {

class AssignValuesWidget : public QDialog
{
    Q_OBJECT

    enum AssignMethod {
        ID,
        ALPHA,
        ID_ALPHA,
        UNIFORM_INTEGER,
        UNIFORM_FLOAT,
        CONSTANT
    };

public:
    explicit AssignValuesWidget(GraphDocumentPtr document, QWidget *parent = nullptr);
    ~AssignValuesWidget() override;

public Q_SLOTS:
    /**
     * Assign values as specified at the UI.
     */
    void assignValues();

    /**
     * Only enable Apply/Ok buttons if the specified property is valid.
     */
    void updateApplyButtonStates();

private:
    GraphDocumentPtr m_document;
    Ui::AssignValuesWidget *ui;
    QPushButton *m_okButton;
    QPushButton *m_applyButton;
};
}

#endif

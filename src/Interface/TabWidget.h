/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QString>
#include <KIcon>

class KMultiTabBar;
class QStackedWidget;
class QBoxLayout;
class KAction;

class TabWidget : public QWidget {
    Q_OBJECT
    KAction* _runAction;
public:
    enum Orientation {TabOnLeft, TabOnRight, TabOnTop, TabOnBottom};
    TabWidget(TabWidget::Orientation o, QWidget *parent = 0);
    void addWidget(QWidget *w, const QString& text,const KIcon& icon);
    void addAction(KAction *a);

private:
    void createLayout();
    KMultiTabBar *m_tabs;
    QStackedWidget *m_widgets;
    Orientation m_orientation;
    QBoxLayout *m_layout;
    int m_activeTab;
    int m_numOfTabs; // kmultitabbar is silly and doesn't let me query how much tabs I have.
    int m_numOfActions; // same
signals:
    void widgetActivated(int id);
    void actionTriggered(int id);

public slots:
  void setPlayString();
  void setStopString();

private slots:
    void releaseButton(int id);
    void releaseActionButton(int id);
    void controlPanel(int id);
};

#endif

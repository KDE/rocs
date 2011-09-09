/* 
    This file is part of Rocs.
    Copyright 2008-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>

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

private slots:
    void releaseButton(int id);
    void releaseActionButton(int id);
    void controlPanel(int id);
};

#endif

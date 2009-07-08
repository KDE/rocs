#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QString>
#include <KIcon>

class KMultiTabBar;
class QStackedWidget;
class QBoxLayout;
class KAction;

class TabWidget : public QWidget{
Q_OBJECT
public:
  enum Orientation{TabOnLeft, TabOnRight, TabOnTop, TabOnBottom};
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


private slots:
  void releaseButton(int id);
  void releaseActionButton(int id);
  void controlPanel(int id);
};

#endif
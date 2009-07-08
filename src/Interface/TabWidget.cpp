#include "TabWidget.h"
#include <QBoxLayout>
#include <QStackedWidget>

#include <kmultitabbar.h>
#include <QStackedWidget>
#include <KIcon>
#include <QSplitter>
#include <KDebug>
#include <KAction>

TabWidget::TabWidget( TabWidget::Orientation o, QWidget *parent) : QWidget(parent){
  m_orientation = o;
  m_layout = 0;
  m_numOfTabs = 0;
  m_tabs =  new KMultiTabBar((KMultiTabBar::KMultiTabBarPosition)o, this);
  m_tabs->setStyle(KMultiTabBar::KDEV3ICON);
  m_widgets = new QStackedWidget(this);

  createLayout();
}

void TabWidget::createLayout(){
  if (m_layout != 0){
    delete m_layout;
    m_layout = 0;
  }

  if ((m_orientation == TabOnLeft) || (m_orientation == TabOnRight) ){
    m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
  }
  else{
    m_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  }

  switch(m_orientation){
    case TabOnLeft  :
      m_layout->addWidget(m_tabs);
      m_layout->addWidget(m_widgets);
    break;
    case TabOnRight :
      m_layout->addWidget(m_widgets);
      m_layout->addWidget(m_tabs);
    break;
    case TabOnTop :
      m_layout->addWidget(m_tabs);
      m_layout->addWidget(m_widgets);
    break;
    case TabOnBottom :
      m_layout->addWidget(m_widgets);
      m_layout->addWidget(m_tabs);
  break;
  }

}

void TabWidget::controlPanel(int index){
  if ( m_widgets -> currentIndex() == index){
    m_widgets ->setVisible(! m_widgets -> isVisible() );
    if (! m_widgets->isVisible()){
      setMaximumSize(m_tabs->size());
    }
    else{
      setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }
  }
  else{
    m_widgets->setCurrentIndex(index);
    releaseButton(index);
    emit widgetActivated(index);
  }
  kDebug() << "Widget Size:" << size();
}

void TabWidget::releaseButton(int index) {
    if ( m_activeTab == index ) {
        m_tabs -> setTab( m_activeTab, true );
        return;
    }
    m_tabs -> setTab( m_activeTab, false );
    m_activeTab = index;
}

void TabWidget::addWidget(QWidget *w, const QString& text,const KIcon& icon){
  m_widgets -> addWidget(w);
  m_tabs -> appendTab(icon.pixmap(16), m_numOfTabs, text);
  connect(m_tabs->tab(m_numOfTabs), SIGNAL(clicked(int)), this, SLOT(controlPanel(int)));
  m_numOfTabs++;
}

void TabWidget::addAction(KAction *a){
  m_tabs -> appendTab( a->icon().pixmap(16), m_numOfTabs+m_numOfActions, a->text());
  connect(m_tabs->tab(m_numOfTabs+m_numOfActions), SIGNAL(clicked(int)), a, SLOT(trigger()));
  connect(m_tabs->tab(m_numOfTabs+m_numOfActions), SIGNAL(clicked(int)), this, SLOT(releaseActionButton(int)));
  kDebug() << "calling a->trigger() ";
  a->trigger();
}

void TabWidget::releaseActionButton(int index) {
    m_tabs->setTab(m_activeTab, true);
    m_tabs->setTab(index, false);
}

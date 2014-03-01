/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "LoadedPluginsDialog.h"
#include "GraphFileBackendManager.h"

#include <KLocale>
#include <KAboutData>
#include <KAboutApplicationDialog>
#include <KComponentData>
#include <KWidgetItemDelegate>
#include <KPushButton>
#include <KTitleWidget>
#include <QIcon>
#include <QIconLoader>

#include <QApplication>
#include <QAbstractListModel>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPainter>
#include <QPointer>

#define MARGIN 5
//FIXME revisit plugin list
bool sortPlugins(GraphFilePluginInterface* pluginL, GraphFilePluginInterface* pluginR)
{
    return pluginL->aboutData()->internalProgramName() < pluginR->aboutData()->internalProgramName();
}

class PluginsModel : public QAbstractListModel
{
public:
    enum ExtraRoles {
        CommentRole = Qt::UserRole+1
    };
    PluginsModel(QObject* parent = 0)
        : QAbstractListModel(parent)
    {
        _plugins = GraphFileBackendManager::self()->backends();
        qSort(_plugins.begin(), _plugins.end(), sortPlugins);
    }

    GraphFilePluginInterface* pluginData(const QModelIndex& index) const
    {
        if (!index.isValid()) {
            return 0;
        }
        if (index.parent().isValid()) {
            return 0;
        }
        if (index.column() != 0) {
            return 0;
        }
        if (index.row() >= _plugins.count()) {
            return 0;
        }
        return _plugins[index.row()];
    }

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        GraphFilePluginInterface* plugin = pluginData(index);
        if (!plugin) {
            return QVariant();
        }
        switch (role) {
            case Qt::DisplayRole:
            {
                QString name(plugin->aboutData()->productName());
                return name;
            }
            case CommentRole:
                return plugin->aboutData()->shortDescription();
            default:
                return QVariant();
        };
    }

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
    {
        if (!parent.isValid()) {
            return _plugins.count();
        }
        return 0;
    }

private:
    QList<GraphFilePluginInterface*> _plugins;
};


class LoadedPluginsDelegate : public KWidgetItemDelegate
{
    Q_OBJECT

public:

    LoadedPluginsDelegate(QAbstractItemView *itemView, QObject *parent = 0)
        : KWidgetItemDelegate(itemView, parent)
        , pushButton(new KPushButton)
    {
        pushButton->setIcon(QIcon("dialog-information")); // only for getting size matters
    }

    ~LoadedPluginsDelegate()
    {
        delete pushButton;
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        int i = 5;
        int j = 1;

        QFont font = titleFont(option.font);
        QFontMetrics fmTitle(font);

        return QSize(qMax(fmTitle.width(index.model()->data(index, Qt::DisplayRole).toString()),
                        option.fontMetrics.width(index.model()->data(index, PluginsModel::CommentRole).toString())) +
                        QIconLoader::SizeMedium + MARGIN * i + pushButton->sizeHint().width() * j,
                    qMax(QIconLoader::SizeMedium + MARGIN * 2, fmTitle.height() + option.fontMetrics.height() + MARGIN * 2));

    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const
    {
        if (!index.isValid()) {
            return;
        }

        painter->save();

        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        QRect contentsRect(dependantLayoutValue(MARGIN * 2 + option.rect.left(), option.rect.width() - MARGIN * 3, option.rect.width()), MARGIN + option.rect.top(), option.rect.width() - MARGIN * 3, option.rect.height() - MARGIN * 2);

        int lessHorizontalSpace = MARGIN * 2 + pushButton->sizeHint().width();

        contentsRect.setWidth(contentsRect.width() - lessHorizontalSpace);

        if (option.state & QStyle::State_Selected) {
            painter->setPen(option.palette.highlightedText().color());
        }

        if (itemView()->layoutDirection() == Qt::RightToLeft) {
            contentsRect.translate(lessHorizontalSpace, 0);
        }

        painter->save();

        painter->save();
        QFont font = titleFont(option.font);
        QFontMetrics fmTitle(font);
        painter->setFont(font);
        painter->drawText(contentsRect, Qt::AlignLeft | Qt::AlignTop, fmTitle.elidedText(index.model()->data(index, Qt::DisplayRole).toString(), Qt::ElideRight, contentsRect.width()));
        painter->restore();

        painter->drawText(contentsRect, Qt::AlignLeft | Qt::AlignBottom, option.fontMetrics.elidedText(index.model()->data(index, PluginsModel::CommentRole).toString(), Qt::ElideRight, contentsRect.width()));

        painter->restore();
        painter->restore();
    }

    QList<QWidget*> createItemWidgets() const
    {
        KPushButton *button = new KPushButton();
        button->setIcon(QIcon("dialog-information"));
        setBlockedEventTypes(button, QList<QEvent::Type>() << QEvent::MouseButtonPress
                             << QEvent::MouseButtonRelease << QEvent::MouseButtonDblClick);

        connect(button, SIGNAL(clicked(bool)), this, SLOT(info()));
        return QList<QWidget*>()
            << button;
    }

    void updateItemWidgets(const QList<QWidget*> widgets,
                           const QStyleOptionViewItem &option,
                           const QPersistentModelIndex &index) const
    {
        Q_UNUSED(index);
        KPushButton *aboutPushButton = static_cast<KPushButton*>(widgets[0]);
        QSize aboutPushButtonSizeHint = aboutPushButton->sizeHint();
        aboutPushButton->resize(aboutPushButtonSizeHint);
        aboutPushButton->move(dependantLayoutValue(option.rect.width() - MARGIN - aboutPushButtonSizeHint.width(), aboutPushButtonSizeHint.width(), option.rect.width()), option.rect.height() / 2 - aboutPushButtonSizeHint.height() / 2);
    }

    int dependantLayoutValue(int value, int width, int totalWidth) const
    {
        if (itemView()->layoutDirection() == Qt::LeftToRight) {
            return value;
        }
        return totalWidth - width - value;
    }

    QFont titleFont(const QFont &baseFont) const
    {
        QFont retFont(baseFont);
        retFont.setBold(true);
        return retFont;
    }

private Q_SLOTS:
    void info()
    {
        PluginsModel *model = static_cast<PluginsModel*>(itemView()->model());
        GraphFilePluginInterface* plugin = model->pluginData(focusedIndex());
        if (plugin) {
            QPointer<KAboutApplicationDialog> aboutPlugin = new KAboutApplicationDialog(plugin->aboutData(), itemView());
            aboutPlugin->exec();
            return;
        }
    }
private:
    QPushButton *pushButton;
};


class PluginsView : public QListView
{
public:
    PluginsView(QWidget* parent = 0)
        :QListView(parent)
    {
        setModel(new PluginsModel());
        setItemDelegate(new LoadedPluginsDelegate(this));
        setVerticalScrollMode(QListView::ScrollPerPixel);
    }

    virtual ~PluginsView()
    {
        // explicitly delete the delegate here since otherwise
        // we get spammed by warnings that the KPushButton we return
        // in createItemWidgets is deleted before the delegate
        delete itemDelegate();
    }

    virtual QSize sizeHint() const
    {
        QSize ret = QListView::sizeHint();
        ret.setWidth(qMax(ret.width(), sizeHintForColumn(0) + 30));
        return ret;
    }
};


LoadedPluginsDialog::LoadedPluginsDialog(QWidget* parent)
    : KDialog( parent )
{
    setPlainCaption(i18n("Loaded Plugins"));
    setButtons(KDialog::Close);
    setDefaultButton(KDialog::Close);

    QVBoxLayout* vbox = new QVBoxLayout(mainWidget());
    KTitleWidget* title = new KTitleWidget(this);
    title->setText(i18n("Graph File Plugins loaded for %1", KGlobal::mainComponent().aboutData()->programName()));
    vbox->addWidget(title);
    vbox->addWidget(new PluginsView());
}

#include "moc_LoadedPluginsDialog.cpp"
#include "LoadedPluginsDialog.moc"

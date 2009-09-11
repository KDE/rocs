#include "model_GraphProperties.h"
#include <KLocale>
#include <KDebug>

GraphPropertiesModel::GraphPropertiesModel( QObject *parent ) : QAbstractTableModel(parent) {
    // start all pointers to zero, so we don't crash things.
    _dataSource = 0;
    _metaObject = 0;
}

int GraphPropertiesModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    // if there's no dataSource, there's no data. return zero.
    // else return the size of properties of the object.
    if (_dataSource == 0) {
        return 0;
    }
    kDebug() << _dataSource->dynamicPropertyNames().size();
    return _dataSource->dynamicPropertyNames().size();
}

int GraphPropertiesModel::columnCount ( const QModelIndex & parent ) const {
    // should always be 2.
    Q_UNUSED(parent);
    return 2;
}

QVariant GraphPropertiesModel::data(const QModelIndex &index, int role) const {
    // error conditions, return a default value constructed QVariant().
    if ( !index.isValid() ) {
        return QVariant();
    }
    if ( index.row() >= _dataSource->dynamicPropertyNames().size() ) {
        return QVariant();
    }
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }

    // if it's the first column, return it's name.
    // if it's the second column, return it's data.

    if (index.column() == 0) {
        return _dataSource->dynamicPropertyNames()[index.row()];
    }
    else if (index.column() == 1) {
        return _dataSource->property( _dataSource->dynamicPropertyNames()[index.row()]);
    }
    // if it's anything else, it's an error, return a default value constructed QVariant.
    return QVariant();
}

QVariant GraphPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    // if the role is not for displaying anything, return a default empty value.
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return i18n("Property");
        case 1:
            return i18n("Value");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void GraphPropertiesModel::setDataSource(QObject *dataSource) {
    // if there isn't any datasource being send, just remove everything from the model and exit.
    if (dataSource == 0) {
        int count = rowCount();
        if (count == 0) return;
        beginRemoveRows(QModelIndex(), 0, count-1);
        endRemoveRows();
        return;
    }

    // clear the model for the new data.
    if ( _dataSource) {
        beginRemoveRows(QModelIndex(), 0, _dataSource->dynamicPropertyNames().size()-1);
        endRemoveRows();
    }

    // set some needed variables.
    _dataSource = dataSource;
    _metaObject = _dataSource -> metaObject();

    // insert the information.


    beginInsertRows(QModelIndex(), 0, dataSource->dynamicPropertyNames().size()-1);
    endInsertRows();

}

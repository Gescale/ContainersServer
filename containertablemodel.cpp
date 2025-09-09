#include "containertablemodel.h"

ContainerTableModel::ContainerTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
    _headerData << "Pallet" << "Container" << "Code" << "Height" << "Breadth/Diameter" << "Length" << "Weight";
}

int ContainerTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return _gridData.count();
}

int ContainerTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return _headerData.count();
}

QVariant ContainerTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    return _gridData[index.row()][index.column()];
}

QVariant ContainerTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return _headerData[section];
    }
    return QVariant();
}

void ContainerTableModel::updateData(const QList<QList<QVariant>>& newData) {
    beginResetModel();
    _gridData = newData;
    endResetModel();
}

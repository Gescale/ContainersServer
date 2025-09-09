#ifndef CONTAINERTABLEMODEL_H
#define CONTAINERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>

class ContainerTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ContainerTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void updateData(const QList<QList<QVariant>>& newData);

private:
    QList<QList<QVariant>> _gridData;
    QStringList _headerData;
};

#endif // CONTAINERTABLEMODEL_H

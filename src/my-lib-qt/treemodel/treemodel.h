#pragma once

#include <QAbstractItemModel>
#include <QScopedPointer>

namespace rdi {
class TreeModelPrivate;
class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    virtual ~TreeModel();
    explicit TreeModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

private:
    Q_DECLARE_PRIVATE(TreeModel)
    Q_DISABLE_COPY(TreeModel)
    QScopedPointer<TreeModelPrivate> d;
};
}

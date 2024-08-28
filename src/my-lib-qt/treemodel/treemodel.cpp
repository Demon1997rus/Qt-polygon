#include "treemodel.h"

#include "treemodel-private.h"

namespace rdi {
class TreeModelPrivate;
TreeModel::~TreeModel() {}

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent), d(new TreeModelPrivate) {}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    Node *child = d->findNode(parent)->child(row);
    return child ? createIndex(row, column, child) : QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    Node *parent = d->findNode(child)->parent();
    if (parent == d->root.data()) {
        return QModelIndex();
    }
    return createIndex(parent->row(), 0, parent);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    return d->findNode(parent)->rowCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return d->findNode(parent)->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.isValid()) {
        return reinterpret_cast<Node *>(index.internalId())->data(index.column());
    }
    return QVariant();
}

TreeModelPrivate::TreeModelPrivate() : root(new Node({})) {}

Node *TreeModelPrivate::findNode(const QModelIndex &index) const
{
    if (index.isValid()) {
        Node *node = reinterpret_cast<Node *>(index.internalId());
        if (node) {
            return node;
        }
    }
    return root.data();
}
}

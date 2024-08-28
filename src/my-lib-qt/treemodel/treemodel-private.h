#pragma once

#include <QScopedPointer>

#include "node.h"

namespace rdi {
class TreeModelPrivate {
public:
    TreeModelPrivate();
    Node *findNode(const QModelIndex &index) const;
    QScopedPointer<Node> root;
};
}

#pragma once

#include <QList>
#include <QVariant>

namespace rdi {
class Node {
public:
    Node(const QList<QVariant> data, Node *parent = nullptr);
    ~Node();

public:
    void appendChild(Node *child);
    Node *child(int row) const;
    int rowCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    Node *parent() const;

private:
    Node *m_parent;
    QList<Node *> m_children;
    QList<QVariant> m_data;
};
}

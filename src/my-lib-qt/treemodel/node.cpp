#include "node.h"

namespace rdi {

Node::Node(const QList<QVariant> data, Node *parent) : m_parent(parent), m_data(data) {}

Node::~Node()
{
    qDeleteAll(m_children);
}

void Node::appendChild(Node *child)
{
    m_children << child;
}

Node *Node::child(int row) const
{
    return m_children.value(row, nullptr);
}

int Node::rowCount() const
{
    return m_children.size();
}

int Node::columnCount() const
{
    return m_data.size();
}

QVariant Node::data(int column) const
{
    return m_data.value(column, "No data");
}

int Node::row() const
{
    if (m_parent) {
        return m_parent->m_children.indexOf(const_cast<Node *>(this));
    }
    return 0;
}

Node *Node::parent() const
{
    return m_parent;
}
}

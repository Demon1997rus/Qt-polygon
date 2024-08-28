#pragma once

#include <QScopedPointer>
#include <QString>

class QDomDocument;
class QDomElement;
class QDomNodeList;

namespace rdi {
class ConfigPrivate;
class Config {
public:
    Config();
    explicit Config(const QString &configPath);
    ~Config();

public:
    bool open(const QString &configPath);
    bool setContent(const QString &xml);
    QDomElement root() const;
    QDomDocument doc() const;
    bool isNull() const;
    QDomNodeList elementsByTagName(const QString &tagname) const;

private:
    Q_DISABLE_COPY(Config)
    QScopedPointer<ConfigPrivate> d;
};
}

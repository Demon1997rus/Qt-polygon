#include "config.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "config-private.h"

namespace rdi {

Config::Config() : d(new ConfigPrivate) {}

Config::Config(const QString &configPath) : d(new ConfigPrivate)
{
    open(configPath);
}

Config::~Config() {}

bool Config::open(const QString &configPath)
{
    QFile file(configPath);
    if (file.open(QIODevice::ReadOnly)) {
        qInfo() << "Opening" << configPath;
        QTextStream stream(&file);
        return setContent(stream.readAll());
    } else {
        qCritical() << Q_FUNC_INFO;
        qCritical() << "Not opening" << configPath;
        qCritical() << "Error" << file.errorString();
    }
    return false;
}

bool Config::setContent(const QString &xml)
{
    QString errorMsg;
    int errorLine;
    int errorColumn;
    if (!d->document.setContent(xml, &errorMsg, &errorLine, &errorColumn)) {
        qCritical() << Q_FUNC_INFO;
        qCritical() << Q_FUNC_INFO << "Config parsing failed:";
        qCritical() << "Line:" << errorLine << "Column:" << errorColumn;
        qCritical() << errorMsg;
        return false;
    }
    return true;
}

QDomElement Config::root() const
{
    return d->document.documentElement();
}

QDomDocument Config::doc() const
{
    return d->document;
}

bool Config::isNull() const
{
    return d->document.isNull();
}

QDomNodeList Config::elementsByTagName(const QString &tagname) const
{
    return d->document.elementsByTagName(tagname);
}
}

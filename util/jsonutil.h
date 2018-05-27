#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

class JsonUtil
{
public:
    JsonUtil(const QString& jsonFilePath);
    int getInt(const QString &path,int defaultValue = 0) const;
    bool getBool(const QString &path, bool defaultValue = false) const;
    QString getString(const QString &path,const QString &defaultValue = QString()) const;
    QStringList getStringList(const QString &path)const;
    QJsonValue getJsonValue(const QString &path)const;

private:
    QJsonObject rootObject;
};

#endif // JSONUTIL_H

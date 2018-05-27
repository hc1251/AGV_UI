#include "jsonutil.h"
#include <QDebug>

JsonUtil::JsonUtil(const QString &jsonFilePath)
{
    QFile file(jsonFilePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        rootObject = jsonDocument.object();
        qDebug()<<jsonFilePath;
    }else{
        QMessageBox::warning(0,"警告信息",QString("Can't open the file: %1").arg(jsonFilePath));
    }
}

int JsonUtil::getInt(const QString &path, int defaultValue) const
{
    return getJsonValue(path).toInt(defaultValue);
}

bool JsonUtil::getBool(const QString &path, bool defaultValue) const
{
    return getJsonValue(path).toBool(defaultValue);
}

QString JsonUtil::getString(const QString &path, const QString &defaultValue) const
{
    return getJsonValue(path).toString(defaultValue).trimmed();
}

QStringList JsonUtil::getStringList(const QString &path) const
{
    QStringList result;
    QJsonArray array = getJsonValue(path).toArray();
    for(QJsonArray::const_iterator iter = array.begin(); iter != array.end() ;++iter){
        QJsonValue value = *iter;
        result << value.toString().trimmed();
    }
    return result;
}

QJsonValue JsonUtil::getJsonValue(const QString &path) const
{
    QStringList tokens = path.split(QRegularExpression("\\."));
    QJsonObject object = rootObject;
    int size = tokens.size();
    //根据path提供的路径找个节点值
    for(int i = 0;i < size - 1; ++i){
        if(object.isEmpty()){
            return QJsonValue();
        }
        object = object.value(tokens.at(i)).toObject();
    }
    return object.value(tokens.last());
}


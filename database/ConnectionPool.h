#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QStack>
#include "util/ConfigUtil.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QSemaphore>
#include <QDebug>
#include <QTime>
class ConnectionPoolPrivate;
class ConnectionPool
{
    SINGLETON(ConnectionPool)
public:
    QSqlDatabase getConnection();  //获取连接
    void releaseConnection(const QSqlDatabase &connection); //释放连接
    void destoryConnectionPool(); //关闭连接
private:
    QSqlDatabase createConnection(const QString &connectName);
    ConnectionPoolPrivate *data;
};

#endif // CONNECTIONPOOL_H

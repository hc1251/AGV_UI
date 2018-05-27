#include "ConnectionPool.h"

class ConnectionPoolPrivate{
public:
    ConnectionPoolPrivate();
    ~ConnectionPoolPrivate();
    QStack<QString> usedConnectionNames;  //已使用的数据库连接
    QStack<QString> unUsedConnectionNames;  //未使用的数据库连接
    QString hostName;
    QString userName;
    QString password;
    QString databaseName;
    QString dataType;
    int port;
    bool testOnBorrow;
    QString testOnBorrowSql;
    int maxWaitTime;
    int maxConnectionCount;
    QSemaphore *semaphore;
    static QMutex mutex;
    static int lastKey;
};

QMutex ConnectionPoolPrivate::mutex;
int ConnectionPoolPrivate::lastKey = 0;
ConnectionPoolPrivate::ConnectionPoolPrivate(){
    ConfigUtil &config = Singleton<ConfigUtil>::getInstance();
    hostName = config.getDatabaseHost();
    databaseName = config.getDatabaseName();
    // databaseName ="agv.db";
    qDebug()<<databaseName;
    userName = config.getDatabaseUsername();
    password = config.getDatabasePassword();
    dataType = config.getDatabaseType();
    port = config.getDatabasePort();
    testOnBorrow = config.getDatabaseTestOnBorrow();
    testOnBorrowSql = config.getDatabaseTestOnBorrowSql();
    maxWaitTime = config.getDatabaseMaxWaitTime();
    maxConnectionCount = config.getDatabaseMaxConnectionCount();
    semaphore = new QSemaphore(maxConnectionCount);
}

ConnectionPoolPrivate::~ConnectionPoolPrivate()
{
    foreach (QString connenctName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connenctName);
    }
    foreach (QString connenctName, unUsedConnectionNames) {
        QSqlDatabase::removeDatabase(connenctName);
    }
    delete semaphore;
}

ConnectionPool::ConnectionPool():data(new ConnectionPoolPrivate)
{

}

ConnectionPool::~ConnectionPool()
{
    delete data;
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectName)
{
    Q_ASSERT(NULL != data);
    if(QSqlDatabase::contains(connectName)){
        QSqlDatabase existDb = QSqlDatabase::database(connectName);
        if(data->testOnBorrow){
            QSqlQuery query(data->testOnBorrowSql,existDb);
            if(query.lastError().type() != QSqlError::NoError && !existDb.open()){
                qDebug() << "Open datatabase error:" <<existDb.lastError().text();
                return QSqlDatabase();
            }
        }
        return existDb;
    }
    QSqlDatabase newDb = QSqlDatabase::addDatabase(data->dataType,connectName);
    newDb.setDatabaseName(data->databaseName);
    if (!newDb.open()) {
        qDebug() << "Open datatabase error:" << newDb.lastError().text();
        return QSqlDatabase();
    }
    qDebug()<<"new create connnection "<<connectName;
    return newDb;
}

QSqlDatabase ConnectionPool::getConnection()
{
    Q_ASSERT(NULL != data);
    if(data->semaphore->tryAcquire(1,data->maxWaitTime)){
        // 有已经回收的连接，复用它们
        // 没有已经回收的连接，则创建新的连接
        ConnectionPoolPrivate::mutex.lock();
        QString connectionName = data->unUsedConnectionNames.size() > 0  \
                ? data->unUsedConnectionNames.pop() \
                : QString("C%1").arg(++ConnectionPoolPrivate::lastKey);

        data->usedConnectionNames.push(connectionName);
        ConnectionPoolPrivate::mutex.unlock();
        //创建连接比较耗时，不放在lock范围内以提高并发效率
        QSqlDatabase db = createConnection(connectionName);
        if(!db.isOpen()){
            ConnectionPoolPrivate::mutex.lock();
            data->usedConnectionNames.removeOne(connectionName);
            data->unUsedConnectionNames.push(connectionName);
            ConnectionPoolPrivate::mutex.unlock();
            data->semaphore->release();
        }
        return db;
    }
    // 创建连接超时，返回一个无效连接
    qDebug()<< "Time out to create connection.";
    return QSqlDatabase();

}

void ConnectionPool::releaseConnection(const QSqlDatabase &connection)
{
    Q_ASSERT(NULL != data);
    QString connectName = connection.connectionName();
    if(data->usedConnectionNames.contains(connectName)){
        QMutexLocker locker(&ConnectionPoolPrivate::mutex);
        data->usedConnectionNames.removeOne(connectName);
        data->unUsedConnectionNames.push(connectName);
        //  qDebug()<<"shujuku"<<"unUnsed"<<data->unUsedConnectionNames.size()<<"used"<<data->usedConnectionNames;
        data->semaphore->release();
    }
}

void ConnectionPool::destoryConnectionPool()
{
    Q_ASSERT(NULL != data);
    if(NULL != data){
        ConnectionPoolPrivate::mutex.lock();
        delete data;
        data = NULL;
        ConnectionPoolPrivate::mutex.unlock();
    }
}








#ifndef ICONDRAW_H
#define ICONDRAW_H

#include <QObject>
#include <QFont>
#include <QFontDatabase>
#include <QMutex>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class IconDraw : public QObject
{
private:
    explicit IconDraw(QObject *parent = 0);
    QFont iconFont;
    static IconDraw* _instance;

public:
    static IconDraw* Instance()
    {
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new IconDraw;
            }
        }
        return _instance;
    }

    void SetIcon(QLabel* lab, QChar c, int size = 10);
    void SetIcon(QPushButton* btn, QChar c, int size = 10);

};

#endif // IconDraw_H

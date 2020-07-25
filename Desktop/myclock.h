#ifndef MYCLOCK_H
#define MYCLOCK_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <QStringList>
#include <QDate>
#include <QPainter>

namespace Ui {
class MyClock;
}

class MyClock : public QWidget
{
    Q_OBJECT

public:
    explicit MyClock(QWidget *parent = nullptr);
    ~MyClock();
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    double MaxDate(int m);
    int Week(QString str,QStringList list);
    void init();
private:
    Ui::MyClock *ui;

    QTimer* timer;		//定时器
    QStringList strListDate, strListState, strListWeek;
};

#endif // MYCLOCK_H

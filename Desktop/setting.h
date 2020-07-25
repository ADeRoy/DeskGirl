#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QLabel>
#include "myclock.h"
#include "finddesktop/finddesktop.h"

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();
signals:
    //定义status发送信号，widget槽函数收收
    void status_check_index(int index);
private slots:
    void slot_RadioButton_checket(int ButtonId);
private slots:
    void on_select_pushButton_clicked();
    void on_apply_pushButton_clicked();

    void on_close_button_clicked();

protected:
    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint mouseWindowTopLeft; //鼠标相对窗口左上角的坐标         在mousePressEvent 得到
    QPoint mouseDeskTopLeft;   //鼠标相对于桌面左上角坐标         在mouseMoveEvent实时获取
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角坐标    在mouseMoveEvent实时计算(矢量)获得
private:
    void Setting_Init();
private:

    Ui::Setting *ui;
    QString srcPath;    //选择的图片位置

    QButtonGroup* statusGroup;
private:
    HWND desktopWnd;    //桌面句柄
    MyClock* LabWidget;
    QWidget* bkWidget;
    QLabel* srcLabel;
};

#endif // SETTING_H

#ifndef WIDGET_H
#define WIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QtDebug>
#include <QSystemTrayIcon>
#include <QMenu>

#include "setting.h"
#include "Screen/screen.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void default_DeskTop();     //默认数字时钟壁纸
    void start_screenShot();    //开始截屏信号
private slots:
    void frame_refreshTime();
    void Setting_status_change(int);
    void Setting_status2_change(int);
    void Setting_mode_change(int);
    void Setting_status_isAction(bool);
    void Setting_window_isTop(bool);
private slots:
    void on_quit_button_clicked();

    void on_setting_Button_clicked();

    void on_add_button_clicked();

    void on_screen_shot_clicked();
private slots:
    int OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason);
protected:
    // Event handlers
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
private:
    void Widget_var_init();
    void Menu_init();
    void UI_Init();
public:
    /*实现左键按下移动窗口的位置坐标*/
    QPoint mouseWindowTopLeft; //鼠标相对窗口左上角的坐标         在mousePressEvent 得到
    QPoint mouseDeskTopLeft;   //鼠标相对于桌面左上角坐标         在mouseMoveEvent实时获取
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角坐标    在mouseMoveEvent实时计算(矢量)获得
private:
    int mode;   //人物模型
    int status; //人物状态
private:
    Ui::Widget *ui;
    /*动态人物*/
    QTimer* frameTimer;
private:
    /*系统菜单*/
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
private:
    /*应用菜单*/
    Setting* setting;
    /*屏幕截图*/
    Screen * screen;
};

#endif // WIDGET_H

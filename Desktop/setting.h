#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QLabel>
#include <QSettings>
#include <QTextCodec>
#include <QMovie>
#include "myclock.h"
#include "finddesktop/finddesktop.h"
#include "player/VlcPlayer.h"

//开机启动注册表
#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

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
    void status_check_index(int index); //girl1状态信号
    void status_isAction(bool);         //girl是否动态信号
    void status2_check_index(int index); //girl1状态信号
    void mode_check_index(int index);   //模型发生改变
    void window_isTop(bool flag);
private slots:
    void slot_RadioButton_checket(int ButtonId);    //girl1状态发生改变
    void slot_RadioButton2_checket(int ButtonId);   //girl2状态发生改变
    void slot_CheckButton_checket(int ButtonId);    //设置组发生改变
    void slot_modeButton_checket(int ButtonId);     //模型发生改变

    void slot_media_player_stop();  //vlc播放结束通知
private slots:
    void on_select_pushButton_clicked();
    void on_apply_pushButton_clicked();

    void on_close_button_clicked();

    void on_AppSettingButton_clicked();

    void on_TopPeopleButton_clicked();

    void on_TopPeopleButton_2_clicked();

    void on_TopPeopleButton_3_clicked();

    void on_TopPeopleButton_4_clicked();

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
    void SetAutoRun(bool isAutoRun);//是否开机启动
    void write_ini();
    void read_ini();
public:
    void default_DeskTop();
private:

    Ui::Setting *ui;

    Qt::WindowFlags flags;
    QString srcPath;    //选择的图片位置

    QButtonGroup* ButtonGroup;  //girl2的设置
    QButtonGroup* statusGroup;  //girl1的设置
    QButtonGroup* statusGroup2;  //girl2的设置
    QButtonGroup* AppSetting;
    QButtonGroup* modeGroup;    //模型组
private:
    HWND desktopWnd;    //桌面句柄
    MyClock* LabWidget; //数字时钟窗口
    QWidget* bkWidget;  //jpg，gif窗口
    QLabel* srcLabel;   //bkWidget的中心部件
    VlcPlayer* player;  //vlc播放器对象

};

#endif // SETTING_H

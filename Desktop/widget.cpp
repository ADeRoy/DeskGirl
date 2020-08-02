#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);      //去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);   //设置背景透明
    this->setGeometry(1500,600,500,500);
    Widget_var_init();  //初始化变量
    Menu_init();        //初始化菜单
    UI_Init();          //初始化UI

    ui->widget->setStyleSheet(QString("background-image:url(:/src/action1/%1.png);").arg(1));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::frame_refreshTime()
{
    static int frame = 1;
    if(mode == 0)
    {
        frame = frame%4 + 1;
        if(status==0) {
            ui->widget->setStyleSheet(QString("background-image:url(:/src/action1/%1.png);").arg(frame));
        }
        else if(status==1){
            ui->widget->setStyleSheet(QString("background-image:url(:/src/action2/%1.png);").arg(frame));
        }
        else {
            ui->widget->setStyleSheet(QString("background-image:url(:/src/action3/%1.png);").arg(frame));
        }
    }
    else if(mode == 1){
        frame = frame%6 + 1;
        if(status==0) {
            ui->widget->setStyleSheet(QString("background-image:url(:/src/default/action1/%1.png);").arg(frame));
        }
        else if(status == 1){
            ui->widget->setStyleSheet(QString("background-image:url(:/src/default/action2/%1.png);").arg(frame));
        }
        else if(status == 2){
            ui->widget->setStyleSheet(QString("background-image:url(:/src/default/action3/%1.png);").arg(frame));
        }
        else {
            ui->widget->setStyleSheet(QString("background-image:url(:/src/default/action4/%1.png);").arg(frame));
        }
    }
    else if (mode == 2) {
        frame = frame%6 + 1;
        ui->widget->setStyleSheet(QString("background-image:url(:/src/boy1/%1.png);").arg(frame));
    }
    else if (mode == 3) {
        frame = frame%6 + 1;
        ui->widget->setStyleSheet(QString("background-image:url(:/src/girl1/%1.png);").arg(frame));
    }
}

void Widget::Setting_status_change(int index)
{
    this->status = index;
    qDebug()<<"girl1状态改变为："<<index;
}

void Widget::Setting_status2_change(int index)
{
    this->status = index;
    qDebug()<<"girl2状态改变为："<<index;
}

void Widget::Setting_mode_change(int index)
{
    mode = index;
    switch (mode) {
    case 0:
        this->setGeometry(1500,600,500,500);
        break;
    case 1:
        this->setGeometry(1650,650,360,360);
        break;
    case 2:
        this->setGeometry(1650,550,196,608);
        break;
    case 3:
        this->setGeometry(1600,600,343,480);
        break;
    }
    qDebug()<<"模型为："<<index;
}

void Widget::Setting_status_isAction(bool isActive)
{
    if(isActive)
    {
        frameTimer->stop();
    }
    else {
        frameTimer->start(500);
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
    ui->setting_Button->setGeometry(event->x() + 50,event->y() - 100,32,32);
    ui->screen_shot->setGeometry(event->x() + 50,event->y() - 50,32,32);
    ui->add_button->setGeometry(event->x() + 50,event->y(),32,32);
    ui->quit_button->setGeometry(event->x() + 50,event->y() + 50,32,32);
    if(event->buttons() == Qt::RightButton)
    {
        ui->add_button->show();
        ui->setting_Button->show();
        ui->quit_button->show();
        ui->screen_shot->show();
    }
    if(event->buttons() == Qt::LeftButton)
    {
        ui->add_button->hide();
        ui->setting_Button->hide();
        ui->quit_button->hide();
        ui->screen_shot->hide();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    //窗口移动
    if (event->buttons() & Qt::LeftButton)
    {
        mouseDeskTopLeft = event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
        this->move(windowDeskTopLeft);     //移动到目的地
    }
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    status++;
    status = status % 3;
    qDebug()<<status;
}

void Widget::Widget_var_init()
{
    mode = 0;   //默认第一个小姐姐

    frameTimer = new QTimer;
    connect(frameTimer,SIGNAL(timeout()),this, SLOT(frame_refreshTime()));
    frameTimer->start(500);
    /*人物状态*/
    status = 0;
    setting = new Setting;
    //绑定设置的status信号
    connect(setting,SIGNAL(status_check_index(int)),this,SLOT(Setting_status_change(int)));
    connect(setting,SIGNAL(status_isAction(bool)),this,SLOT(Setting_status_isAction(bool)));
    connect(setting,SIGNAL(mode_check_index(int)),this,SLOT(Setting_mode_change(int)));
    connect(setting,SIGNAL(status2_check_index(int)),this,SLOT(Setting_status2_change(int)));
    setting->default_DeskTop();

    screen = new Screen;
    connect(this,SIGNAL(start_screenShot()),screen,SLOT(GetWindowSrceen()));
}

void Widget::Menu_init()
{
    /*系统菜单*/
    minimizeAction = new QAction(tr("最小化"), this);
    //minimizeAction->setIcon(QIcon(":/1.ico"));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("最大化"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("还原"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("退出"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    /*系统托盘*/
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
    SLOT(OnSystemTrayClicked(QSystemTrayIcon::ActivationReason)));   //单机双击系统托盘关联

    trayIcon->setContextMenu(trayIconMenu);
    /* 设置托盘显示的图标。 */
    trayIcon->setIcon(QIcon(":/DeskGirl.ico"));
    setWindowIcon(QIcon(":/DeskGirl.ico"));
    /* 设置鼠标移动到托盘图标显示的文本。 */
    trayIcon->setToolTip("DeskGirl");
    trayIcon->show();
}

void Widget::UI_Init()
{
    ui->add_button->hide();
    ui->setting_Button->hide();
    ui->quit_button->hide();
    ui->screen_shot->hide();
}

void Widget::on_quit_button_clicked()
{
    //this->close();
    qApp->quit();
}

void Widget::on_setting_Button_clicked()
{
    setting->show();
}

void Widget::on_add_button_clicked()
{
    this->hide();
}

void Widget::on_screen_shot_clicked()
{
    emit start_screenShot();
}

int Widget::OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
    //匹配双击和单击进行操作
    if (reason == QSystemTrayIcon::Trigger)
    {
        // 显示主窗口
        this->showNormal();
    }
    else if (reason == QSystemTrayIcon::DoubleClick) {
        //截屏
        emit start_screenShot();
    }
    return 0;
}

#include "setting.h"
#include "ui_setting.h"

#include <QMovie>

Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);      //去掉标题栏
    Setting_Init();
}

Setting::~Setting()
{
    delete ui;
}

void Setting::slot_RadioButton_checket(int ButtonId)
{
    //发送status信号
    emit status_check_index(ButtonId);
}

void Setting::on_select_pushButton_clicked()
{
    srcLabel->setParent(bkWidget);
    srcLabel->setGeometry(0,0,1920,1080);
    srcLabel->setScaledContents(true);         //是否缩放


    srcPath =QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("选择文件"),"",
                                          QString::fromLocal8Bit("媒体文件( *.gif *.jpg *.png)"));

    if(srcPath != "" && (srcPath.contains(".jpg")||srcPath.contains(".png")))//jpg,png
    {

        srcLabel->setPixmap(QPixmap(srcPath));
        if(desktopWnd) //设置父窗体为桌面
        {
            SetParent((HWND)bkWidget->winId(), desktopWnd);
            qDebug()<<"over";
        }
        bkWidget->setWindowFlags(Qt::FramelessWindowHint); //无边框
        bkWidget->showFullScreen();                        //全屏
        qDebug()<<"success";
        return;
    }
    if(srcPath != "" && srcPath.contains(".gif"))   //gif
    {
        QMovie* m_movie = new QMovie(srcPath);
        srcLabel->setMovie(m_movie);
        m_movie->start();//开始播放图片
        if(desktopWnd)  //设置父窗体为桌面
        {
            SetParent((HWND)bkWidget->winId(), desktopWnd);
            qDebug()<<"over";
        }
        bkWidget->setWindowFlags(Qt::FramelessWindowHint); //无边框
        bkWidget->showFullScreen();                        //全屏
        qDebug()<<"success";
        return;
    }
    qDebug()<<"faile";
}

void Setting::on_apply_pushButton_clicked()
{
    if(desktopWnd) //设置父窗体为桌面
    {
        SetParent((HWND)LabWidget->winId(), desktopWnd);
        qDebug()<<"over";
    }
    LabWidget->setWindowFlags(Qt::FramelessWindowHint); //无边框
    LabWidget->showFullScreen();                        //全屏
}

void Setting::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void Setting::mouseMoveEvent(QMouseEvent *event)
{
    //窗口移动
    if (event->buttons() & Qt::LeftButton)
    {
        mouseDeskTopLeft = event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
        this->move(windowDeskTopLeft);     //移动到目的地
    }
}

void Setting::Setting_Init()
{
    statusGroup = new QButtonGroup;
    statusGroup->setExclusive(true);   //重点    设置互斥
    statusGroup->addButton(ui->happy_radioButton,0);
    statusGroup->addButton(ui->anger_radioButton,1);
    statusGroup->addButton(ui->grief_radioButton,2);
    ui->happy_radioButton->setChecked(true);
    connect(statusGroup, SIGNAL(buttonClicked(int)), this,SLOT(slot_RadioButton_checket(int)));

    SendMessageToDesktop();                 //发送消息
    this->desktopWnd=findDesktopIconWnd();  //获取桌面句柄
    LabWidget = new MyClock;
    bkWidget = new QWidget;
    srcLabel = new QLabel;
}

void Setting::on_close_button_clicked()
{
    this->hide();   //隐藏
}

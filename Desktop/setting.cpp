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

void Setting::slot_RadioButton2_checket(int ButtonId)
{
    qDebug()<<"准备发送girl2状态改变信号";
    emit status2_check_index(ButtonId);
}

void Setting::slot_CheckButton_checket(int ButtonId)
{
    /*group修改，写入配置文件*/
    QCheckBox* temp = (QCheckBox*)AppSetting->button(ButtonId);
    switch(ButtonId)
    {
    case 0:
        if(temp->isChecked())
        {
            SetAutoRun(true);
            qDebug()<<"开启：开机自启";
        }
        else {
            SetAutoRun(false);
            qDebug()<<"关闭：开机自启";
        }
        break;
    case 1:
        if(temp->isChecked())
        {
            emit status_isAction(false);
        }
        else {
            emit status_isAction(true);
        }
        break;
    }
    write_ini();
}

void Setting::slot_modeButton_checket(int ButtonId)
{
    //给widget发送模型改变信号
    emit mode_check_index(ButtonId);
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
    //模型
    modeGroup = new QButtonGroup;
    modeGroup->setExclusive(true);   //重点    设置互斥
    modeGroup->addButton(ui->mode1_checkBox,0);
    modeGroup->addButton(ui->mode2_checkBox,1);
    modeGroup->addButton(ui->mode3_checkBox,2);
    modeGroup->addButton(ui->mode4_checkBox,3);
    connect(modeGroup, SIGNAL(buttonClicked(int)), this,SLOT(slot_modeButton_checket(int)));

    //选项按钮互斥
    ButtonGroup = new QButtonGroup;
    ButtonGroup->setExclusive(true);   //重点    设置互斥
    ButtonGroup->addButton(ui->AppSettingButton,0);
    ButtonGroup->addButton(ui->TopPeopleButton,1);
    ButtonGroup->addButton(ui->TopPeopleButton_2,2);
    ButtonGroup->addButton(ui->TopPeopleButton_3,3);
    ButtonGroup->addButton(ui->TopPeopleButton_4,4);

    //girl1状态按钮
    statusGroup = new QButtonGroup;
    statusGroup->setExclusive(true);   //重点    设置互斥
    statusGroup->addButton(ui->happy_radioButton,0);
    statusGroup->addButton(ui->anger_radioButton,1);
    statusGroup->addButton(ui->grief_radioButton,2);
    connect(statusGroup, SIGNAL(buttonClicked(int)), this,SLOT(slot_RadioButton_checket(int)));

    //girl2状态按钮
    statusGroup2 = new QButtonGroup;
    statusGroup2->setExclusive(true);   //重点    设置互斥
    statusGroup2->addButton(ui->happy2_radioButton,0);
    statusGroup2->addButton(ui->anger2_radioButton,1);
    statusGroup2->addButton(ui->grief2_radioButton,2);
    statusGroup2->addButton(ui->shy_radioButton,3);

    connect(statusGroup2, SIGNAL(buttonClicked(int)), this,SLOT(slot_RadioButton2_checket(int)));

    //App设置按钮
    AppSetting = new QButtonGroup;
    AppSetting->setExclusive(false);
    AppSetting->addButton(ui->autorun_checkBox,0);
    AppSetting->addButton(ui->action_checkBox,1);
    connect(AppSetting, SIGNAL(buttonClicked(int)), this,SLOT(slot_CheckButton_checket(int)));

    SendMessageToDesktop();                 //发送消息
    this->desktopWnd=findDesktopIconWnd();  //获取桌面句柄
    LabWidget = new MyClock;
    bkWidget = new QWidget;
    srcLabel = new QLabel;
    read_ini();
}

void Setting::SetAutoRun(bool isAutoRun)
{
    QString  strApplicationName = QApplication::applicationName();              //获取应用名称
    QSettings * settings = new QSettings(AUTO_RUN, QSettings::NativeFormat);

    if(isAutoRun)
    {
        QString strApplicationFilePath = QApplication::applicationFilePath();   //找到应用的目录
        settings->setValue(strApplicationName, strApplicationFilePath.replace("/", "\\"));//写入注册表
    }
    else
        settings->remove(strApplicationName);
}

void Setting::write_ini()
{
    //打开配置文件
    QSettings *config = new QSettings("DeskGirl.ini", QSettings::IniFormat);
    //设置编码方式
    config->setIniCodec(QTextCodec::codecForName("utf‐8"));
    //设置组group
    QString section=QString("setting/");
    config->beginGroup(section);                //设置组
    config->setValue(QString("%1").arg(AppSetting->button(0)->text()),QString("%1").arg(AppSetting->button(0)->isChecked()));
    config->setValue(QString("%1").arg(AppSetting->button(1)->text()),QString("%1").arg(AppSetting->button(1)->isChecked()));
    config->endGroup();
    delete config;
}

void Setting::read_ini()
{
    int value;
    //打开配置文件
    QSettings *config = new QSettings("DeskGirl.ini", QSettings::IniFormat);
    //设置编码方式
    config->setIniCodec(QTextCodec::codecForName("utf‐8"));
    //设置组group
    QString section=QString("setting/");        //一定要加/，否则读取会出问题

    value = config->value(section + QString("%1").arg(AppSetting->button(0)->text())).toInt();
    AppSetting->button(0)->setChecked(value);

    value = config->value(section + QString("%1").arg(AppSetting->button(1)->text())).toInt();
    AppSetting->button(1)->setChecked(value);

    delete config;
}

void Setting::default_DeskTop()
{
    emit ui->apply_pushButton->clicked();
}

void Setting::on_close_button_clicked()
{
    this->hide();   //隐藏
}

void Setting::on_AppSettingButton_clicked()
{
    //转到第一页
    ui->stackedWidget->setCurrentIndex(0);
}

void Setting::on_TopPeopleButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Setting::on_TopPeopleButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Setting::on_TopPeopleButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Setting::on_TopPeopleButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

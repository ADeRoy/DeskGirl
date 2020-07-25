## 无边框背景透明窗体
#### 设置属性
~~~C++
this->setWindowFlags(Qt::FramelessWindowHint);      //去掉标题栏
this->setAttribute(Qt::WA_TranslucentBackground);   //设置背景透明
~~~
#### 设置窗体png
在主`Widget`上拖一个`Widge`控件，然后设置Widget的窗体，以**qss**的形式设置窗体的背景图
~~~C++
ui->widget->setStyleSheet(QString("background-image:url(:/src/action1/%1.png);").arg(1));
~~~
因为要设置动态图，所以用一个定时器循环设置窗体的背景图
#### 定时器
~~~C++
  /*动态人物*/
  QTimer* frameTimer;
  frameTimer = new QTimer;
  connect(frameTimer,SIGNAL(timeout()),this, SLOT(frame_refreshTime()));
  frameTimer->start(500);
~~~
定时器循环设置窗体背景图
~~~C++
void Widget::frame_refreshTime()
{
    static int frame = 1;
    frame = frame%4 + 1;
    //qDebug()<<"frame:"<<frame;
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
~~~
这里**status**是人物状态，0喜，1怒，2哀

`这样一个不规则窗体就设置完成了，加上小姐姐动态图组，nice`
## Setting设计
项目->右键->Add New->选择Qt->选择Qt Desiger Form->选择Widget
UI是这样设计的
![](https://imgkr.cn-bj.ufileos.com/cc4e9e08-eec7-4df4-8da8-df15b9a1f7a8.png)
为什么是花花绿绿的呢，这个**设置样式表**，直接copy我的就行
#### 人物状态选项
这个直接绑定信号与槽，让主界面和setting界面进行数据传递
Setting界面定义信号，check按钮之后发送信号
~~~C++
signals:
    //定义status发送信号，widget槽函数收收
    void status_check_index(int index);
~~~
~~~C++
void Setting::slot_RadioButton_checket(int ButtonId)
{
    //发送status信号
    emit status_check_index(ButtonId);
}
~~~
Widget绑定信号
~~~C++
/*应用菜单*/
Setting* setting;
setting = new Setting;
//绑定设置的status信号
connect(setting,SIGNAL(status_check_index(int)),this,SLOT(Setting_status_change(int)));
~~~
这样就完成了两个窗体的交互
## 设置桌面壁纸
简单来说就是找到桌面Program Manager，给桌面发送0x52c消息，让它接触父子关系，然和将自己写好的窗口和它建立父子关系`SetParent`
#### 获取桌面句柄
这里直接封装阿几个函数，然后调用，具体看源码
~~~C++
SendMessageToDesktop();   //发送消息,解除父子关系
this->desktopWnd=findDesktopIconWnd();    //获取桌面句柄
~~~
#### 建立父子关系
这里以单独的背景图片为例，没有数字时钟的
~~~C++
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
~~~
因为要显示图片。所以给bkWidget添加一个Qlabel，相关设置之后，就是将用户选择的图片路径给srcLabel去显示。

设置父窗体为桌面先判断是否找到桌面，然后再给桌面设置自己的Widget为它的父亲，这样就完成了桌面壁纸的设置
## 数字时钟壁纸
#### MyClock
这是自己重写的QWidget类`MyClock`，主要完成的是绘制问题，设置锚点，然后绘制，个中计算复杂，这里就不一一介绍，详情看源码
#### 将MyClock设置到桌面
~~~C++
void Setting::on_apply_pushButton_clicked()
{
    if(desktopWnd) //设置父窗体为桌面
    {
        SetParent((HWND)LabWidget->winId(), desktopWnd);
    }
    LabWidget->setWindowFlags(Qt::FramelessWindowHint); //无边框
    LabWidget->showFullScreen();  //全屏
}
~~~
这里要无边框全屏显示，否则显示不全
## 动态壁纸
动态壁纸就是将与桌面建立父子关系的Widget显示视频，这个就需要视频解码，Qt自带的`QMediaPlayer`虽然也能解码，但是视频解码的种类拙荆见肘，我就记的可以解码一个.avi格式的视频
那如果想要显示mp4,WMV等等格式的视频
#### 推荐**libvlc**
![](https://img-blog.csdnimg.cn/2019052313572061.png)
> VLC是一个**免费的开源跨平台多媒体播放器和框架**，可播放大多数多媒体文件以及 DVD，音频CD，VCD和各种流媒体协议。从技术上讲，它是一个软件包，可以处理计算机和网络上的媒体。它提供直观的 API和模块化架构，可轻松添加对新编解码器，容器格式和传输协议的支持。来自 VLC的大多数已知软件包是Windows，Linux和 OSX上常用的播放器。

这里我先给出本期项目的GitHub地址，关于Star，如果有Star的话，这个项目我会继续增加完善功能，上次的NodePad一堆的Bug，没有兴趣去解决，以后无聊了在去解决

DeskGirl
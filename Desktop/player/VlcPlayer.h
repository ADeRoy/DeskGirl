#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include <QObject>
#include <QWidget>
#include "vlc/vlc.h"
#include "vlc/libvlc.h"

class VlcPlayer : public QObject
{
    Q_OBJECT
public:
    explicit VlcPlayer(QObject *parent = nullptr);
signals:
    void signal_media_player_stop();
public:
    bool setURL(QString& URL);
    void media_play();
    void media_pause();
    void media_stop();
    void media_replay();
    void MediaPlayerEncounteredError();
    void* get_media_player_hwnd();
    void set_media_player_hwnd(QWidget* widget);
    void set_media_player_sound(size_t volume);
    libvlc_state_t get_vlc_state();
    libvlc_state_t get_media_player_state();
private:
    //订阅事件
    void attachEvents();
    static void handleEvents(const libvlc_event_t* event, void* userData);
private:
    libvlc_event_manager_t* libvlc_eventManager;	/*事件管理器*/
    libvlc_instance_t* libvlc_inst;					/*vlc播放器实例*/
    libvlc_media_player_t* libvlc_mp;				/*vlc播放器*/
    libvlc_media_t* libvlc_m;						/*音视频文件*/

    libvlc_state_t libvlc_state;					/*播放状态*/
    QWidget* libvlc_mainWindow;                     /*播放器窗口*/
    WId libvlc_WId;                                 /*窗口wid*/
    QString libvlc_url;                             /*播放的URL*/
};

#endif // VLCPLAYER_H

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QThread>
#include <QElapsedTimer>

#include <Phonon/AudioOutput>
#include <Phonon/MediaSource>
#include <Phonon/MediaObject>

extern "C" {
#include "wiimote_api.h"
}

class EventHandler : public QThread
{
    Q_OBJECT
public:
    inline EventHandler(QObject *p = 0, wiimote_t* w = 0)
        : QThread(p) { doStop = false; mote = w; }

    inline void stop() { doStop = true; }

protected:
    void run();

private:
    wiimote_t* mote;
    bool doStop;

    inline bool chk(QElapsedTimer* t)
    { if(t->elapsed() < 700) { return false; } else { t->restart(); return true; } }
};

namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWin(QWidget *parent = 0);
    ~MainWin();

    void changeVol(qreal toAdd);

public slots:
    void on_addBtn_clicked();
    void on_remBtn_clicked();
    void on_upBtn_clicked();
    void on_downBtn_clicked();

    void on_playBtn_clicked();
    void on_pauseBtn_clicked();
    void on_stopBtn_clicked();

    void on_songNxtBtn_clicked();
    void on_songPrevBtn_clicked();

    void on_wiiCnctBtn_clicked();
    void on_wiiDisCnctBtn_clicked();

private slots:
    void on_songList_currentItemChanged(QTreeWidgetItem *current);

private:
    Ui::MainWin *ui;
    wiimote_t mote;
    EventHandler *ev;

    Phonon::AudioOutput *audioOutput;
    Phonon::MediaObject *mediaObject;
};

#endif // MAINWIN_H

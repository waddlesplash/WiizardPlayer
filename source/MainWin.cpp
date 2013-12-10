#include "MainWin.h"
#include "ui_MainWin.h"

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QUrl>

MainWin* mainWin;

#define CHECK if(!chk(&t)){goto done;}
void EventHandler::run()
{
    QElapsedTimer t;
    t.start();
    while(wiimote_is_open(mote)) {
        wiimote_update(mote); // synchronize

        if(mote->keys.home) { mainWin->on_stopBtn_clicked(); }
        if(mote->keys.a) { mainWin->on_playBtn_clicked(); }
        if(mote->keys.b) { mainWin->on_pauseBtn_clicked(); }

        if(mote->keys.left) { CHECK; mainWin->on_songPrevBtn_clicked(); }
        if(mote->keys.right) { CHECK; mainWin->on_songNxtBtn_clicked(); }

        if(mote->keys.plus) { CHECK; mainWin->changeVol(.20); }
        if(mote->keys.minus) { CHECK; mainWin->changeVol(-.20); }
done:
        if(doStop) { return; }
        msleep(10);
    }
}

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWin)
{
    ui->setupUi(this);
    ui->songList->hideColumn(1);
    mainWin = this;

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    ui->seekSlider->setMediaObject(mediaObject);
    ui->volumeSlider->setAudioOutput(audioOutput);
    Phonon::createPath(mediaObject, audioOutput);
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::changeVol(qreal toAdd)
{
    if(audioOutput->volume()+toAdd > 1) { return; }
    audioOutput->setVolume(audioOutput->volume()+toAdd);
}

void MainWin::on_addBtn_clicked()
{
    QStringList s = QFileDialog::getOpenFileNames(this,"Select songs...");
    foreach(QString str,s) {
        QTreeWidgetItem* i = new QTreeWidgetItem(ui->songList);
        QFileInfo f(str);
        i->setText(0,f.fileName());
        i->setText(1,str);
    }
}
void MainWin::on_remBtn_clicked()
{
    QTreeWidgetItem* i = ui->songList->selectedItems().at(0);
    if(i) { i->~QTreeWidgetItem(); }
}
void MainWin::on_upBtn_clicked()
{
    QTreeWidgetItem* i = ui->songList->selectedItems().at(0);
    if(!i) { return; }
    int index = ui->songList->indexOfTopLevelItem(i);
    if(index-1 < 0) { return; }
    ui->songList->takeTopLevelItem(index);
    ui->songList->insertTopLevelItem(index-1,i);
    ui->songList->clearSelection();
    i->setSelected(true);
    on_songList_currentItemChanged(i);
}
void MainWin::on_downBtn_clicked()
{
    QTreeWidgetItem* i = ui->songList->selectedItems().at(0);
    if(!i) { return; }
    int index = ui->songList->indexOfTopLevelItem(i);
    if(index == ui->songList->topLevelItemCount()-1) { return; }
    ui->songList->takeTopLevelItem(index);
    ui->songList->insertTopLevelItem(index+1,i);
    ui->songList->clearSelection();
    i->setSelected(true);
    on_songList_currentItemChanged(i);
}

void MainWin::on_playBtn_clicked()
{
    mediaObject->play();
}
void MainWin::on_pauseBtn_clicked()
{
    mediaObject->pause();
}
void MainWin::on_stopBtn_clicked()
{
    mediaObject->stop();
}

void MainWin::on_songNxtBtn_clicked()
{
    QTreeWidgetItem* i = ui->songList->selectedItems().at(0);
    if(!i) { return; }
    int index = ui->songList->indexOfTopLevelItem(i);
    QTreeWidgetItem* i2 = ui->songList->topLevelItem(index+1);
    if(!i2) { return; }
    i2->setSelected(true);
    i->setSelected(false);
    on_songList_currentItemChanged(i2);
}
void MainWin::on_songPrevBtn_clicked()
{
    QTreeWidgetItem* i = ui->songList->selectedItems().at(0);
    if(!i) { return; }
    int index = ui->songList->indexOfTopLevelItem(i);
    QTreeWidgetItem* i2 = ui->songList->topLevelItem(index-1);
    if(!i2) { return; }
    i2->setSelected(true);
    i->setSelected(false);
    on_songList_currentItemChanged(i2);
}

void MainWin::on_wiiCnctBtn_clicked()
{
    ui->blueAddr->setStyleSheet("");
    if(wiimote_connect(&mote, ui->blueAddr->text().toUtf8()) == WIIMOTE_OK) {
        ui->blueAddr->setStyleSheet("background-color: lime;");
        mote.led.bits = 0;
        mote.rumble = 0;
        ev = new EventHandler(this,&mote);
        ev->start();
    } else {
        ui->blueAddr->setStyleSheet("background-color: red;");
    }
}
void MainWin::on_wiiDisCnctBtn_clicked()
{
    ev->stop();
    ev->wait(100);
    ev->terminate();
    delete ev;
    wiimote_disconnect(&mote);
    ui->blueAddr->setStyleSheet("");
}

void MainWin::on_songList_currentItemChanged(QTreeWidgetItem *current)
{
    QList<QUrl> *q = new QList<QUrl>();
    q->append(QUrl(current->text(1)));
    mediaObject->clear();
    mediaObject->setQueue(*q);
    mote.led.bits = ui->songList->indexOfTopLevelItem(current)+1;
}

#include "dialog.h"
#include "ui_dialog.h"
#include "variables.h"
#include <QUrl>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);

    connect( player , &QMediaPlayer::positionChanged , this , &Dialog::on_position_changed );
    connect( player , &QMediaPlayer::durationChanged , this , &Dialog::on_duration_changed );
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_progress_slider_sliderMoved(int position)
{
   player->setPosition(position);
}

void Dialog::on_volume_slider_sliderMoved(int position)
{
    player->setVolume(position);
}

void Dialog::on_start_button_clicked()
{
    player->setMedia(QUrl::fromLocalFile(variables::audio_file));
    player->play();
    qDebug() << player->errorString();

}

void Dialog::on_stop_button_clicked()
{
   player->stop();
}

void Dialog::on_position_changed( qint64 position )
{
    ui->progress_slider->setValue(position);
}

void Dialog::on_duration_changed( qint64 position )
{
    ui->progress_slider->setMaximum(position);
}

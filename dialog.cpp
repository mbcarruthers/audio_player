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

    // set start and stop buttons to disabled until a .mp3 file is choosen
    ui->stop_button->setEnabled(false);
    ui->start_button->setEnabled(false);
    ui->volume_slider->setValue(50);
    player->setVolume(ui->volume_slider->value());


//    connect( player , &QMediaPlayer::positionChanged , this , &Dialog::on_position_changed );
//    connect( player , &QMediaPlayer::durationChanged , this , &Dialog::on_duration_changed );
    // can work either way. I haven't decided which one to go with yet.
    connect( player , &QMediaPlayer::positionChanged , [this] (qint64 value) {
        on_position_changed(value);
    });
    connect( player , &QMediaPlayer::durationChanged , [this] ( qint64 value )  {
        on_duration_changed(value);
    });
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
    player->setMedia(QUrl::fromLocalFile(file_name));
    player->play();
    qDebug() << player->errorString();
    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(true);

}

void Dialog::on_stop_button_clicked()
{
   player->pause();
   ui->stop_button->setEnabled(false);
   ui->start_button->setEnabled(true);
}

void Dialog::on_position_changed( qint64 position )
{
    ui->progress_slider->setValue(position);
}

void Dialog::on_duration_changed( qint64 position )
{
    ui->progress_slider->setMaximum(position);
}

void Dialog::on_file_button_clicked()
{
    file_name = QFileDialog::getOpenFileName(this,tr("Open File"),variables::path,tr("Images (*.mp3)"));

    if( file_name.isEmpty() )
    {
        qDebug() << "File is empty";
    } else {
        ui->start_button->setEnabled(true);
    }
}

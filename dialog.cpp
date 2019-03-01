#include "dialog.h"
#include "ui_dialog.h"
#include "variables.h"
#include <QUrl>
#include <QMessageBox>
#include <QFileInfo>
//==============================================================================
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


    connect( player , &QMediaPlayer::positionChanged , [this] (qint64 value) {
        on_position_changed(value);
    });
    connect( player , &QMediaPlayer::durationChanged , [this] ( qint64 value )  {
        on_duration_changed(value);
    });
    connect( player , &QMediaObject::metaDataAvailableChanged,[this] () {
        qDebug() << "available meta data has changed";
        qDebug() << player->availableMetaData();
        set_available_metadata();
    });
}
//==============================================================================
Dialog::~Dialog()
{
    delete ui;
}
//==============================================================================
void Dialog::on_progress_slider_sliderMoved(int position)
{
   player->setPosition(position);
}
//==============================================================================
void Dialog::on_volume_slider_sliderMoved(int position)
{
    player->setVolume(position);
}
//==============================================================================
void Dialog::on_start_button_clicked()
{
    player->setMedia( QUrl::fromLocalFile(file_name) );

    player->play();


    if( !player->errorString().isEmpty() ) {
        qDebug() << player->errorString();
        QMessageBox::warning(this,
                             "!",
                             "There was an error playing your file!");
        return;
    }

    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(true);


}
//==============================================================================
void Dialog::on_stop_button_clicked()
{
   player->stop();
   //TODO: set the progress_slider's position to zero
   ui->stop_button->setEnabled(false);
   ui->start_button->setEnabled(true);
}
//==============================================================================
void Dialog::on_position_changed( qint64 position )
{
    ui->progress_slider->setValue(position);
}
//==============================================================================
void Dialog::on_duration_changed( qint64 position )
{
    ui->progress_slider->setMaximum(position);
}
//==============================================================================
void Dialog::on_file_button_clicked()
{
    player->pause();
    file_name = QFileDialog::getOpenFileName(this,tr("Open File"),variables::path,tr("mp3 (*.mp3)"));

    if( file_name.isEmpty() )
    {
        qDebug() << "File is empty";
        if(file.exists()) {
            file_name = file.filePath();
        }
    } else {
       ui->start_button->setEnabled(true);
       file.setFile(file_name);
//       ui->song_info->setText(file.completeBaseName());

    }
}
//==============================================================================
void Dialog::set_available_metadata()
{
    QString song_info{}; //TODO: name that variable better.line 123 might come off as confusing
    if( !player->metaData(QMediaMetaData::ContributingArtist).toString().isNull() )
    {
        qDebug() << player->metaData(QMediaMetaData::ContributingArtist).toString();
        song_info.append(player->metaData(QMediaMetaData::ContributingArtist).toString() + "\n");
    }

    if( !player->metaData(QMediaMetaData::AlbumTitle).toString().isNull() )
    {
        qDebug() << player->metaData(QMediaMetaData::AlbumTitle).toString();
        song_info.append(player->metaData(QMediaMetaData::AlbumTitle).toString()+ "\n");
    }
    if( !player->metaData(QMediaMetaData::Title).toString().isNull() )
    {

        qDebug() << player->metaData(QMediaMetaData::Title).toString();
        song_info.append(player->metaData(QMediaMetaData::Title).toString());
    }

    ui->song_info->setText(song_info);
}
//==============================================================================

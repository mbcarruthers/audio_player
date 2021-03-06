#include "dialog.h"
#include "ui_dialog.h"
#include "variables.h"
#include <QUrl>
#include <QMessageBox>
#include <QFileInfo>
//==============================================================================
// constructor - destructor
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

Dialog::~Dialog()
{
    delete ui;
}
//==============================================================================
// Private Slots
//==============================================================================

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
    QString temp = QFileDialog::getOpenFileName(this,tr("Open File"),variables::path,tr("mp3 (*.mp3 , *.mpeg, *.m4a)"));

    if( temp.isEmpty() && !file_name.isEmpty() )
        {
            ui->start_button->setEnabled(true);
        }
    else if( !temp.isEmpty() ){
        player->stop();
           ui->start_button->setEnabled(true);
           file_name = temp;
           player->setMedia( QUrl::fromLocalFile(file_name) );
        }
    else if( temp.isEmpty() )
    {
        qDebug() << "File is empty";

    }
}
//==============================================================================
// Private Functions
//==============================================================================

void Dialog::set_available_metadata()
{
    QString song_info{}; //TODO: name that variable better.line 129 might come off as confusing
    if( !player->metaData(QMediaMetaData::ContributingArtist).toString().isNull() )
    {
        song_info.append(player->metaData(QMediaMetaData::ContributingArtist).toString() + "\n");
    }

    if( !player->metaData(QMediaMetaData::AlbumTitle).toString().isNull() )
    {
        song_info.append(player->metaData(QMediaMetaData::AlbumTitle).toString()+ "\n");
    }
    if( !player->metaData(QMediaMetaData::Title).toString().isNull() )
    {

        song_info.append(player->metaData(QMediaMetaData::Title).toString());
    }
    // apparently none of my music has album artwork associated with it when it does
    // so i've got this to give me an extra message if something does show up
    if( !player->metaData(QMediaMetaData::CoverArtImage).value<QImage>().isNull() )
    {
        QMessageBox::information(this,
                                 "!",
                                 "This file has an album cover associated with it!");
    }


    ui->song_info->setText(song_info);
}



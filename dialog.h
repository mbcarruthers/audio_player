#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QLabel>
#include <QMediaMetaData>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
private slots:
    void on_progress_slider_sliderMoved( int position );
    void on_volume_slider_sliderMoved( int position );
    void on_start_button_clicked();
    void on_stop_button_clicked();
    void on_duration_changed( qint64 position );
    void on_position_changed( qint64 position );
    void on_file_button_clicked();
private:
    Ui::Dialog *ui;
    QMediaPlayer* player;
    QString file_name{};

    void set_available_metadata();
};

#endif // DIALOG_H

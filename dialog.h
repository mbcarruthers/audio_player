#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QDebug>

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

    void on_durationChanged( qint64 position );

    void on_positionChanged( qint64 position );

private:
    Ui::Dialog *ui;
    QMediaPlayer* player;
};

#endif // DIALOG_H

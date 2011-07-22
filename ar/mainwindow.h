#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QtMultimedia/QAudioOutput>

#include "recordaudio.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void startRecording();
    void stopRecording();
    void lastRecording();

public slots:
    void getFileName(const QString&);

private slots:
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();
    void on_buttonGetLastRecording_clicked();
    void finishedPlaying(QAudio::State);

private:
    Ui::MainWindow *ui;

    RecordAudio raudio;
    QFile inputFile;
    QAudioOutput* paudio;
};

#endif // MAINWINDOW_H

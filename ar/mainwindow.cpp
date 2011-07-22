#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connectors
    connect(this, SIGNAL(startRecording()), &raudio, SLOT(startHistoryRecording()));
    connect(this, SIGNAL(stopRecording()), &raudio, SLOT(stopHistoryRecording()));
    connect(this, SIGNAL(lastRecording()), &raudio, SLOT(provideLastFinishedRecording()));
    connect(&raudio, SIGNAL(lastFinishedRecording(const QString&)), this, SLOT(getFileName(const QString&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getFileName(const QString& filename)
{
    ui->labelText->setText(filename);

    inputFile.setFileName(filename);
    inputFile.open(QIODevice::ReadOnly);

    QAudioFormat format;
    /*
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    */
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    format = info.preferredFormat();
    if (!info.isFormatSupported(format)) {
        return;
    }

    paudio = new QAudioOutput(format, this);
    connect(paudio,SIGNAL(stateChanged(QAudio::State)),this,SLOT(finishedPlaying(QAudio::State)));
    paudio->start(&inputFile);
}

void MainWindow::finishedPlaying(QAudio::State state)
{
    ui->labelText->setText("STATE CHANGE");
    if(state == QAudio::IdleState) {
        ui->labelText->setText("STATE STOP");
        paudio->stop();
        inputFile.close();
        delete paudio;
    }
}

void MainWindow::on_buttonStart_clicked()
{
    emit startRecording();
}

void MainWindow::on_buttonStop_clicked()
{
    emit stopRecording();
}

void MainWindow::on_buttonGetLastRecording_clicked()
{
    emit lastRecording();
}

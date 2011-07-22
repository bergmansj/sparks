#include "recordaudio.h"

#include <iostream>

RecordAudio::RecordAudio(QObject *parent) :
    QObject(parent)
{
    recording_time = 5;
    timer = new QTimer(this);

    // set audio format
    /*
    format.setFrequency(16000);
    format.setChannels(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    */

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    format = info.preferredFormat();
    if (!info.isFormatSupported(format)) {
        std::cout << "default format not supported try to use nearest" << std::endl;
        format = info.nearestFormat(format);
    }

    // connectors
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

RecordAudio::~RecordAudio()
{
    stopHistoryRecording();
}

void RecordAudio::startHistoryRecording()
{
    timer->start(1000);
}

void RecordAudio::stopHistoryRecording()
{
/*
    timer->stop();
    //stop and remove all recordings
    while( audioinputlist.size() > 0 ) {
        QAudioInput ai = audioinputlist.first();
        ai.stop();
        QBuffer ab = audiobufferlist.first();
        ab.close();
        audioinputlist.removeFirst();
        audiobufferlist.removeFirst();
    }
*/
    timer->stop();
    //stop and remove all recordings
    lastaudioname = "";
    while( audioinputlist.size() > 0 ) {
        // remove the first
        QAudioInput *ai = audioinputlist.first();
        QFile *af = audiofilelist.first();
        ai->stop();
        af->close();
        audioinputlist.removeFirst();
        audionameslist.removeFirst();
        audiofilelist.removeFirst();
        delete ai;
        delete af;
    }
}

void RecordAudio::update()
{
/*
    QBuffer audiobuffer;
    audiobuffer.open( QBuffer::WriteOnly | QBuffer::Truncate );
    // Always start new recording and add it to the list
    QAudioInput audioinput = new QAudioInput(format, this);
    audioinput.start(&audiobuffer);
    audioinputlist.append(audioinput);
    audiobufferlist.append(audiobuffer);

    // Check if we need to remove a recording
    // We need to keep recording_time + 1 recording (the +1st recording is the one we need to send at some point)
    if( audioinputlist.size() > recording_time ) {
        // remove the first
        QAudioInput ai = audioinputlist.first();
        ai.stop();
        QBuffer ab = audiobufferlist.first();
        ab.close();
        audioinputlist.removeFirst();
        audiobufferlist.removeFirst();
        // stop the new-first
        QAudioInput ai2 = audioinputlist.first();
        ai2.stop();

    }
*/
    QFile *outputFile = new QFile();
    // Prepare file
    QDateTime now = QDateTime::currentDateTime();
    QString id = now.toString("yyyy-MM-dd_hh-mm-ss");
    QString filename = "./" + id + ".raw";
    outputFile->setFileName(filename);
    outputFile->open( QIODevice::WriteOnly | QIODevice::Truncate );
    // Always start new recording and add it to the list
    QAudioInput *audioinput = new QAudioInput(format, this);
    audioinput->start(outputFile);
    // Store in the lists
    audioinputlist.append(audioinput);
    audionameslist.append(filename);
    audiofilelist.append(outputFile);

    // Check if we need to remove a recording
    if( audioinputlist.size() >= recording_time ) {
        // remove the first
        QAudioInput *ai = audioinputlist.first();
        lastaudioname = audionameslist.first();
        QFile *af = audiofilelist.first();
        ai->stop();
        af->close();
        audioinputlist.removeFirst();
        audionameslist.removeFirst();
        audiofilelist.removeFirst();
    }
}


void RecordAudio::provideLastFinishedRecording()
{
/*
    // Check if recording is available
    if( audioinputlist.size() >= recording_time ) {
        // Prepare file
        QDateTime now = QDateTime::currentDateTime();
        QString id = now.toString("yyyy-MM-dd_hh-mm-ss");
        QString filename = "./" + id + ".raw";
        QFile outputFile;
        outputFile.setFileName(filename);
        outputFile.open( QIODevice::WriteOnly | QIODevice::Truncate );
        QAudioInput ai = audioinputlist.first();
        ai.stop();
        QBuffer ab = audiobufferlist.first();

        ab.seek(0);
        QByteArray ba;
        ba = ab.readAll();
        ai.
        ai.

        pixmap.save(&file, "PNG");


        ab.close();
        emit lastFinishedRecording(filename);
    }
*/
    emit lastFinishedRecording(lastaudioname);
}

#ifndef RECORDAUDIO_H
#define RECORDAUDIO_H

#include <QObject>
#include <QBuffer>
#include <QtMultimedia/QAudioInput>
#include <QTimer>
#include <QList>

#include <QFile>
#include <QDateTime>
#include <QByteArray>

class RecordAudio : public QObject
{
    Q_OBJECT
public:
    explicit RecordAudio(QObject *parent = 0);
    virtual ~RecordAudio();

signals:
    void lastFinishedRecording(const QString&);

public slots:
    void startHistoryRecording();
    void stopHistoryRecording();
    void provideLastFinishedRecording();

    void update();

private:
    int recording_time;

    QTimer *timer;
    //QList<QBuffer> audiobufferlist;
    QString lastaudioname;
    QList<QString> audionameslist;
    QList<QFile*> audiofilelist;
    QList<QAudioInput*> audioinputlist;

    QAudioFormat format;

};

#endif // RECORDAUDIO_H

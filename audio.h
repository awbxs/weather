#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include "QDebug"
#include <QAudioDeviceInfo>
#include <QMessageBox>
#include <QFile>
#include <QAudioFormat>
#include <QAudioInput>
class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(QObject *parent = nullptr);

    void startAudio(QString filename);

    void stopAudio();

signals:

public slots:

private:
    QAudioInput *m_audio;
    QFile *m_file;
};

#endif // AUDIO_H

#ifndef AUDIOOUT_H
#define AUDIOOUT_H

#include <QAudioSink>
#include <QByteArray>
#include <QComboBox>
#include <QIODevice>
#include <QLabel>
#include <QMainWindow>
#include <QMediaDevices>
#include <QObject>
#include <QPushButton>
#include <QScopedPointer>
#include <QSlider>
#include <QTimer>




class AudioOut : public QObject{
    Q_OBJECT
public:
    AudioOut();
    ~AudioOut(){}

    void initAudio(QAudioDevice&);
    void set_level(int);
    void change_device(int);
    void play(QByteArray &buffer);
    void start();
    void stop();

signals:


private:
    bool mute = false;

    QIODevice* io;
    QAudioFormat m_format;

    QScopedPointer<QAudioSink> m_audioOutput;
    QMediaDevices *m_devices = nullptr;
};

#endif // AUDIOOUT_H

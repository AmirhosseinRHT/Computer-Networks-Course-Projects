#ifndef AUDIO_H
#define AUDIO_H

#include <QAudioSource>
#include <QMediaDevices>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

#include <QPixmap>

#include <QByteArray>
#include <QScopedPointer>
#include<iostream>

#include"AudioOut.h"
#include"WebRTCAnswerer.h"
#include"webRtcOferrer.h"

class AudioInfo : public QIODevice
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat &format);

    void start();
    void stop();
    qreal level() const { return m_level; }
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    qreal calculateLevel(const char *data, qint64 len) const;

signals:
    void levelChanged(qreal level);

private:
    const QAudioFormat m_format;
    qreal m_level = 0.0;
};

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = nullptr);

    void setLevel(qreal value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    qreal m_level = 0;
};

class InputTest : public QWidget
{
    Q_OBJECT

public:
    InputTest();
    ~InputTest(){if(web1)web1->close();
        if(web2)web2->close();
    }

private:
    void initializeWindow();
    void initializeAudio(const QAudioDevice &deviceInfo);
    void initializeErrorWindow();

private slots:
    void init();
    void toggleMode();
    void toggleSuspend();
    void deviceChanged(int index);
    void deviceOutputChanged(int index);
    void sliderChanged(int value);
    void toggleSpeaker();
    void chooseCallMode1();
    void chooseCallMode2();


private:
    // Owned by layout
    RenderArea *m_canvas = nullptr;
    QPushButton *m_modeButton = nullptr;
    QPushButton *m_suspendResumeButton = nullptr;
    QPushButton *m_muteSpeakerButton = nullptr;
    QComboBox *m_deviceBox = nullptr;
    QComboBox *m_deviceBox2 = nullptr;
    QSlider *m_volumeSlider = nullptr;
    QPushButton *m_modeCallButton1 = nullptr;
    QPushButton *m_modeCallButton2 = nullptr;
    QLineEdit *inputIp;
    WebRTCClientAnswerer *web1=nullptr;
    WebRTCClientOferrer *web2 = nullptr;
    AudioOut *m_audioOutput;
    QMediaDevices *m_devices = nullptr;
    QScopedPointer<AudioInfo> m_audioInfo;
    QScopedPointer<QAudioSource> m_audioInput;
    bool m_pullMode = true;
    bool is_mutespeaker = true;
    int call_mode = 0;
};







#endif // AUDIO_H

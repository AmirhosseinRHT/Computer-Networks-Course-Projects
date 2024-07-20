#include "AudioOut.h"

#include <QAudioDevice>
#include <QAudioSink>
#include <QDebug>
#include <QVBoxLayout>
#include <QtEndian>
#include <QtMath>



AudioOut::AudioOut() {
    m_devices = new QMediaDevices{this};

    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(QAudioFormat::Int16);

    m_audioOutput.reset( new QAudioSink{m_devices->defaultAudioOutput(),
                                       m_format,nullptr});

    mute = 0;
    //io = m_audioOutput->start();
    // if(!io->open(QIODevice::WriteOnly))
    // {
    //     qDebug() << "Couldn't open AudioSink!";
    // }
    // connect(this,&AudioSink::checkAudioDataReceived,[&,io]()
    //         {
    //             audioSinkData(io);
    //         });

}

void AudioOut::start(){
    mute = false;
    m_audioOutput.reset( new QAudioSink{m_devices->defaultAudioOutput(),
                                       m_format,nullptr});
    io = m_audioOutput->start();
}


void AudioOut::stop(){
    mute = true;
    m_audioOutput->stop();
}

void AudioOut::change_device(int index)
{

    m_audioOutput->stop();
    //m_audioOutput->disconnect(this);
    //initializeAudio(m_deviceBox->itemData(index).value<QAudioDevice>());
}

void AudioOut::initAudio(QAudioDevice &a_device){
    m_format = a_device.preferredFormat();
    m_audioOutput.reset(new QAudioSink(a_device, m_format));
    io = m_audioOutput->start();
}


void AudioOut::play(QByteArray &buffer){

    int len = buffer.size();
    if(len && (!mute))
        io->write(buffer.constData() , len);
}

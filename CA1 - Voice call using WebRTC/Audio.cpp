#include "Audio.h"
#include <QAudioDevice>
#include <QAudioSource>
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QtEndian>
#include <QLineEdit>
#if QT_CONFIG(permissions)
#include <QCoreApplication>
#include <QPermission>
#endif

#include <math.h>
#include <stdlib.h>
int debug_number = 0;
AudioInfo::AudioInfo(const QAudioFormat &format) : m_format(format) { }

void AudioInfo::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
    close();
}

qint64 AudioInfo::readData(char * /* data */, qint64 /* maxlen */)
{
    return 0;
}

qreal AudioInfo::calculateLevel(const char *data, qint64 len) const
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = len / sampleBytes;

    float maxValue = 0;
    auto *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < m_format.channelCount(); ++j) {
            float value = m_format.normalizedSampleValue(ptr);
            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
        }
    }
    return maxValue;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
    m_level = calculateLevel(data, len);
    emit levelChanged(m_level);
    return len;
}

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMinimumHeight(30);
    setMinimumWidth(200);
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    const QRect frame = painter.viewport() - QMargins(10, 10, 10, 10);
    painter.drawRect(frame);
    if (m_level == 0.0)
        return;
    const int pos = qRound(qreal(frame.width() - 1) * m_level);
    painter.fillRect(frame.left() + 1, frame.top() + 1, pos, frame.height() - 1, Qt::red);
}

void RenderArea::setLevel(qreal value)
{
    m_level = value;
    update();
}

InputTest::InputTest() : m_devices(new QMediaDevices(this))
{
    init();
}

void InputTest::initializeWindow()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_modeCallButton1 = new QPushButton(this);
    connect(m_modeCallButton1, &QPushButton::clicked, this, &InputTest::chooseCallMode1);
    layout->addWidget(m_modeCallButton1);

    m_modeCallButton2 = new QPushButton(this);
    m_modeCallButton1->setText("Call");
    connect(m_modeCallButton2, &QPushButton::clicked, this, &InputTest::chooseCallMode2);
    layout->addWidget(m_modeCallButton2);
    m_modeCallButton2->setText("Wait for Call");
    m_canvas = new RenderArea(this);
    layout->addWidget(m_canvas);
    inputIp = new QLineEdit;
    inputIp->setPlaceholderText("IP");
    layout->addWidget(inputIp);

    m_deviceBox = new QComboBox(this);
    const QAudioDevice &defaultDeviceInfo = QMediaDevices::defaultAudioInput();
    m_deviceBox->addItem(defaultDeviceInfo.description(), QVariant::fromValue(defaultDeviceInfo));
    for (auto &deviceInfo : m_devices->audioInputs()) {
        if (deviceInfo != defaultDeviceInfo)
            m_deviceBox->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }

    connect(m_deviceBox, &QComboBox::activated, this, &InputTest::deviceChanged);
    layout->addWidget(m_deviceBox);

    m_deviceBox2 = new QComboBox(this);
    const QAudioDevice &defaultDeviceInfo2 = m_devices->defaultAudioOutput();
    m_deviceBox2->addItem(defaultDeviceInfo2.description(), QVariant::fromValue(defaultDeviceInfo2));
    for (auto &deviceInfo : m_devices->audioOutputs()) {
        if (deviceInfo != defaultDeviceInfo2)
            m_deviceBox2->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
    }
    connect(m_deviceBox2, QOverload<int>::of(&QComboBox::activated), this,
            &InputTest::deviceOutputChanged);
    layout->addWidget(m_deviceBox2);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(100);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &InputTest::sliderChanged);
    layout->addWidget(m_volumeSlider);

    m_modeButton = new QPushButton(this);
    connect(m_modeButton, &QPushButton::clicked, this, &InputTest::toggleMode);
    layout->addWidget(m_modeButton);

    m_suspendResumeButton = new QPushButton(this);
    connect(m_suspendResumeButton, &QPushButton::clicked, this, &InputTest::toggleSuspend);
    layout->addWidget(m_suspendResumeButton);

    m_muteSpeakerButton = new QPushButton(this);
    connect(m_muteSpeakerButton, &QPushButton::clicked, this, &InputTest::toggleSpeaker);
    layout->addWidget(m_muteSpeakerButton);
}

void InputTest::initializeAudio(const QAudioDevice &deviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    m_audioOutput = new AudioOut();
    m_audioInfo.reset(new AudioInfo(format));
    connect(m_audioInfo.data(), &AudioInfo::levelChanged, m_canvas, &RenderArea::setLevel);
    m_audioInput.reset(new QAudioSource(deviceInfo, format));
    qreal initialVolume = QAudio::convertVolume(m_audioInput->volume(), QAudio::LinearVolumeScale,
                                                QAudio::LogarithmicVolumeScale);
    m_volumeSlider->setValue(qRound(initialVolume * 100));
    m_audioInfo->start();
    toggleMode();
    toggleSpeaker();
}

void InputTest::initializeErrorWindow()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *errorLabel = new QLabel(tr("Microphone permission is not granted!"));
    errorLabel->setWordWrap(true);
    errorLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(errorLabel);
}

void InputTest::init()
{
#if QT_CONFIG(permissions)
    QMicrophonePermission microphonePermission;
    switch (qApp->checkPermission(microphonePermission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(microphonePermission, this, &InputTest::init);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning("Microphone permission is not granted!");
        initializeErrorWindow();
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
#endif
    initializeWindow();
    initializeAudio(QMediaDevices::defaultAudioInput());
}
int h=0;

void InputTest::toggleMode()
{
    m_audioInput->stop();
    toggleSuspend();
    if (m_pullMode) {
        m_modeButton->setText(tr("Unmute mic"));

        m_audioInput->start(m_audioInfo.data());
    } else {
        m_modeButton->setText(tr("Mute mic"));
        auto *io = m_audioInput->start();
        if (!io)
            return;

        connect(io, &QIODevice::readyRead, [this, io]() {
            static const qint64 BufferSize = 2048 * 4 ;
            const qint64 len = qMin(m_audioInput->bytesAvailable(), BufferSize);

            QByteArray buffer(len, 0);
            qint64 l = io->read(buffer.data(), len);
            if (l > 0) {
                const qreal level = m_audioInfo->calculateLevel(buffer.constData(), l);
                if(call_mode == 1 && web1){
                    web1->sendMessage(buffer.data());
                }else if(call_mode == 2 && web2){
                    web2->sendMessage(buffer.data());
                }
                m_canvas->setLevel(level);
            }
        });
    }

    m_pullMode = !m_pullMode;
}

void InputTest::toggleSuspend()
{
    switch (m_audioInput->state()) {
    case QAudio::SuspendedState:
    case QAudio::StoppedState:
        m_audioInput->resume();
        m_suspendResumeButton->setText(tr("Suspend recording"));
        break;
    case QAudio::ActiveState:
        m_audioInput->suspend();
        m_suspendResumeButton->setText(tr("Resume recording"));
        break;
    case QAudio::IdleState:
        break;
    }
}

void InputTest::toggleSpeaker()
{
    if(is_mutespeaker){
        m_audioOutput->start();
        m_muteSpeakerButton->setText(tr("mute Speaker"));
    }
    else{
        m_audioOutput->stop();
        m_muteSpeakerButton->setText(tr("unmute Speaker"));
    }
    is_mutespeaker = !is_mutespeaker;
}

void InputTest::deviceChanged(int index)
{
    m_audioInfo->stop();
    m_audioInput->stop();
    m_audioInput->disconnect(this);

    initializeAudio(m_deviceBox->itemData(index).value<QAudioDevice>());
}

void InputTest::deviceOutputChanged(int index)
{

    //std::cout << "yes" << std::endl;
    // m_audioOutput->stop();
    // m_audioOutput->disconnect(this);
    // initializeAudio(m_deviceBox->itemData(index).value<QAudioDevice>());
}

void InputTest::sliderChanged(int value)
{
    qreal linearVolume = QAudio::convertVolume(value / qreal(100), QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    m_audioInput->setVolume(linearVolume);
}

void InputTest::chooseCallMode1(){
    if(call_mode == 0){
        auto p = inputIp->text().toStdString();
        call_mode = 1;
        web1 = new WebRTCClientAnswerer(p);
        web1->setAudioOut(m_audioOutput);
        web1->start();
    }
}

void InputTest::chooseCallMode2(){
    if(call_mode == 0){
        std::cout << "set wait call" << std::endl;
        auto p = inputIp->text().toStdString();
        call_mode = 2;
        web2 = new WebRTCClientOferrer(p);
        web2->setAudioOut(m_audioOutput);
        web2->start();
    }
}

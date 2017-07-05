#include "c_audio.h"
//#include <QAudio>
#include <QDebug>

c_audio::c_audio(QObject *parent):
    QObject(parent)
  , m_audioInput(0)
  , m_series(0)
//, m_device(0)
{
    qDebug()<<"audio init - input devices";
    //for( const auto &d:QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) qDebug()<<d.deviceName();
    //qDebug()<<"default input:"<<m_device.defaultInputDevice().deviceName();
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    m_audioInput = new QAudioInput (format, this);
//    m_audioQIO   =m_audioInput->start();
//    CHECKED_CONNECT(m_audioQIO, SIGNAL(readyRead()),
//                    this, SLOT(audioDataReady()));
    qDebug()<<"m_audioInput::error -> "<<m_audioInput->error();
    qDebug()<<"m_audioInput::state -> "<<m_audioInput->state();

}

void c_audio::audioDataReady()
{
    //Q_ASSERT(0 == m_bufferPosition);
//    const qint64 bytesReady = m_audioInput->bytesReady();
//    const qint64 bytesSpace = m_buffer.size() - m_dataLength;
//    const qint64 bytesToRead = qMin(bytesReady, bytesSpace);
    const qint64 bytesReady=m_audioInput->bytesReady();
    m_buffer.clear();
    m_buffer.resize(bytesReady);
    const qint64 bytesRead = m_device->read(
                                       m_buffer.data(),
                                       bytesReady);

    if (bytesRead) {
       qDebug()<<m_buffer.toHex();
    }



}

c_audio::~c_audio()
{
//    if (m_audioInput) delete m_audioInput;
//    if (m_device) delete m_device;

}

QStringList c_audio::availableDevices()
{
    QStringList sl;
    for( const auto &d:QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        //qDebug()<<"device name:"<<d.deviceName();
        sl.append(d.deviceName());
    }
    return sl;
}

QLineSeries *c_audio::freq() const
{
    return m_freq;
}

void c_audio::setFreq(QLineSeries *freq)
{
    m_freq = freq;
}






QLineSeries *c_audio::series() const
{
    return m_series;
}

void c_audio::setSeries(QLineSeries *series)
{
    m_series = series;
    if (m_series&&m_device) delete m_device;

    m_device = new XYSeriesIODevice(m_series, this);
    m_device->open(QIODevice::WriteOnly);

    m_audioInput->start(m_device);
}


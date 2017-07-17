#include "c_audio.h"
//#include <QAudio>
#include <QDebug>
#include <QValueAxis>

c_audio::c_audio(QObject *parent):
    QObject(parent)
  , m_audioInput(0)
  , m_device(0)
  , m_series(0)
  , m_sample_size(16)
  , m_sample_rate(16000)

  , m_spectrumAnalyser()

{
    qDebug()<<"audio init - input devices";
    //for( const auto &d:QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) qDebug()<<d.deviceName();

    setCurrnet_device(QAudioDeviceInfo::defaultInputDevice().deviceName());
    m_spectrumAnalyser.setWindowFunction(WindowFunction::HannWindow);
    CHECKED_CONNECT(&m_spectrumAnalyser,
                    SIGNAL(spectrumChanged(FrequencySpectrum)),
                    this,
                    SLOT(spectrumChanged(FrequencySpectrum)));

//    m_audioQIO   =m_audioInput->start();
//    CHECKED_CONNECT(m_audioQIO, SIGNAL(readyRead()),
//                    this, SLOT(audioDataReady()));


}

//void c_audio::audioDataReady()  // to delete
//{
//    //Q_ASSERT(0 == m_bufferPosition);
////    const qint64 bytesReady = m_audioInput->bytesReady();
////    const qint64 bytesSpace = m_buffer.size() - m_dataLength;
////    const qint64 bytesToRead = qMin(bytesReady, bytesSpace);
//    const qint64 bytesReady=m_audioInput->bytesReady();
//    m_buffer.clear();
//    m_buffer.resize(bytesReady);
//    const qint64 bytesRead = m_device->read(
//                                       m_buffer.data(),
//                                       bytesReady);

//    if (bytesRead) {
//       qDebug()<<m_buffer.toHex();
//    }
//}

void c_audio::start()
{
    if(!m_series) {
        qDebug()<<"chart serie not initialized in qml";
        return;
    }
    m_format.setChannelCount(1);
    m_format.setSampleSize(m_sample_size);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setSampleRate(m_sample_rate);
    if (!m_currnet_device_info.isFormatSupported(m_format)) {
        m_format = m_currnet_device_info.nearestFormat(m_format);
        setSample_rate(m_format.sampleRate());
        setSample_size(m_format.sampleSize());
    }
    qDebug()<<"m_format.samplesize:"<<m_format.sampleSize()
          <<"\nm_format.samplerate:"<<m_format.sampleRate()
          <<"\nm_format.byteorder:"<<m_format.byteOrder()
          <<"\nm_format.sampletype:"<<m_format.sampleType()
          <<"\nm_format.codec"<<m_format.codec()
          <<"\nm_format.channels"<<m_format.channelCount()
                ;
    qDebug()<<"m_device:"<<m_device
            <<"\nm_audioInput:"<<m_audioInput
           <<"\ndevname:"<<m_currnet_device_info.deviceName();
    if (m_series&&m_device) { delete m_device; m_device=0;}
    if (m_audioInput) {delete m_audioInput; m_audioInput=0;}
    m_device = new XYSeriesIODevice(m_series, m_format.sampleSize()/8, this);
    m_device->open(QIODevice::WriteOnly);

    m_audioInput = new QAudioInput (m_currnet_device_info, m_format, this);
    m_audioInput->setNotifyInterval(100); // Notify interval 100 ms
    m_audioInput->start(m_device);
    qDebug()<<"m_audioInput::error -> "<<m_audioInput->error();
    qDebug()<<"m_audioInput::state -> "<<m_audioInput->state();

    //============
    m_spectrumAnalyser.cancelCalculation();
    spectrumChanged(0, 0, FrequencySpectrum());
    CHECKED_CONNECT(m_audioInput, SIGNAL(notify()),
                    this, SLOT(audioNotify()));
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

QAudioDeviceInfo c_audio::getDeviceByName(QString name)
{
        for( const auto &d:QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
            if(d.deviceName() == name) {
                return d;
            }
        return QAudioDeviceInfo::defaultInputDevice();
}

qint32 c_audio::sample_rate() const
{
    return m_sample_rate;
}

void c_audio::setSample_rate(const qint32 &sample_rate)
{
    m_sample_rate = sample_rate;
    emit sample_rateChanged();
}

qint16 c_audio::sample_size() const
{
    return m_sample_size;
}

void c_audio::setSample_size(const qint16 &sample_size)
{
    m_sample_size = sample_size;
    emit sample_sizeChanged();
}

QString c_audio::currnet_device() const
{
    return m_currnet_device;
}

void c_audio::setCurrnet_device(const QString &currnet_device)
{
    m_currnet_device_info = getDeviceByName(currnet_device);
    m_currnet_device=m_currnet_device_info.deviceName();
    emit current_deviceChanged();
    qDebug()<<"Current Device:"<<m_currnet_device_info.deviceName()
            <<"\nDevice:"<<m_currnet_device_info.isNull();


    start();
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
    start();


}

void c_audio::spectrumChanged(const FrequencySpectrum &spectrum)
{
    ENGINE_DEBUG << "Engine::spectrumChanged" << "pos" << m_spectrumPosition;
    emit spectrumChanged(m_spectrumPosition, m_spectrumBufferLength, spectrum);
}

void c_audio::audioNotify()
{
//    const qint64 recordPosition = qMin(m_bufferLength, audioLength(m_format, m_audioInput->processedUSecs()));
//    setRecordPosition(recordPosition);
//    const qint64 levelPosition = m_dataLength - m_levelBufferLength;
//    if (levelPosition >= 0)
//        calculateLevel(levelPosition, m_levelBufferLength);
//    if (m_dataLength >= m_spectrumBufferLength) {
//        const qint64 spectrumPosition = m_dataLength - m_spectrumBufferLength;
//        calculateSpectrum(spectrumPosition);
//    }
//    emit bufferChanged(0, m_dataLength, m_buffer);
//    collect N samples, where N is a power of 2
//    apply a suitable window function to the samples, e.g. Hanning
//    pass the windowed samples to an FFT routine - ideally you want a real-to-complex FFT but if all you have a is complex-to-complex FFT then pass 0 for all the imaginary input parts
//    calculate the squared magnitude of your FFT output bins (re * re + im * im)
//    (optional) calculate 10 * log10 of each magnitude squared output bin to get a magnitude value in dB
//    Now that you have your power spectrum you just need to identify the peak(s), which should be pretty
//            straightforward if you have a reasonable S/N ratio. Note that frequency resolution improves with larger N.
//            For the above example of 44.1 kHz sample rate and N = 32768 the frequency resolution of each bin is 44100 / 32768 = 1.35 Hz.
}

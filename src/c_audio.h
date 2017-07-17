#ifndef C_AUDIO_H
#define C_AUDIO_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QQmlListProperty>
#include <qstringlist.h>
#include <qaudioinput.h>
#include <xyseriesiodevice.h>
//#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <frequencyspectrum.h>
#include <spectrumanalyser.h>


//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// Macro which connects a signal to a slot, and which causes application to
// abort if the connection fails.  This is intended to catch programming errors
// such as mis-typing a signal or slot name.  It is necessary to write our own
// macro to do this - the following idiom
//     Q_ASSERT(connect(source, signal, receiver, slot));
// will not work because Q_ASSERT compiles to a no-op in release builds.

#define CHECKED_CONNECT(source, signal, receiver, slot) \
    if (!connect(source, signal, receiver, slot)) \
        qt_assert_x(Q_FUNC_INFO, "CHECKED_CONNECT failed", __FILE__, __LINE__);

class c_audio: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableDevices READ availableDevices CONSTANT)
    Q_PROPERTY(QLineSeries* series READ series WRITE setSeries NOTIFY seriesChanged)
    Q_PROPERTY(QLineSeries* freq READ freq WRITE setFreq NOTIFY freqChanged)
    Q_PROPERTY(qint32 sample_rate READ sample_rate WRITE setSample_rate NOTIFY sample_rateChanged)
    Q_PROPERTY(qint32 sample_size READ sample_size WRITE setSample_size NOTIFY sample_sizeChanged)
    Q_PROPERTY(QString currnet_device READ currnet_device() WRITE setCurrnet_device NOTIFY current_deviceChanged)

public:
    explicit c_audio(QObject *parent = 0);
    virtual ~c_audio();
    QLineSeries *series() const;
    void setSeries(QLineSeries *series);


    QLineSeries *freq() const;
    void setFreq(QLineSeries *freq);

    QString currnet_device() const;
    void setCurrnet_device(const QString &currnet_device);

    qint16 sample_size() const;
    void setSample_size(const qint16 &sample_size);

    qint32 sample_rate() const;
    void setSample_rate(const qint32 &sample_rate);

private slots:
//    void audioDataReady();  //to delete
    void start();
    void spectrumChanged(const FrequencySpectrum &spectrum);
    void audioNotify();

signals:
    void chartChanged();
    void seriesChanged();
    void freqChanged();
    void current_deviceChanged();
    void sample_rateChanged();
    void sample_sizeChanged();
    /**
     * Spectrum has changed.
     * \param position Position of start of window in bytes
     * \param length   Length of window in bytes
     * \param spectrum Resulting frequency spectrum
     */
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);
private:
    QStringList  availableDevices();
    QString m_currnet_device;
    QAudioDeviceInfo getDeviceByName(QString name);
    QAudioDeviceInfo m_currnet_device_info;
    //=============================
    QAudioInput* m_audioInput;
//    QIODevice *  m_audioQIO;
//    QByteArray   m_buffer;


    XYSeriesIODevice *m_device=0;
    QLineSeries *m_series;
    qint16 m_sample_size;
    qint32 m_sample_rate;
//============== spectr ===============
    QLineSeries*        m_freq;
    FrequencySpectrum   m_freq_spectrum;
    QAudioFormat        m_format;
    int                 m_spectrumBufferLength;
    QByteArray          m_spectrumBuffer;
    SpectrumAnalyser    m_spectrumAnalyser;
    qint64              m_spectrumPosition;
    void calculateSpectrum(qint64 position);

};

#endif // C_AUDIO_H

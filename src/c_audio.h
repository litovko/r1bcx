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

public:
    explicit c_audio(QObject *parent = 0);
    virtual ~c_audio();
    QLineSeries *series() const;
    void setSeries(QLineSeries *series);


    QLineSeries *freq() const;
    void setFreq(QLineSeries *freq);

private slots:
    void audioDataReady();
signals:
    void chartChanged();
    void seriesChanged();
    void freqChanged();
private:
    QStringList  availableDevices();
    //=============================
    QAudioInput* m_audioInput;
//    QIODevice *  m_audioQIO;
    QByteArray   m_buffer;

    XYSeriesIODevice *m_device=0;
    QLineSeries *m_series;
//============== spectr ===============
    QLineSeries *m_freq;
    FrequencySpectrum m_freq_spectrum;
    QAudioFormat        m_format;
    SpectrumAnalyser    m_spectrumAnalyser;

};

#endif // C_AUDIO_H

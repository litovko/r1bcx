#ifndef C_AUDIO_INPUT_H
#define C_AUDIO_INPUT_H

#include <QObject>

class c_audio_input : public QObject
{
    Q_OBJECT
public:
    explicit c_audio_input(QObject *parent = nullptr);

signals:

public slots:
};

#endif // C_AUDIO_INPUT_H
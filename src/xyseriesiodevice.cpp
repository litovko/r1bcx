/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "xyseriesiodevice.h"
#include <QtCharts/QXYSeries>
#include <QDebug>
#include <QValueAxis>

XYSeriesIODevice::XYSeriesIODevice(QXYSeries * series, qint16 samplesize, QObject *parent) :
    QIODevice(parent),
    m_series(series),
    m_range(2000),
    m_samplesize(samplesize)

{
    QAbstractAxis* ax;
    foreach (QAbstractAxis* axis, m_series->attachedAxes()) {
        //qDebug()<<axis->orientation(); //Qt::Orientation(Horizontal)
        if (axis->orientation()==Qt::Horizontal) ax=axis;
    }
    if (qobject_cast<QValueAxis*>(ax)) m_range=qobject_cast<QValueAxis*>(ax)->max();
    qDebug()<<"SampleSize:"<<m_samplesize
            <<"\nRange:"<<m_range;

}

qint64 XYSeriesIODevice::readData(char * data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 XYSeriesIODevice::writeData(const char * data, qint64 maxSize)
{

    QVector<QPointF> oldPoints = m_series->pointsVector();
    QVector<QPointF> points;
//    qint16* ptr= static_cast<qint16*>(static_cast<void*>(data));
    qint64 dsize=maxSize/m_samplesize;

    if (oldPoints.count() < m_range) {
        points = m_series->pointsVector();
    } else {
        for (int i = maxSize/m_samplesize; i < oldPoints.count(); i++)
            points.append(QPointF(i - maxSize/m_samplesize, oldPoints.at(i).y()));
    }

    qint64 size = points.count();
    for (int k = 0; k < dsize; k++){
        //buf[0] | buf[1] << 8  two bytes to integer

        points.append(QPointF(k + size,
                          m_samplesize==2?(qint16)(((quint8)data[m_samplesize * k]|(quint8)data[m_samplesize * k+1]<<8))/32768.0:
//                        //m_samplesize==2?((data[m_samplesize * k]|data[m_samplesize * k+1]<<8))/32768.0:
                          ((quint8)data[k]-128)/128.0)
                     );
}

    m_series->replace(points);
//    qDebug()<<"NP:"<<m_series->count()
//           <<"\nSamples:"<<dsize;
    return maxSize;
}

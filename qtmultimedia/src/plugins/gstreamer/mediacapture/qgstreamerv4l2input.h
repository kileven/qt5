/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QGSTREAMERV4L2INPUT_H
#define QGSTREAMERV4L2INPUT_H

#include <QtCore/qhash.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtCore/qsize.h>
#include "qgstreamercapturesession.h"

QT_BEGIN_NAMESPACE

class QGstreamerV4L2Input : public QObject, public QGstreamerVideoInput
{
    Q_OBJECT
public:
    QGstreamerV4L2Input(QObject *parent = 0);
    virtual ~QGstreamerV4L2Input();

    GstElement *buildElement();

    QList<qreal> supportedFrameRates(const QSize &frameSize = QSize()) const;
    QList<QSize> supportedResolutions(qreal frameRate = -1) const;

    QByteArray device() const;

public slots:
    void setDevice(const QByteArray &device);
    void setDevice(const QString &device);

private:
    void updateSupportedResolutions(const QByteArray &device);

    QList<qreal> m_frameRates;
    QList<QSize> m_resolutions;

    QHash<QSize, QSet<int> > m_ratesByResolution;

    QByteArray m_device;
};

QT_END_NAMESPACE

#endif // QGSTREAMERV4L2INPUT_H
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

#include <QtCore/qdebug.h>

#include "dscameracontrol.h"
#include "dscameraservice.h"
#include "dscamerasession.h"

QT_BEGIN_NAMESPACE

DSCameraControl::DSCameraControl(QObject *parent)
    :QCameraControl(parent), m_captureMode(QCamera::CaptureStillImage)
{
    m_session = qobject_cast<DSCameraSession*>(parent);
    connect(m_session, SIGNAL(stateChanged(QCamera::State)),this, SIGNAL(stateChanged(QCamera::State)));
}

DSCameraControl::~DSCameraControl()
{
}

void DSCameraControl::setState(QCamera::State state)
{
    switch (state) {
        case QCamera::ActiveState:
            start();
            break;
        case QCamera::UnloadedState: /* fall through */
        case QCamera::LoadedState:
            stop();
            break;
    }
}

bool DSCameraControl::isCaptureModeSupported(QCamera::CaptureModes mode) const
{
    bool bCaptureSupported = false;
    switch (mode) {
    case QCamera::CaptureStillImage:
        bCaptureSupported = true;
        break;
    case QCamera::CaptureVideo:
        bCaptureSupported = false;
        break;
    }
    return bCaptureSupported;
}

void DSCameraControl::start()
{
    m_session->record();
}

void DSCameraControl::stop()
{
    m_session->stop();
}

QCamera::State DSCameraControl::state() const
{
    return (QCamera::State)m_session->state();
}

QT_END_NAMESPACE
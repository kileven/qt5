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


#include "qaudio_mac_p.h"

QT_BEGIN_NAMESPACE

// Conversion
QAudioFormat toQAudioFormat(AudioStreamBasicDescription const& sf)
{
    QAudioFormat    audioFormat;

    audioFormat.setSampleRate(sf.mSampleRate);
    audioFormat.setChannelCount(sf.mChannelsPerFrame);
    audioFormat.setSampleSize(sf.mBitsPerChannel);
    audioFormat.setCodec(QString::fromLatin1("audio/pcm"));
    audioFormat.setByteOrder((sf.mFormatFlags & kAudioFormatFlagIsBigEndian) != 0 ? QAudioFormat::BigEndian : QAudioFormat::LittleEndian);
    QAudioFormat::SampleType type = QAudioFormat::UnSignedInt;
    if ((sf.mFormatFlags & kAudioFormatFlagIsSignedInteger) != 0)
        type = QAudioFormat::SignedInt;
    else if ((sf.mFormatFlags & kAudioFormatFlagIsFloat) != 0)
        type = QAudioFormat::Float;
    audioFormat.setSampleType(type);

    return audioFormat;
}

AudioStreamBasicDescription toAudioStreamBasicDescription(QAudioFormat const& audioFormat)
{
    AudioStreamBasicDescription sf;

    sf.mFormatFlags         = kAudioFormatFlagIsPacked;
    sf.mSampleRate          = audioFormat.sampleRate();
    sf.mFramesPerPacket     = 1;
    sf.mChannelsPerFrame    = audioFormat.channelCount();
    sf.mBitsPerChannel      = audioFormat.sampleSize();
    sf.mBytesPerFrame       = sf.mChannelsPerFrame * (sf.mBitsPerChannel / 8);
    sf.mBytesPerPacket      = sf.mFramesPerPacket * sf.mBytesPerFrame;
    sf.mFormatID            = kAudioFormatLinearPCM;

    switch (audioFormat.sampleType()) {
    case QAudioFormat::SignedInt: sf.mFormatFlags |= kAudioFormatFlagIsSignedInteger; break;
    case QAudioFormat::UnSignedInt: /* default */ break;
    case QAudioFormat::Float: sf.mFormatFlags |= kAudioFormatFlagIsFloat; break;
    case QAudioFormat::Unknown:  default: break;
    }

    if (audioFormat.byteOrder() == QAudioFormat::BigEndian)
        sf.mFormatFlags |= kAudioFormatFlagIsBigEndian;

    return sf;
}

// QAudioRingBuffer
QAudioRingBuffer::QAudioRingBuffer(int bufferSize):
    m_bufferSize(bufferSize)
{
    m_buffer = new char[m_bufferSize];
    reset();
}

QAudioRingBuffer::~QAudioRingBuffer()
{
    delete m_buffer;
}

int QAudioRingBuffer::used() const
{
    return m_bufferUsed.load();
}

int QAudioRingBuffer::free() const
{
    return m_bufferSize - m_bufferUsed.load();
}

int QAudioRingBuffer::size() const
{
    return m_bufferSize;
}

void QAudioRingBuffer::reset()
{
    m_readPos = 0;
    m_writePos = 0;
    m_bufferUsed.store(0);
}

QT_END_NAMESPACE


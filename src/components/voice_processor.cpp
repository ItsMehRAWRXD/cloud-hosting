#include "voice_processor.hpp"
#include <QMutexLocker>

VoiceProcessor::VoiceProcessor(QObject* parent)
    : QObject(parent)
    , m_supportedLanguages{"en-US", "en-GB", "es-ES", "fr-FR", "de-DE", "ja-JP", "zh-CN"}
{
}

bool VoiceProcessor::startListening()
{
    QMutexLocker lock(&m_mutex);
    m_listening = true;
    emit listeningStateChanged(true);
    return true;
}

bool VoiceProcessor::stopListening()
{
    QMutexLocker lock(&m_mutex);
    m_listening = false;
    emit listeningStateChanged(false);
    return true;
}

bool VoiceProcessor::isListening() const
{
    QMutexLocker lock(&m_mutex);
    return m_listening;
}

QString VoiceProcessor::processAudioBuffer(const QByteArray& buffer)
{
    QMutexLocker lock(&m_mutex);

    if (buffer.isEmpty()) {
        emit errorOccurred("Empty audio buffer");
        return {};
    }

    // Simulate transcription by converting buffer bytes to text
    QString transcription = QString::fromUtf8(buffer);
    emit transcriptionReady(transcription);
    return transcription;
}

QStringList VoiceProcessor::supportedLanguages() const
{
    QMutexLocker lock(&m_mutex);
    return m_supportedLanguages;
}

void VoiceProcessor::setLanguage(const QString& lang)
{
    QMutexLocker lock(&m_mutex);
    if (m_supportedLanguages.contains(lang)) {
        m_language = lang;
    }
}

QString VoiceProcessor::currentLanguage() const
{
    QMutexLocker lock(&m_mutex);
    return m_language;
}

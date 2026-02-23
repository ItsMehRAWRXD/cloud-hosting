#ifndef VOICE_PROCESSOR_HPP
#define VOICE_PROCESSOR_HPP

#include <QObject>
#include <QMutex>
#include <QByteArray>
#include <QString>
#include <QStringList>

class VoiceProcessor : public QObject
{
    Q_OBJECT

public:
    explicit VoiceProcessor(QObject* parent = nullptr);

    bool startListening();
    bool stopListening();
    bool isListening() const;

    QString processAudioBuffer(const QByteArray& buffer);
    QStringList supportedLanguages() const;
    void setLanguage(const QString& lang);
    QString currentLanguage() const;

signals:
    void transcriptionReady(const QString& text);
    void errorOccurred(const QString& error);
    void listeningStateChanged(bool listening);

private:
    mutable QMutex m_mutex;
    bool m_listening = false;
    QString m_language = "en-US";
    QStringList m_supportedLanguages;
};

#endif // VOICE_PROCESSOR_HPP

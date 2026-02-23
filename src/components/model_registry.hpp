#ifndef MODEL_REGISTRY_HPP
#define MODEL_REGISTRY_HPP

#include <QObject>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>

class ModelRegistry : public QObject {
    Q_OBJECT

public:
    struct ModelInfo {
        QString id;
        QString name;
        QString path;
        qint64 sizeBytes = 0;
        QString quantization;
        bool loaded = false;
        QString format;
    };

    explicit ModelRegistry(QObject* parent = nullptr);

    bool registerModel(const ModelInfo& info);
    bool unregisterModel(const QString& id);
    ModelInfo getModel(const QString& id) const;
    QVector<ModelInfo> allModels() const;
    QVector<ModelInfo> findModels(const QString& query) const;
    bool loadModel(const QString& id);
    bool unloadModel(const QString& id);
    bool isModelLoaded(const QString& id) const;
    int modelCount() const;
    int loadedModelCount() const;
    bool validateModel(const QString& id) const;

signals:
    void modelRegistered(const QString& id);
    void modelUnregistered(const QString& id);
    void modelLoaded(const QString& id);
    void modelUnloaded(const QString& id);
    void errorOccurred(const QString& error);

private:
    mutable QMutex m_mutex;
    QMap<QString, ModelInfo> m_models;
};

#endif // MODEL_REGISTRY_HPP

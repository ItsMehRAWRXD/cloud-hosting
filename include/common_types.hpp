#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <QString>

enum class PatchLayer {
    Memory,
    ByteLevel,
    Server,
    Unified
};

struct PatchResult {
    bool success;
    QString detail;
    int errorCode;

    static PatchResult ok(const QString& detail) {
        return {true, detail, 0};
    }

    static PatchResult error(const QString& detail, int code = -1) {
        return {false, detail, code};
    }
};

struct UnifiedResult {
    bool success;
    QString operation;
    QString detail;
    PatchLayer layer;

    static UnifiedResult failureResult(const QString& op, const QString& detail, PatchLayer layer) {
        return {false, op, detail, layer};
    }

    static UnifiedResult successResult(const QString& op, const QString& detail, PatchLayer layer) {
        return {true, op, detail, layer};
    }
};

struct CorrectionResult {
    bool success;
    QString correctedText;
    QString errorDetail;

    static CorrectionResult ok(const QString& text) {
        return {true, text, {}};
    }

    static CorrectionResult error(const QString& detail) {
        return {false, {}, detail};
    }
};

#endif // COMMON_TYPES_HPP

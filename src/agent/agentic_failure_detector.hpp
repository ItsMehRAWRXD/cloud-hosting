#ifndef AGENTIC_FAILURE_DETECTOR_HPP
#define AGENTIC_FAILURE_DETECTOR_HPP

#include "core/patch_result.hpp"

#ifndef NO_QT_SUPPORT
#include <QObject>
#endif

namespace RawrXD {

enum class FailureType {
    None,
    Refusal,
    Hallucination,
    Timeout,
    ResourceExhaustion,
    SafetyViolation
};

#ifndef NO_QT_SUPPORT
class AgenticFailureDetector : public QObject {
    Q_OBJECT
public:
    AgenticFailureDetector() : QObject(nullptr) {}
    ~AgenticFailureDetector() {}
};
#else
class AgenticFailureDetector {
public:
    AgenticFailureDetector() {}
    ~AgenticFailureDetector() {}
};
#endif

} // namespace RawrXD

#endif // AGENTIC_FAILURE_DETECTOR_HPP

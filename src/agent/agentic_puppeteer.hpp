#ifndef AGENTIC_PUPPETEER_HPP
#define AGENTIC_PUPPETEER_HPP

#include "core/patch_result.hpp"

#ifndef NO_QT_SUPPORT
#include <QObject>
#endif

namespace RawrXD {

#ifndef NO_QT_SUPPORT
class AgenticPuppeteer : public QObject {
    Q_OBJECT
public:
    AgenticPuppeteer() : QObject(nullptr) {}
    ~AgenticPuppeteer() {}
};
#else
class AgenticPuppeteer {
public:
    AgenticPuppeteer() {}
    ~AgenticPuppeteer() {}
};
#endif

} // namespace RawrXD

#endif // AGENTIC_PUPPETEER_HPP

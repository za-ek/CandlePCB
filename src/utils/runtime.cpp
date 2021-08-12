#include "runtime.h"
#include <QVariant>

Runtime * Runtime::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = new Runtime();
    }
    return _instance;
}

void Runtime::bind(QString key, std::function<void(const QString)>cb) {
    cb(getInstance()->key(key));
}

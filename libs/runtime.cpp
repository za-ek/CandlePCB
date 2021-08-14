#include "runtime.h"

Runtime * Runtime::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = new Runtime();
    }
    return _instance;
}

void Runtime::bind(QString key, std::function<void(const QString)>cb) {
    bindings.insert(key, cb);
}

void Runtime::insert(const QString &key, const QString &value) {
    QMap::insert(key, value);

    if(bindings.contains(key)) {
        bindings.value(key)(getInstance()->value(key));
    }
}

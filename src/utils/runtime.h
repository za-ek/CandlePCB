#ifndef RUNTIME_H
#define RUNTIME_H

#include <QMap>
#include <QVector>
#include <QVariant>
#include <mutex>

class Runtime : public QMap<QString, QVariant>
{
private:
    inline static Runtime * _instance{nullptr};
    inline static std::mutex _mutex;

protected:
    Runtime() {}

public:
    ~Runtime() {}
    Runtime(Runtime &a) = delete;
    void operator=(const Runtime &) = delete;

    static Runtime *getInstance();
    static void bind(QString key, std::function<void(const QString)>cb);
};

//Runtime* Runtime::_instance{nullptr};
//std::mutex Runtime::_mutex;

#endif // RUNTIME_H

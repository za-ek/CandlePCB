#ifndef RUNTIME_H
#define RUNTIME_H

#include <QMap>
#include <QVector>
#include <QString>
#include <mutex>

class Runtime : public QMap<QString, QString>
{
private:
    inline static Runtime * _instance{nullptr};
    inline static std::mutex _mutex;

    inline static QMap<QString, std::function<void(const QString)>> bindings;
    inline static std::function<void(const QString, const QString)> fn = nullptr;
protected:
    Runtime() {}

public:
    ~Runtime() {}
    Runtime(Runtime &a) = delete;
    void operator=(const Runtime &) = delete;

    static Runtime *getInstance();
    static void bind(QString key, std::function<void(const QString)>cb);
    static void bind(std::function<void(const QString, const QString)>cb);

    void insert(const QString &key, const QString &value);
};

//Runtime* Runtime::_instance{nullptr};
//std::mutex Runtime::_mutex;

#endif // RUNTIME_H

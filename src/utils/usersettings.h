#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings.h"
#include <QMap>
#include <QVector>
#include <QVariant>
#include <mutex>

class UserSettings : Settings
{
private:
    inline static Settings* _instance{nullptr};
    inline static std::mutex _mutex;

protected:
    UserSettings() {}

public:
    ~UserSettings() {}
    UserSettings(UserSettings &a) = delete;
    void operator=(const UserSettings &) = delete;

    static Settings* getInstance();
};

//Settings* UserSettings::_instance{nullptr};
//std::mutex UserSettings::_mutex;

#endif // SETTINGS_H

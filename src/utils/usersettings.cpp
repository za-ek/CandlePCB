#include "settings.h"
#include "usersettings.h"
#include <QVariant>

Settings * UserSettings::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = new Settings();
    }
    return _instance;
}

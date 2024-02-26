#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QString>

class SettingManager
{
protected:
    SettingManager();

public:
    static SettingManager* GetInstance();

public:
    QString m_title;
    QString m_url;
    QString m_jsPath;
    int m_windowWidth = 800;
    int m_windowHeight = 600;
};

#endif // SETTINGMANAGER_H

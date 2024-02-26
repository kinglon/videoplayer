#include "settingmanager.h"

SettingManager::SettingManager()
{

}

SettingManager* SettingManager::GetInstance()
{
    static SettingManager* instance = new SettingManager();
    return instance;
}

#ifndef MUGEN_CONFIG_H
#define MUGEN_CONFIG_H
#include "json.h"
#include "SDL_Utils.h"
#include <SDL2/SDL.h>
#include <string>
using JSON = nlohmann::json;

class Config
{
public:
    int m_sampleRate = -1;
    int m_channels = -1;
    int m_blockCount = -1;
    int m_blockSize = -1;
    char m_sampleType = '\0';
    std::string m_outputDevice;
    std::string assetsDir;
    ColorTheme colorTheme = {
            {0, 0, 0},
            {255, 255, 255},
            {119, 119, 119},
            {85, 85, 85},
            {64, 64, 64}};
    void load();
private:
    void loadConfigData(JSON data);
    void loadAudioSettings(JSON config);
    void loadColorTheme(JSON config);
    static SDL_Color hexToSDLColor(const std::string& hexColor);
    static JSON createDefaultAudioConfig();
    static JSON createDefaultThemeConfig();
    static bool isValidNumber(const JSON& param);
    static char getSampleType(const JSON& sType);
    static void createDefaultConfigFile(const std::string& path);
    static bool isNumber(const std::string& str);
    static bool isValidHex(const JSON& param);
};

#endif //MUGEN_CONFIG_H
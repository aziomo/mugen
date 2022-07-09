#include "../include/Config.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void Config::load()
{
    assetsDir = std::filesystem::is_directory("./assets/") ? "./assets/" : "../assets/";
    auto cfgPath = fs::current_path().string() + "/config.json";
    if (!fs::exists(cfgPath))
        createDefaultConfigFile(cfgPath);
    std::ifstream file;
    file.open(cfgPath);
    JSON cfgData = JSON::parse(file);
    loadConfigData(cfgData);
}

void Config::loadAudioSettings(JSON config)
{
    auto blockCount = config["block_count"],
    blockSize = config["block_size"],
    channels = config["channels"],
    sampleRate = config["sample_rate"],
    sampleType = config["sample_type"],
    outputDevice = config["output_device"];

    m_blockCount = isValidNumber(blockCount) ? blockCount.get<int>() : 8;
    m_blockSize = isValidNumber(blockSize) ? blockSize.get<int>() : 512;
    m_channels = isValidNumber(channels) ? channels.get<int>() : 1;
    m_sampleRate = isValidNumber(sampleRate) ? sampleRate.get<int>() : 44100;
    m_sampleType = getSampleType(sampleType);
    if (!outputDevice.empty()) m_outputDevice = outputDevice.get<std::string>();
}

void Config::loadColorTheme(JSON config)
{
    auto foreground = config["foreground"],
    background = config["background"],
    focusedBit = config["focusedBit"],
    inactiveBorder = config["inactiveBorder"],
    inactiveText = config["inactiveText"];

    if (isValidHex(foreground))
        colorTheme.foreground = hexToSDLColor(config["foreground"]);
    if (isValidHex(background))
        colorTheme.background = hexToSDLColor(config["background"]);
    if (isValidHex(focusedBit))
        colorTheme.focusedBit = hexToSDLColor(config["focusedBit"]);
    if (isValidHex(inactiveBorder))
        colorTheme.inactiveBorder = hexToSDLColor(config["inactiveBorder"]);
    if (isValidHex(inactiveText))
        colorTheme.inactiveText = hexToSDLColor(config["inactiveText"]);
}

SDL_Color Config::hexToSDLColor(const std::string& hexColor){
    int r, g, b;
    std::sscanf(hexColor.c_str(), "#%02x%02x%02x", &r, &g, &b);
    return {(Uint8)r, (Uint8)g, (Uint8)b};
}

bool Config::isValidNumber(const JSON& param)
{
    return !param.empty() && param.is_number_integer() && param.get<int>() > 0;
}

bool Config::isValidHex(const JSON& param)
{
    if (param.empty())
        return false;
    printf("1");
    if (!param.is_string())
        return false;
    printf("2");
    auto value = param.get<std::string>();
    if (value.size() != 7)
        return false;
    printf("3");
    if (value.at(0) != '#')
        return false;
    printf("4");
    printf("%s", value.substr(1).c_str());

    if (!isNumber(value.substr(1)))
        return false;
    printf("5\n");
    return true;
}

bool Config::isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0){
            if (c < 65 || c > 102 || (c > 70 && c < 97)) return false;
        }
    }
    return true;
}

char Config::getSampleType(const JSON& sType)
{
    auto sampleTypeStr = sType.get<std::string>();
    if (sampleTypeStr.rfind('f', 0) == 0) return 'f';
    if (sampleTypeStr.rfind('i', 0) == 0) return 'i';
    return 'f';
}

void Config::createDefaultConfigFile(const std::string& path)
{
    JSON config;
    config["audio_settings"] = createDefaultAudioConfig();
    config["color_theme"] = createDefaultThemeConfig();
//    config["block_count"] = 8;
//    config["block_size"] = 512;
//    config["channels"] = 1;
//    config["sample_rate"] = 44100;
//    config["sample_type"] = "float";
//    config["output_device"] = "";
    std::ofstream file;
    file.open("config.json");
    file << config.dump(4);
    file.close();
}

JSON Config::createDefaultAudioConfig(){
    JSON config;
    config["block_count"] = 8;
    config["block_size"] = 512;
    config["channels"] = 1;
    config["sample_rate"] = 44100;
    config["sample_type"] = "float";
    config["output_device"] = "";
    return config;
}

JSON Config::createDefaultThemeConfig(){
    JSON config;
    config["background"] = "#000000";
    config["foreground"] = "#ffffff";
    config["inactive_text"] = "#777777";
    config["inactive_border"] = "#555555";
    config["focused_bit"] = "#404040";
    return config;
}

void Config::loadConfigData(JSON data) {
    loadAudioSettings(data["audio_settings"]);
    loadColorTheme(data["color_theme"]);

}

#include "../include/Config.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void Config::load()
{
    auto cfgPath = fs::current_path().string() + "/config.json";
    if (!fs::exists(cfgPath))
        createDefaultConfigFile(cfgPath);
    std::ifstream file;
    file.open(cfgPath);
    JSON cfgData = JSON::parse(file);
    loadConfigData(cfgData);
}

void Config::loadConfigData(JSON data)
{
    auto blockCount = data["block_count"],
    blockSize = data["block_size"],
    channels = data["channels"],
    sampleRate = data["sample_rate"],
    sampleType = data["sample_type"],
    device = data["device"];

    m_blockCount = isValidNumber(blockCount) ? blockCount.get<int>() : 8;
    m_blockSize = isValidNumber(blockSize) ? blockSize.get<int>() : 512;
    m_channels = isValidNumber(channels) ? channels.get<int>() : 1;
    m_sampleRate = isValidNumber(sampleRate) ? sampleRate.get<int>() : 44100;
    m_sampleType = getSampleType(sampleType);
    if (!device.empty()) m_device = device.get<std::string>();
}

bool Config::isValidNumber(const JSON& param)
{
    return !param.empty() && param.is_number_integer() && param.get<int>() > 0;
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
    config["block_count"] = 8;
    config["block_size"] = 512;
    config["channels"] = 1;
    config["sample_rate"] = 44100;
    config["sample_type"] = "float";
    config["device"] = "";
    std::ofstream file;
    file.open("config.json");
    file << nlohmann::to_string(config);
    file.close();
}

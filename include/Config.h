//
// Created by alberto on 4/11/22.
//

#ifndef MUGEN_CONFIG_H
#define MUGEN_CONFIG_H
#include "json.h"
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
    std::string m_device;
    void load();
private:
    void loadConfigData(JSON data);
    static bool isValidNumber(const JSON& param);
    static char getSampleType(const JSON& sType);
    static void createDefaultConfigFile(const std::string& path);
};

#endif //MUGEN_CONFIG_H
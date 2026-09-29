#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "DLCFile.h"

class DLCAudioFile : public DLCFile {
public:
    // If you add to the Enum,then you need to add the array of type names
    // These are the names used in the XML for the parameters
    enum EAudioType {
        e_AudioType_Invalid = -1,

        e_AudioType_Overworld = 0,
        e_AudioType_Nether,
        e_AudioType_End,

        e_AudioType_Max,
    };
    enum EAudioParameterType {
        e_AudioParamType_Invalid = -1,

        e_AudioParamType_Cuename = 0,
        e_AudioParamType_Credit,

        e_AudioParamType_Max,

    };
    static const char* wchTypeNamesA[e_AudioParamType_Max];

    DLCAudioFile(const std::string& path);

    virtual void addData(std::uint8_t* pbData, std::uint32_t dataBytes);
    virtual std::uint8_t* getData(std::uint32_t& dataBytes);

    bool processDLCDataFile(std::uint8_t* pbData, std::uint32_t dataLength);
    int GetCountofType(DLCAudioFile::EAudioType ptype);
    std::string& GetSoundName(int iIndex);

private:
    using DLCFile::addParameter;

    std::uint8_t* m_pbData;
    std::uint32_t m_dataBytes;
    static const int CURRENT_AUDIO_VERSION_NUM = 1;
    // std::unordered_map<int, std::string> m_parameters;
    std::vector<std::string> m_parameters[e_AudioType_Max];

    // use the EAudioType to order these
    void addParameter(DLCAudioFile::EAudioType type,
                      DLCAudioFile::EAudioParameterType ptype,
                      const std::string& value);
    DLCAudioFile::EAudioParameterType getParameterType(
        const std::string& paramName);
};

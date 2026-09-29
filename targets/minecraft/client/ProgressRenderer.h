#pragma once
#include <stdint.h>

#include <mutex>
#include <string>

#include "minecraft/util/ProgressListener.h"

class Minecraft;

class ProgressRenderer : public ProgressListener {
public:
    enum eProgressStringType {
        eProgressStringType_ID,
        eProgressStringType_String,  // 4J-PB added for updating the bytes read
                                     // on a save transfer
    };

    static std::recursive_mutex s_progress;

    int getCurrentPercent();
    int getCurrentTitle();
    int getCurrentStatus();
    std::string& getProgressString(void);
    ProgressRenderer::eProgressStringType getType();

private:
    int lastPercent;

private:
    int status;
    Minecraft* minecraft;
    int title;
    int64_t lastTime;
    bool noAbort;
    std::string m_wstrText;
    eProgressStringType m_eType;

    void setType(eProgressStringType eType);

public:
    ProgressRenderer(Minecraft* minecraft);
    virtual void progressStart(int title);
    virtual void progressStartNoAbort(int string);
    void _progressStart(int title);
    virtual void progressStage(int status);
    virtual void progressStage(std::string& wstrText);
    virtual void progressStagePercentage(int i);
};
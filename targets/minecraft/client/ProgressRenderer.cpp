#include "ProgressRenderer.h"

#include "java/System.h"
#include "minecraft/client/Minecraft.h"

std::recursive_mutex ProgressRenderer::s_progress;

ProgressRenderer::ProgressRenderer(Minecraft* minecraft) {
    status = -1;
    title = -1;
    lastTime = System::currentTimeMillis();
    noAbort = false;
    this->minecraft = minecraft;
    this->m_eType = eProgressStringType_ID;
}

void ProgressRenderer::progressStart(int title) {
    noAbort = false;
    _progressStart(title);
}

void ProgressRenderer::progressStartNoAbort(int string) {
    noAbort = true;
    _progressStart(string);
}

void ProgressRenderer::_progressStart(int title) {
    // 4J Stu - Removing all progressRenderer rendering. This will be replaced
    // on the xbox
    if (!minecraft->running) {
        if (noAbort) return;
        //        throw new StopGameException();		// 4J - removed
    }

    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        lastPercent = 0;
        this->title = title;
    }
}

void ProgressRenderer::progressStage(int status) {
    if (!minecraft->running) {
        if (noAbort) return;
        //        throw new StopGameException();		// 4J - removed
    }

    lastTime = 0;
    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        m_eType = eProgressStringType_ID;
        this->status = status;
    }
    progressStagePercentage(-1);
    lastTime = 0;
}

void ProgressRenderer::progressStagePercentage(int i) {
    // 4J Stu - Removing all progressRenderer rendering. This will be replaced
    // on the xbox
    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        lastPercent = i;
    }
}

int ProgressRenderer::getCurrentPercent() {
    int returnValue = 0;
    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        returnValue = lastPercent;
    }
    return returnValue;
}

int ProgressRenderer::getCurrentTitle() {
    int returnValue;
    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        returnValue = title;
    }
    return returnValue;
}

int ProgressRenderer::getCurrentStatus() {
    int returnValue;
    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        returnValue = status;
    }
    return returnValue;
}

ProgressRenderer::eProgressStringType ProgressRenderer::getType() {
    eProgressStringType returnValue;
    {
        std::lock_guard<std::recursive_mutex> lock(
            ProgressRenderer::s_progress);
        returnValue = m_eType;
    }
    return returnValue;
}

void ProgressRenderer::setType(eProgressStringType eType) {
    std::lock_guard<std::recursive_mutex> lock(ProgressRenderer::s_progress);
    m_eType = eType;
}

void ProgressRenderer::progressStage(std::string& wstrText) {
    std::lock_guard<std::recursive_mutex> lock(ProgressRenderer::s_progress);
    m_wstrText = wstrText;
    m_eType = eProgressStringType_String;
}

std::string& ProgressRenderer::getProgressString(void) {
    std::lock_guard<std::recursive_mutex> lock(ProgressRenderer::s_progress);
    std::string& temp = m_wstrText;
    return temp;
}

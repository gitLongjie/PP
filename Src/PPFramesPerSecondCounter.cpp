#include "PPFramesPerSecondCounter.h"

#include <assert.h>

#include "Core/Logger.h"

PPFramesPerSecondCounter::PPFramesPerSecondCounter(float avgInterval /*= 0.5f*/)
    : m_avgInterval(avgInterval){
    assert(m_avgInterval > 0.f);
}

bool PPFramesPerSecondCounter::OnFrame(float deltaSeconds, bool frameRendered /*= true*/) {
    if (frameRendered)
        m_numFrames++;

    m_accumulatedTime += deltaSeconds;

    if (m_accumulatedTime > m_avgInterval) {
        m_currentFPS = static_cast<float>(m_numFrames / m_accumulatedTime);
        if (m_printFPS) {
            INFOLOG("FPS: {}", m_currentFPS);
        }

        m_numFrames = 0;
        m_accumulatedTime = 0;
        return true;
    }

    return false;
}

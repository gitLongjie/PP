#pragma once

class PPFramesPerSecondCounter {
public:
    PPFramesPerSecondCounter(float avgInterval = 0.5f);
    ~PPFramesPerSecondCounter() = default;

    bool OnFrame(float deltaSeconds, bool frameRendered = true);
    int GetFPS() const { return m_currentFPS; }

    void EnablePrintFPS(bool enable) { m_printFPS = enable; }

private:
    bool m_printFPS = false;
    const float m_avgInterval = 0.5f;
    unsigned int m_numFrames = 0;
    double m_accumulatedTime = 0;
    float m_currentFPS = 0.0f;
};
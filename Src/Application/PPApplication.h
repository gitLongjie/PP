#pragma once


namespace PPEngine {
    class PPApplication {
    public:
        PPApplication();
        virtual ~PPApplication();

        static PPApplication* Get();

        virtual bool Initialize();
        virtual int RunLoop();
        virtual void Uninitialize();

        float GeltaSeconds() const { return m_deltaSeconds; }

    protected:
        double m_timeStamp{ 0.0 };
        float m_deltaSeconds{ 0.0f };

    };
}


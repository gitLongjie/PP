#pragma once

namespace Core {
    class MainLoopImpl {
    public:
        virtual ~MainLoopImpl() = default;
        virtual int Run() = 0;
    };

    class MainLoop {
    public:
        MainLoop(MainLoopImpl* impl);
        ~MainLoop() = default;

        int Run();

    private:
        MainLoopImpl* impl_{ nullptr };
    };
}

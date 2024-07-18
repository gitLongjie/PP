#pragma once

#include <functional>
#include <assert.h>

namespace Core {

    template <typename S>
    class Singleton {
    public:
        virtual ~Singleton() = default;

        static bool Init() {
            assert(nullptr == instance_);
            instance_ = new S();
                
            return instance_->Initialize();

        }
        static void Shotdown() {
            assert(nullptr != instance_);
            instance_->Uninitialize();
            delete instance_;
            instance_ = nullptr;
        }

        static S* Get() {
            return instance_;
        }

        virtual bool Initialize() = 0;
        virtual void Uninitialize() = 0;

    protected:
        static S* instance_;
    };

    template <typename S>
    class SingletonSub {
    public:
        virtual ~SingletonSub() = default;

        static bool Init() {
            assert(nullptr == instance_);
            instance_ = S::Create();

            return instance_->Initialize();

        }
        static void Shotdown() {
            assert(nullptr != instance_);
            instance_->Uninitialize();
            delete instance_;
            instance_ = nullptr;
        }

        static S* Get() {
            return instance_;
        }

        virtual bool Initialize() = 0;
        virtual void Uninitialize() = 0;

    protected:
        static S* instance_;
    };
}

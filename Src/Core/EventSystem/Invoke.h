#pragma once

#include <atomic>
#include <functional>
#include <memory>

namespace PPEngine {
    namespace Core {
        namespace EventSystem {
            class Invoke {
            public:
                Invoke() = default;
                virtual ~Invoke() = default;

              //  virtual void Do(std::shared_ptr<class Event >&) = 0;
            };

            template<typename ..._Args> struct InvokeImpl;

            //template<class _Rx>
            template <typename _Ty, typename ..._Args>
            struct InvokeImpl<_Ty, _Args...> : public Invoke {
                //InvokeImpl(_Rx fun) {}// : fun_(std::move(fun)) {}


            };

        }
    }
}

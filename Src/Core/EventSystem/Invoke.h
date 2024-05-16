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

            //template<class _Rx>
            template <class _Result, class _Ty, class _Arg>
            class MemberInvokeImpl : public Invoke {
            public:
               // using FuncType = std::mem_fun<_Rx>;
                
                    MemberInvokeImpl(_Rx fun) {}// : fun_(std::move(fun)) {}

             //   void Do(std::shared_ptr<class Event >&) {}

            private:
              //  FuncType fun_;


            };

        }
    }
}

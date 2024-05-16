#pragma once


#include <queue>
#include <memory>
#include <vector>
#include <map>
//#include <any>

#include "Core/Constent.h"
#include "Core/Singleton.h"
#include "Core/EventSystem/Constent.h"
#include "Core/EventSystem/Event.h"
#include "Core/EventSystem/Invoke.h"

namespace PPEngine {
    namespace Core {
        namespace EventSystem {
            struct IStoreValue {
                virtual ~IStoreValue() = default;
                virtual void Exec(const IEvent& event) = 0;
            };
            
            template <class ET, class O, typename F>
            struct StoreValue : public IStoreValue {
                O obj_;
                F fun_;
                EventType eventType_;

                void Exec(const IEvent& event) override {
                    if (event.GetType() != eventType_) {
                        return;
                    }

                    fun_(*reinterpret_cast<const ET*>(&event));
                }
            };

            class EventManager : public Singleton<EventManager> {
            public:
                EventManager() = default;
                ~EventManager() override = default;

            public:
                bool Initialize() override;
                void Uninitialize() override;

                template<typename ...Args, typename C, typename ...FArgs, class O, typename ET, typename R>
                void Attach(const ET& et, R (C::* f)(FArgs...), O&& o) {
                    std::function<R(Args...)> fn = [&](Args ...args) {
                        return (*o.*f)(std::forward<Args>(args)...);
                    };
                    
                    StoreValue<ET, O, std::function<R(Args...)>>* storeValue = new StoreValue<ET, O, std::function<R(Args...)>>;
                    test_ = storeValue;

                    storeValue->obj_ = std::move(o);
                    storeValue->fun_ = std::move(fn);
                    storeValue->eventType_ = ET::Type();
                }

             /*   template<class... Args, class C, class... DArgs, class P, typename R = int>
                void Attach(R(C::* f)(DArgs...), P&& p) {
                    std::function<R(Args...)> fn = [&, f](Args... args) {return (*p.*f)(std::forward<Args>(args)...); };
                }*/
                template<typename ET, typename Sender>
                bool Send(const ET& event, Sender* sender) {
                   // return sender->OnHandlerEvent(event);
                    EventType eventType = ET::Type();

                    test_->Exec(event);
                    return true;
                }

                void Dispatch();

            private:
                //std::queue<std::shared_ptr<Event>> eventQueue_;
                IStoreValue* test_;
            };
        }
    }
}

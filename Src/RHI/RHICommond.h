#pragma once

#include <stdint.h>
#include <functional>
#include <memory>
#include <atomic>
#include <chrono>
#include <thread>

namespace PPEngine {
    namespace PPRHI {
        class RHICommond {
        public:
            enum class Type : uint8_t {
                NONE,
                UPDATE_SCREEN_SIZE,
                CREATE_SHADER,
                COMPILE_SHADER,
                DELETE_SHADER,
                CREATE_PROGRAM,
                DELETE_PROGRAM,
                CONNECT_UNIFORM_BLOCK_INSTANCE_AND_BINDING_POINT,
                USE_SHADER_PROGRAM,
                CREATE_VAO,
                UPDATE_VBO_SUB_DATA,
                DRAW_ARRAYS,
                DRAW_ELEMENTS,
                DELETE_VAO,
                CREATE_UBO,
                UPDATE_UBO_SUB_DATA,
                CREATE_COMPRESSED_TEX_IMAGE2D,
                CREATE_TEX_IMAGE2D,
                DELETE_TEXTURES,
                UPDATE_TEXTURE_SUB_IMAGE2D,
                SET_ENABLE_STATE,
                SET_BLENDER_FUNC,
                SET_UNIFORM_MATRIX_4FV,
                ACTIVE_AND_BIND_TEXTURE,
                SET_UNIFORM_1I,
                SET_UNIFORM_1F,
                SET_UNIFORM_3F,
                BIND_VAO_AND_DRAW_ELEMENTS,
                SET_CLEAR_FLAG_AND_CLEAR_COLOR_BUFFER,
                SET_STENCIL_FUNC,
                SET_STENCIL_OP,
                SET_STENCIL_BUFFER_CLEAR_VALUE,
                END_FRAME,
                START_RENDER,
                END_RENDER,
            };

        public:
            explicit RHICommond(Type type) noexcept
                : type_(type) {}
            virtual ~RHICommond() = default;

            Type GetType() const noexcept { return type_; }
            void Execute() {
                OnExecute();
                wait_.store(false);
            }

            void Wait() {
                while (wait_.load()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(0));
                }
            }

        protected:
            virtual void OnExecute() = 0;

        private:
            Type type_;
            std::atomic_bool wait_{ true };
        };

        class StartRenderCommand : public RHICommond {
        public:
            virtual ~StartRenderCommand() = default;
            static std::shared_ptr<RHICommond> Create() {
                std::shared_ptr<RHICommond> Self(new StartRenderCommand);
                return Self;
            }

        protected:
            StartRenderCommand() : RHICommond(RHICommond::Type::END_RENDER) {}
            void OnExecute() override;
        };

        class UpdateRenderSizeCommand : public RHICommond {
        public:
            virtual ~UpdateRenderSizeCommand() = default;
            static std::shared_ptr<RHICommond> Create() {
                std::shared_ptr<RHICommond> Self(new UpdateRenderSizeCommand);
                return Self;
            }

        protected:
            UpdateRenderSizeCommand() : RHICommond(RHICommond::Type::UPDATE_SCREEN_SIZE) {}
            void OnExecute() override;
        };

        class EndRenderCommand : public RHICommond {
        public:
            virtual ~EndRenderCommand() = default;
            static std::shared_ptr<RHICommond> Create() {
                std::shared_ptr<RHICommond> Self(new EndRenderCommand);
                return Self;
            }
        protected:
            EndRenderCommand() : RHICommond(RHICommond::Type::END_RENDER) {}
            void OnExecute() override {}

        };
    }
}
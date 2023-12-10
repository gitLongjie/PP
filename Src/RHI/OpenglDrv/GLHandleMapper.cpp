#include "RHI/OpenglDrv/GLHandleMapper.h"

#include <unordered_map>

namespace PPEngine {
    namespace PPRHI {
        static unsigned int s_vaoHandle = 1;
        static std::unordered_map<unsigned int, unsigned int> s_vaoHandleMapper;

        static unsigned int s_vboHandle = 1;
        static std::unordered_map<unsigned int, unsigned int> s_vboHandleMapper;

        static unsigned int s_eboHandle = 1;
        static std::unordered_map<unsigned int, unsigned int> s_eboHandleMapper;
        
        static unsigned int s_shaderHandle = 1;
        static std::unordered_map<unsigned int, unsigned int> s_shaderHandleMapper;
        
        static unsigned int s_programHandle = 1;
        static std::unordered_map<unsigned int, unsigned int> s_programHandleMapper;

        unsigned int GLHandleMapper::GeneratorVAOHandle() {
            return s_vaoHandle++;
        }

        unsigned int GLHandleMapper::GeneratorVBOHandle() {
            return s_vboHandle++;
        }

        unsigned int GLHandleMapper::GeneratorEBOHandle() {
            return s_eboHandle++;
        }

        unsigned int GLHandleMapper::GeneratorShaderHandle() {
            return s_shaderHandle++;
        }

        unsigned int GLHandleMapper::GeneratorProgramHandle() {
            return s_programHandle++;
        }


        void GLHandleMapper::MapVAOHandle(unsigned int vaoHandle, unsigned int vao) {
            s_vaoHandleMapper.insert(std::make_pair(vaoHandle, vao));
        }

        unsigned int GLHandleMapper::GetVAO(unsigned int vaoHandle) {
            if (s_vaoHandleMapper.find(vaoHandle) != s_vaoHandleMapper.end()) {
                return s_vaoHandleMapper[vaoHandle];
            }
            
            return 0;
        }

        void GLHandleMapper::UnmapVAOHandle(unsigned int vaoHandle) {
            s_vaoHandleMapper.erase(vaoHandle);
        }

        void GLHandleMapper::MapVBOHandle(unsigned int vboHandle, unsigned int vbo) {
            s_vboHandleMapper.insert(std::make_pair(vboHandle, vbo));
        }

        unsigned int GLHandleMapper::GetVBO(unsigned int vboHandle) {
            if (s_vboHandleMapper.find(vboHandle) != s_vboHandleMapper.end()) {
                return s_vboHandleMapper[vboHandle];
            }

            return 0;
        }

        void GLHandleMapper::UnmapVBOHandle(unsigned int vboHandle) {
            s_vboHandleMapper.erase(vboHandle);
        }

        void GLHandleMapper::MapEBOHandle(unsigned int eboHandle, unsigned int ebo) {
            s_eboHandleMapper.insert(std::make_pair(eboHandle, ebo));
        }
        unsigned int GLHandleMapper::GetEBO(unsigned int eboHandle) {
            if (s_eboHandleMapper.find(eboHandle) != s_eboHandleMapper.end()) {
                return s_eboHandleMapper[eboHandle];
            }

            return 0;
        }
        void GLHandleMapper::UnmapEBOHandle(unsigned int eboHandle) {
            s_eboHandleMapper.erase(eboHandle);
        }


        void GLHandleMapper::MapShaderHandle(unsigned int shaderHandle, unsigned int shader) {
            s_shaderHandleMapper.insert(std::make_pair(shaderHandle, shader));
        }
        unsigned int GLHandleMapper::GetShader(unsigned int shaderHandle) {
            if (s_shaderHandleMapper.find(shaderHandle) != s_shaderHandleMapper.end()) {
                return s_shaderHandleMapper[shaderHandle];
            }

            return 0;
        }
        void GLHandleMapper::UnmapShaderHandle(unsigned int shaderHandle) {
            s_shaderHandleMapper.erase(shaderHandle);
        }

        void GLHandleMapper::MapProgramHandle(unsigned int programHandle, unsigned int program) {
            s_programHandleMapper.insert(std::make_pair(programHandle, program));
        }
        unsigned int GLHandleMapper::GetProgram(unsigned int programHandle) {
            if (s_programHandleMapper.find(programHandle) != s_programHandleMapper.end()) {
                return s_programHandleMapper[programHandle];
            }

            return 0;
        }
        void GLHandleMapper::UnmapProgramHandle(unsigned int programHandle) {
            s_programHandleMapper.erase(programHandle);
        }
    }
}
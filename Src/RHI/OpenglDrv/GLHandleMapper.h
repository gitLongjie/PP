#pragma once

namespace PPEngine {
    namespace PPRHI {
        class GLHandleMapper {
        public:
            static unsigned int GeneratorVAOHandle();
            static unsigned int GeneratorVBOHandle();
            static unsigned int GeneratorEBOHandle();
            static unsigned int GeneratorShaderHandle();
            static unsigned int GeneratorProgramHandle();

            static void MapVAOHandle(unsigned int vaoHandle, unsigned int vao);
            static unsigned int GetVAO(unsigned int vaoHandle);
            static void UnmapVAOHandle(unsigned int vaoHandle);

            static void MapVBOHandle(unsigned int vboHandle, unsigned int vbo);
            static unsigned int GetVBO(unsigned int vboHandle);
            static void UnmapVBOHandle(unsigned int vboHandle);

            static void MapEBOHandle(unsigned int eboHandle, unsigned int ebo);
            static unsigned int GetEBO(unsigned int eboHandle);
            static void UnmapEBOHandle(unsigned int eboHandle);

            static void MapShaderHandle(unsigned int shaderHandle, unsigned int shader);
            static unsigned int GetShader(unsigned int shaderHandle);
            static void UnmapShaderHandle(unsigned int shaderHandle);

            static void MapProgramHandle(unsigned int programHandle, unsigned int program);
            static unsigned int GetProgram(unsigned int programHandle);
            static void UnmapProgramHandle(unsigned int programHandle);
        };
    }
}
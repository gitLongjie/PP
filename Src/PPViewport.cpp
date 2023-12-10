#include "PPViewport.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>


static const char* shaderCodeVertex = R"(
#version 460 core
layout(std140, binding = 0) uniform PerFrameData
{
	uniform mat4 view;
	uniform mat4 proj;
};
layout (location=0) out vec2 uv;
const vec2 pos[3] = vec2[3](
	vec2(-0.6f, -0.4f),
	vec2( 0.6f, -0.4f),
	vec2( 0.0f,  0.6f)
);
const vec2 tc[3] = vec2[3](
	vec2( 0.0, 0.0 ),
	vec2( 1.0, 0.0 ),
	vec2( 0.5, 1.0 )
);
void main()
{
	gl_Position = proj * view * vec4(pos[gl_VertexID], 0.0, 1.0);
	uv = tc[gl_VertexID];
}
)";

static const char* shaderCodeFragment = R"(
#version 460 core
layout (location=0) in vec2 uv;
layout (location=0) out vec4 out_FragColor;
uniform sampler2D texture0;
void main()
{
	out_FragColor = texture(texture0, uv);
};
)";

PPViewport::PPViewport()
    : shaderVertex(GL_VERTEX_SHADER, shaderCodeVertex)
    , shaderFragment(GL_FRAGMENT_SHADER, shaderCodeFragment)
    , shaderProgram(shaderVertex, shaderFragment){
}

bool PPViewport::Init() {
    return m_camera.Init();
}

void PPViewport::Uninit() {
    m_camera.Uninit();
}

void PPViewport::BeginRender(GLFWwindow* window) {
    m_timeStamp = glfwGetTime();
    m_deltaSeconds = 0.0f;

    vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int w, h, comp;
    const uint8_t* img = stbi_load("D:/test/ch2_sample3_STB.jpg", &w, &h, &comp, 3);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(texture, 1, GL_RGB8, w, h);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(texture, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
    glBindTextures(0, 1, &texture);

    stbi_image_free((void*)img);
}

void PPViewport::Render(GLFWwindow* window) {
    m_framePerSecondCounter.OnFrame(m_deltaSeconds);

    const double newTimeStamp = glfwGetTime();
    m_deltaSeconds = static_cast<float>(newTimeStamp - m_timeStamp);
    m_timeStamp = newTimeStamp;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    const float ratio = width / (float)height;

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera.Update(ratio);
    shaderProgram.useProgram();
    

    glDrawArrays(GL_TRIANGLES, 0, 3);

}

void PPViewport::EndRender(GLFWwindow* window) {
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &vao);
}


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

int main(int argc, char *argv[])
{
    if (!glfwInit())
    {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(error_callback);

    GLFWwindow *window = glfwCreateWindow(800, 600, "gengine", NULL, NULL);
    if (!window)
    {
        std::cerr << "failed to create GLFW widnow" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize GLAD" << std::endl;
        return -1;
    }

    float points[] = {
        0.0f,
        0.5f,
        0.0f, /// xyz of first point

        0.5f,
        -0.5f,
        0.0f, /// xyz of second point

        -0.5f,
        -0.5f,
        0.0f, /// xyz of third point
    };

    // vector buffer object
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    // vector array object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // shaders
    const char *vertex_shader = "#version 330 core\n"
                                "layout (location = 0) in vec3 vp;"
                                "void main() {"
                                "   gl_Position = vec4(vp, 1.0);"
                                "}";

    const char *fragment_shader = "#version 330 core\n"
                                  "out vec4 frag_color;"
                                  "void main() {"
                                  " frag_color = vec4(0.5, 0.0, 0.5, 1.0);"
                                  "}";

    // vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    // fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    int success;
    char infoLog[512];
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cerr << "shader fragment compilation failed" << infoLog << std::endl;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}
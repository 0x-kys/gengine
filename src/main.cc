#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <gengine/shader.h>

void error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(error_callback);

    GLFWwindow *window = glfwCreateWindow(800, 600, "gengine", NULL, NULL);
    if (!window) {
        std::cerr << "failed to create GLFW widnow" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "failed to initialize GLAD" << std::endl;
        return -1;
    }

    float points[] = {
        // positions          // colors
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top -- red
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right -- green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left -- blue
    };

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    // load shader using shader.h
    Shader myShader("shaders/vert.glsl", "shaders/frag.glsl");
    if (myShader.ID == 0) {
        std::cerr << "shader program failed to initialize" << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(myShader.ID);
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}
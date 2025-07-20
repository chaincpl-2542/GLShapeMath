#include <iostream>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.hpp"


using namespace std;
using namespace CPL;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aColor; // Vertex color

out vec3 vertexColor; // Output color to the fragment shader

uniform mat4 uMVP; //

void main()
{
    gl_Position = uMVP * vec4(aPos, 1.0); // Set the position of the vertex
    vertexColor = aColor; // Pass the color to the fragment shader
}

)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor; // Input color from the vertex shader
out vec4 FragColor; // Output color of the fragment

void main()
{
    FragColor = vec4(vertexColor, 1.0f); // Set the output color with full opacity
}

)";

unsigned int compileShader(unsigned int type, const char* source)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	return shader;
}

unsigned int createShaderProgram()
{
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	return shaderProgram;
}
// ───────────────────────────────────────────
int main()
{
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Mesh System", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    unsigned int shaderProgram = createShaderProgram();

    int currentShape = 1;

	Mesh triangle = Mesh::CreateTriangle();
    Mesh quad = Mesh::CreateQuad();
    Mesh circle = Mesh::CreateCircle();
    Mesh cube = Mesh::CreateCube();
    Mesh pyramid = Mesh::CreatePyramid();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currentShape = 1;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currentShape = 2;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currentShape = 3;
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currentShape = 4;
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currentShape = 5;

        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        if (currentShape == 4 || currentShape == 5)
        {
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        }

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * model;

        int mvpLoc = glGetUniformLocation(shaderProgram, "uMVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        //Draw
        switch (currentShape)
        {
            case 1: triangle.Draw(); break;
            case 2: circle.Draw(); break;
            case 3: quad.Draw(); break;
            case 4: cube.Draw(); break;
            case 5: pyramid.Draw(); break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

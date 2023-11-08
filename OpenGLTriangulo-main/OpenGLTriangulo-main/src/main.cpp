#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); // Color blanco\n"
"}\0";

const char *fragmentShaderSourceBlack = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Color negro\n"
"}\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cuadrado Negro Movido hacia Arriba", NULL, NULL);
    if (window == NULL) {
        std::cout << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: Fallo la compilación del shader de vértices" << infoLog << std::endl;
    }

    unsigned int fragmentShaderWhite = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderWhite, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderWhite);
    glGetShaderiv(fragmentShaderWhite, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderWhite, 512, NULL, infoLog);
        std::cout << "ERROR: Fallo la compilación del shader de fragmentos" << infoLog << std::endl;
    }

    unsigned int fragmentShaderBlack = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderBlack, 1, &fragmentShaderSourceBlack, NULL);
    glCompileShader(fragmentShaderBlack);
    glGetShaderiv(fragmentShaderBlack, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderBlack, 512, NULL, infoLog);
        std::cout << "ERROR: Fallo la compilación del shader de fragmentos (negro)" << infoLog << std::endl;
    }

    unsigned int shaderProgramWhite, shaderProgramBlack;
    shaderProgramWhite = glCreateProgram();
    glAttachShader(shaderProgramWhite, vertexShader);
    glAttachShader(shaderProgramWhite, fragmentShaderWhite);
    glLinkProgram(shaderProgramWhite);
    glGetProgramiv(shaderProgramWhite, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramWhite, 512, NULL, infoLog);
        std::cout << "ERROR: Fallo la vinculación del programa de shaders (blanco)" << infoLog << std::endl;
    }

    shaderProgramBlack = glCreateProgram();
    glAttachShader(shaderProgramBlack, vertexShader);
    glAttachShader(shaderProgramBlack, fragmentShaderBlack);
    glLinkProgram(shaderProgramBlack);
    glGetProgramiv(shaderProgramBlack, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramBlack, 512, NULL, infoLog);
        std::cout << "ERROR: Fallo la vinculación del programa de shaders (negro)" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderWhite);
    glDeleteShader(fragmentShaderBlack);

    float vertices[] = {     // rectángulo:
        -0.1f, -0.25f, 0.0f, // abajo izquierda 
         0.1f, -0.25f, 0.0f, // abajo derecha
         0.1f,  0.25f, 0.0f, // arriba derecha
        -0.1f,  0.25f, 0.0f, // arriba izquierda

                             // cuadrado negro:
        -0.2f, 0.1f, 0.0f, // abajo izquierda 
        -0.1f, 0.1f, 0.0f, // abajo derecha
        -0.1f, 0.2f, 0.0f, // arriba derecha
        -0.2f, 0.2f, 0.0f  // arriba izquierda
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6, // Primer triángulo del cuadrado negro
        6, 7, 4  // Segundo triángulo del cuadrado negro
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibuja el rectángulo blanco
        glUseProgram(shaderProgramWhite);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Dibuja el cuadrado negro
        glUseProgram(shaderProgramBlack);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgramWhite);
    glDeleteProgram(shaderProgramBlack);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// g++ main.cpp -o main -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
// ./main

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad.c"
#include "shader.cpp"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// Shader shader;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_Q)){
        glfwSetWindowShouldClose(window, true);
    }
}


int main()
{

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT, "OpenGL!", NULL, NULL);
    if(window == NULL){
        std::cout << "FAILED TO CREATE WINDOW"<< endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    // Check GLAD works
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and Compile Shaders into Programs
    GLuint vertexShader = Shader::make_module("shaders/vertex.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = Shader::make_module("shaders/fragment.frag", GL_FRAGMENT_SHADER);
    GLuint modules[2] = {vertexShader, fragmentShader};
    GLuint shaderProgram = Shader::make_shader(modules);


    // Delete Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Setup Vertex Data
    float triangle[] = {
        // Position             // Phase        // Texture Coord
        -0.5f, -0.5f, 0.0f,     M_PI/2,         0.0f,0.0f,
        -0.5f, 0.5f, 0.0f,      M_PI,           0.0f,1.0f,
        0.5f, 0.5f, 0.0f,       3 * M_PI/2,     1.0f,1.0f,
        0.5f, -0.5f, 0.0f,      2*M_PI,         1.0f,0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    float texCoords[] = {
        0.0f, 0.0f,
        0.5f, 1.0f,
        1.0f, 0.0f
    };

    glm::mat4 transformation = glm::mat4(1.0f);
    transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(0.0f,0.0f,1.0f));
    


    // Image/Texture Stuff

    // Binding 
    GLuint texture1, texture2;
    stbi_set_flip_vertically_on_load(true);  
    
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Actually putting in data
    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Texture not loaded" << "\n";
    }

    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Actually putting in data
    
    data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Texture not loaded2" << "\n";
    }

    stbi_image_free(data);

    // Binding to Arrays and Buffers
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER , sizeof(triangle), triangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Vertex Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Phase shift
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(4*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Clear Buffer/Array
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);


    // Set textures in uniform
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
    
    // Render Loop
    while(!glfwWindowShouldClose(window)){

        // Input 
        processInput(window);

        // Background
        glClearColor(0.2f,0.3f,0.4f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use program
        glUseProgram(shaderProgram);
        
        
        int timeLocation = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(timeLocation, glfwGetTime());

        int transformLoc = glGetUniformLocation(shaderProgram, "transformation");
        
        transformation = glm::mat4(1.0f);
        // transformation = glm::rotate(transformation, glm::radians(12.0f * (float)glfwGetTime()), glm::vec3(0.0f,0.0f,1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation));       

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        

        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}



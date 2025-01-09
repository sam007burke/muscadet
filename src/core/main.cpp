#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "systems/Camera.hpp"

std::string loadShaderFromFile(const std::string& filepath);

struct App {

    // Window dimensions
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    
    SDL_Window* window;
    SDL_GLContext glContext;

    bool running = true;

    GLuint shaderProgram;
    Camera camera;
};

struct Transform {
    // glm::vec3 translation = glm::vec3(0.0f);
    // float scale = 1.0f;
    // float rotate = 0.0f;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
};

struct Mesh3D {
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    GLuint pipeline = 0;
    Transform transform;
};

struct GameProperties {
    float movementSpeed = 0.01f;
};

/* --------------------------------- GLOBALS -------------------------------- */
App app;
Mesh3D mesh1;
Mesh3D mesh2;
GameProperties gameProperties;

static void glClearErrors() {
    while (glGetError() != GL_NO_ERROR) {}
}

static bool glCheckErrorStatus(const char* function, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error
            << "\tLine: " << line
            << "Function: " << function
            << std::endl;
        return true;
    }
    return false;
}

#define glCheck(x) glClearErrors(); x; glCheckErrorStatus(#x, __LINE__);

int initialiseProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // specify OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // updates twice as often
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    app.window = SDL_CreateWindow(
        "Muscadet",
        SDL_WINDOWPOS_CENTERED,
        0, // top of screen
        app.WINDOW_WIDTH,
        app.WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!app.window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    /**
     * OpenGL Context
     * --------------
     * A big object that keeps the state of the OpenGL application.
     */
    app.glContext = SDL_GL_CreateContext(app.window);
    if (!app.glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return -1;
    }

    // setup function pointers
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialise GLAD" << std::endl;
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return -1;
    }

    return 0;
}

void cleanup(Mesh3D* mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteProgram(mesh->pipeline);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

void preDraw() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // setup window + background
    glViewport(0, 0, app.WINDOW_WIDTH, app.WINDOW_HEIGHT);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // clear colour and depth buffer
}

GLint findUniformLocation(GLuint pipeline, const GLchar* name) {
    GLint uniformLocation = glGetUniformLocation(pipeline, name);
    if (uniformLocation < 0) {
        std::cerr << "Could not set " << name << " matrix, consider a mispelling." << std::endl;
        exit(EXIT_FAILURE);
    }

    return uniformLocation;
}

void meshTranslate(Mesh3D* mesh, float x, float y, float z) {
    mesh->transform.modelMatrix = glm::translate(mesh->transform.modelMatrix, glm::vec3(x, y, z));
}

void meshRotate(Mesh3D* mesh, float radians, glm::vec3 normal) {
    mesh->transform.modelMatrix = glm::rotate(mesh->transform.modelMatrix, radians, normal);
}

void meshScale(Mesh3D* mesh, float x, float y, float z) {
    mesh->transform.modelMatrix = glm::scale(mesh->transform.modelMatrix, glm::vec3(x, y, z));
}

void meshScale(Mesh3D* mesh, float xyz) {
    meshScale(mesh, xyz, xyz, xyz);
}

void meshDraw(Mesh3D* mesh) {
    if (mesh == nullptr) {
        return;
    }

    // use our shader
    glUseProgram(mesh->pipeline);

    // NOTE: often, for optimisation, we combine the model and view matrices

    // translation matrix
    GLint modelMatrixLocation = findUniformLocation(mesh->pipeline, "u_modelMatrix");
    glUniformMatrix4fv(modelMatrixLocation, 1, false, &mesh->transform.modelMatrix[0][0]);

    // perspective matrix
    glm::mat4 perspective = app.camera.getPerspective();
    GLint perspectiveMatrixLocation = findUniformLocation(mesh->pipeline, "u_perspectiveMatrix");
    glUniformMatrix4fv(perspectiveMatrixLocation, 1, false, &perspective[0][0]);

    // view matrix
    glm::mat4 view = app.camera.getView();
    GLint viewMatrixLocation = findUniformLocation(mesh->pipeline, "u_viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, false, &view[0][0]);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void handleEvents() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            app.running = false;
            break;
        case SDL_MOUSEMOTION:
            app.camera.mouseLook(event.motion.xrel, event.motion.yrel);
            break;

        // put other events here

        default:
            break;
        }
    }

    // Retrieve keyboard state
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    // [ESC] exit program    
    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        app.running = false;
    }

    // [WASD] camera movement
    if (keyboardState[SDL_SCANCODE_D]) {
        app.camera.moveRight(gameProperties.movementSpeed);
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        app.camera.moveLeft(gameProperties.movementSpeed);
    }
    if (keyboardState[SDL_SCANCODE_W]) {
        app.camera.moveForward(gameProperties.movementSpeed);
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        app.camera.moveBackward(gameProperties.movementSpeed);
    }

    // [<-- / -->] rotate meshes
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        meshRotate(&mesh1, 0.05f, glm::vec3(0.f, 1.f, 0.f));
        meshRotate(&mesh2, 0.05f, glm::vec3(0.f, 1.f, 0.f));
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        meshRotate(&mesh1, -0.05f, glm::vec3(0.f, 1.f, 0.f));
        meshRotate(&mesh2, -0.05f, glm::vec3(0.f, 1.f, 0.f));
    }

    // [ ^ / v] scale meshes
    if (keyboardState[SDL_SCANCODE_UP]) {
        meshScale(&mesh1, 1.01f);
        meshScale(&mesh2, 1.01f);
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        meshScale(&mesh1, 0.99f);
        meshScale(&mesh2, 0.99f);
    }
}

void mainLoop() {

    SDL_WarpMouseInWindow(app.window, app.WINDOW_WIDTH/2, app.WINDOW_HEIGHT/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (app.running) {

        handleEvents();

        // meshRotate(&mesh1, 0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
        // meshRotate(&mesh2, -0.05f, glm::vec3(0.0f, 1.0f, 0.0f));

        preDraw();
        meshDraw(&mesh1);
        meshDraw(&mesh2);

        SDL_GL_SwapWindow(app.window);
    }
}

void meshVertexSpec(Mesh3D* mesh) {

    mesh->pipeline = app.shaderProgram;

    // lives on CPU
    // winding order: anticlockwise
    const std::vector<GLfloat> vertices = {
        // x     y     z     r     g     b
        // front vertices
        -0.2f, -0.2f, 0.2f, 1.0f, 0.0f, 0.0f, // Bottom-left vertex     0
         0.2f, -0.2f, 0.2f, 0.0f, 1.0f, 0.0f, // Bottom-right vertex    1
        -0.2f,  0.2f, 0.2f, 0.0f, 0.0f, 1.0f, // Top-left vertex        2
         0.2f,  0.2f, 0.2f, 1.0f, 0.0f, 0.0f,  // Top-right vertex      3

        // back vertices
        -0.2f, -0.2f,-0.2f, 1.0f, 0.0f, 0.0f, // Bottom-left vertex     4
         0.2f, -0.2f,-0.2f, 0.0f, 1.0f, 0.0f, // Bottom-right vertex    5
        -0.2f,  0.2f,-0.2f, 0.0f, 0.0f, 1.0f, // Top-left vertex        6
         0.2f,  0.2f,-0.2f, 1.0f, 0.0f, 0.0f  // Top-right vertex       7
    };

    const std::vector<GLuint> indices = {
        // front face
        2, 0, 1,
        3, 2, 1,
        
        // bottom face
        0, 4, 1,
        1, 4, 5,

        // top face
        2, 3, 6,
        3, 7, 6,

        // left face
        0, 2, 6,
        0, 6, 4,

        // right face
        1, 7, 3,
        1, 5, 7,

        // back face
        6, 5, 4,
        7, 5, 6
    };

    // Create VAO and VBO
    // VAO: Vertex Array Object (spec for the VBO)
    // VBO: Vertex Buffer Object (where the data is)
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);

    // bind the VAO and VBO
    // binding: telling OpenGL what VAO/VBO we are using
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    // populate the VBO with our vertices
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // create IBO (Index Buffer Object)
    glGenBuffers(1, &mesh->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);

    // populate IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), (GLvoid*)0); // 3 -> "3 components: x, y, z"
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // 3 -> "3 components: r, g, b"
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // cleanup
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint compileShader(GLuint type, const char* source) {
    GLuint shader;

    switch (type)
    {
    case GL_VERTEX_SHADER:
    case GL_FRAGMENT_SHADER:
        shader = glCreateShader(type);
        break;
    
    default:
        break;
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // get status, log errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Failed to compile " << type << " shader: " << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint shaderProgram = glCreateProgram();

    // compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // attach shaders to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Validate program
    glValidateProgram(shaderProgram);

    // cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void createGraphicsPipeline() {
    // load shaders
    const std::string vertexShaderSource = loadShaderFromFile("res/shaders/vertex.glsl");
    const std::string fragmentShaderSource = loadShaderFromFile("res/shaders/fragment.glsl");

    app.shaderProgram = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}

int main() {

    // initialise
    if (initialiseProgram() == -1) {
        return -1;
    }

    // setup up the pipeline (shaders etc)
    createGraphicsPipeline();

    // setup the camera
    app.camera.setPerspective(glm::radians(90.f), (float)app.WINDOW_WIDTH/(float)app.WINDOW_HEIGHT, 0.1f, 10.0f);

    // create cubes
    meshVertexSpec(&mesh1);
    meshVertexSpec(&mesh2);

    // translate meshes
    meshTranslate(&mesh1, -0.5f, 0, -1);
    meshTranslate(&mesh2, 0.5f, 0, -1);

    mainLoop();
    cleanup(&mesh1);
    cleanup(&mesh2);

    return 0;
}
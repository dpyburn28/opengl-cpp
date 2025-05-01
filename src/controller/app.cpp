#include "app.h"

App::App() {
    set_up_glfw();
}

App::~App() {
    glDeleteProgram(shader);

    delete animationSystem;
    delete motionSystem;
    delete cameraSystem;
    delete renderSystem;

    glfwTerminate();
}

void App::run() {
 
    while (!glfwWindowShouldClose(window)) {
        
        motionSystem->update(
          transformComponents, physicsComponents, 16.67f / 1000.0f);
        bool should_close = cameraSystem->update(
          transformComponents, cameraID, *cameraComponent, 16.67f / 1000.0f);
        
        if (should_close) {
            break;
        }

        animationSystem->update(animationComponents, 16.667f);

        renderSystem->update(transformComponents, renderComponents, animationComponents);
    }    
}

void App::set_up_glfw() {
    
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

    window = glfwCreateWindow(640, 480, "hello window", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }  

}

void App::set_up_opengl() {

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    // set the rendering region to the actual screen size
    int w,h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    shader = make_shader(
        "../src/shaders/vertex.glsl", 
        "../src/shaders/fragment.glsl");

    glUseProgram(shader);
    unsigned int projLocation = glGetUniformLocation(shader, "projection");
    glm::mat4 projection = glm::perspective(
        45.0f, 640.0f / 480.0f, 0.1f, 10.0f);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void App::make_systems() {
    animationSystem = new AnimationSystem();
    motionSystem = new MotionSystem();
    cameraSystem = new CameraSystem(shader, window);
    renderSystem = new RenderSystem(shader, window);
}
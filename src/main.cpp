#include "config.h"
#include "cube.h"
#include "camera.h"

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

unsigned int make_module(const std::string& filepath, unsigned int module_type);

GLFWwindow* set_up_glfw() {

	GLFWwindow* window;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	return window;
}

void set_up_opengl(GLFWwindow* window) {
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

	// set rendering region to actual screen size
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

int main() {

	if (!glfwInit()) {
		std::cout << "GLFW couldn't start" << std::endl;
		return -1;
	}

	GLFWwindow* window = set_up_glfw();

	// Load OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

	set_up_opengl(window);

	// make objects
	Cube* cube = new Cube({3.0f, 0.0f, 0.25f}, {0.25f, 0.25f, 0.25f});
	Camera* player = new Camera({0.0f, 0.0f, 1.0f});

	unsigned int shader = make_shader(
		"../src/shaders/vertex.glsl", 
		"../src/shaders/fragment.glsl"
	);

	// configure shader
	glUseProgram(shader);
	unsigned int view_location = glGetUniformLocation(shader, "view");
	unsigned int proj_location = glGetUniformLocation(shader, "projection");
	glm::mat4 projection = glm::perspective(
		45.0f, 640.0f / 480.0f, 0.1f, 10.0f
	);
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(projection));


	while (!glfwWindowShouldClose(window)) {

		// keys
				glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			dPos.x += 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			dPos.y -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			dPos.x -= 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			dPos.y += 1.0f;
		}
		if (glm::length(dPos) > 0.1f) {
			dPos = glm::normalize(dPos);
			player->move(dPos);
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			break;
		}

		// mouse
		glm::vec3 dEulers = {0.0f, 0.0f, 0.0f};
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwSetCursorPos(window, 320.0, 240.0);
		
		glfwPollEvents();

		dEulers.z = -0.01f * static_cast<float>(mouse_x - 320.0);
		dEulers.y = -0.01f * static_cast<float>(mouse_y - 240.0);

		player->spin(dEulers);

		cube->update(16.67f / 1000.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);
		glUniformMatrix4fv(
			view_location, 1, GL_FALSE, 
			glm::value_ptr(player->get_view_transform()));

		cube->draw(shader);

		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);
	delete cube;
	delete player;
	glfwTerminate();
	return 0;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {

	std::vector<unsigned int> modules;
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for(unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader Linking error:\n" << errorLog << std::endl;
	}

	for(unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shader;	
}

unsigned int make_module(const std::string& filepath, unsigned int module_type) {

	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line)) {
		bufferedLines << line << "\n";
	}
	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
	}

	return shaderModule;
}

#include "shader.h"

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {

    // to store all the shader modules
    std::vector<unsigned int> modules;

    // add a vertex shader module
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));

    // add a fragment shader module
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));
    
    // attach all the modules then link the program
    unsigned int shader = glCreateProgram();
    for(unsigned int shaderModule : modules) {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    // check the linking worked
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "Shader linking error:\n" << errorLog << '\n';
    }

    // modules are now unneeded and can be freed
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
        // std::cout << line << std::endl;
		bufferedLines << line << '\n';
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    // check the compilation worked
    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
    }

    return shaderModule;
}
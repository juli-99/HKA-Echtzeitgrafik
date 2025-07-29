#include "Shader.hpp"



std::string Shader::loadShaderSource(std::string filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to oben the Fail: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


GLint Shader::createShaderPipline()
{
        std::filesystem::path FileFrag = fs::path(ROOT_DIR) / "res/shader.frag";
        std::filesystem::path FileVert = fs::path(ROOT_DIR) / "res/shader.vert";

        std::string reVertexSource = loadShaderSource(FileVert.string());
        std::string reFragmentSource = loadShaderSource(FileFrag.string());


        if (reVertexSource.empty()) {
            std::cerr << "Vertex shader source is empty! Path: " << FileVert << std::endl;
        }
        if (reFragmentSource.empty()) {
            std::cerr << "Fragment shader source is empty! Path: " << FileFrag << std::endl;
        }

        const GLchar* vertexSource = reVertexSource.c_str();
        const GLchar* fragmentSource = reFragmentSource.c_str();



   
        /* Vertex shader */
        GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);
        GLint success;
        GLchar infoLog[INFOLOG_LEN];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, INFOLOG_LEN, NULL, infoLog);
            printf("Vertex-Shader compilation failed\n%s\n", infoLog);
        }

        /* Fragment shader */
        GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, INFOLOG_LEN, NULL, infoLog);
            printf("Fragment-Shader compilation failed\n%s\n", infoLog);
        }

        /* Link shaders */
        GLint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, INFOLOG_LEN, NULL, infoLog);
            printf("Shader linking failed\n%s\n", infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
}




void Shader::setUniform(GLint location, int value)
{

}

void Shader::setUniform(GLint location, float value)
{

}

void Shader::setUniform(GLint location, const glm::vec3& value)
{

}

void Shader::setUniform(GLint location, const glm::vec4& value)
{

}

void Shader::setUniform(GLint location, const glm::mat4& value)
{

}

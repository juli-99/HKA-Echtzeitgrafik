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


void Shader::createShaderPipline(std::filesystem::path fileFrag, std::filesystem::path fileVert)
{
        

        std::string reVertexSource = loadShaderSource(fileVert.string());
        std::string reFragmentSource = loadShaderSource(fileFrag.string());


        if (reVertexSource.empty()) {
            std::cerr << "Vertex shader source is empty! Path: " << fileVert << std::endl;
        }
        if (reFragmentSource.empty()) {
            std::cerr << "Fragment shader source is empty! Path: " << fileFrag << std::endl;
        }

        const GLchar* vertexSource = reVertexSource.c_str();
        const GLchar* fragmentSource = reFragmentSource.c_str();

   
        /* Vertex shader */
        GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); //-> Create Vertex Shader for Object
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader); //Compile in machine code for cpu

        //Check for success
        GLint success;
        GLchar infoLog[INFOLOG_LEN];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, INFOLOG_LEN, NULL, infoLog);
            printf("Vertex-Shader compilation failed\n%s\n", infoLog);
        }

        /* Fragment shader */
        GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //-> Creates Fragment Shader for Color
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        //Check for success
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

        glObjectLabel(GL_PROGRAM, shaderProgram, -1, "Shader Program"); //Debug
        
        this->shaderProgram = shaderProgram;
}


GLint Shader::getUniformLoc(const char* name)
{
    return glGetUniformLocation(this->shaderProgram, name);
}

void Shader::setUniform(GLint location, int value)
{
    glUniform1i(location, value);
}

void Shader::setUniform(GLint location, float value)
{
    glUniform1f(location, value);
}

void Shader::setUniform(GLint location, const glm::vec3& value)
{
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform(GLint location, const glm::vec4& value)
{
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform(GLint location, const glm::mat4& value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::use()
{
    glUseProgram(this->shaderProgram);
}

GLint Shader::getShaderProgram() {
    return this->shaderProgram;
}

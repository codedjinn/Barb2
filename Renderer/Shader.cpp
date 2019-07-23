#include "Shader.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace RendererLib;

Shader::Shader(const std::string& fileName, GLenum shaderType)
{
	isCompiled = false;

	type = shaderType;

	VerifyAndGenerateShaderHandle();

	// Read shader source code, upload, and compile
	const char* sourceCode = ReadShader(fileName);

	glShaderSource(handle, 1, (const GLchar**)(&sourceCode), NULL);

	delete [] sourceCode;

	Compile();
}

Shader::Shader(GLenum shaderType)
{
	isCompiled = false;

	type = shaderType;

	VerifyAndGenerateShaderHandle();
}

Shader::~Shader()
{
	if (glIsShader(handle))
	{
		glDeleteShader(handle);
	}
}

void Shader::VerifyAndGenerateShaderHandle()
{
	assert(type != GL_VERTEX_SHADER ||
		   type != GL_FRAGMENT_SHADER);

	handle = glCreateShader(type);
}

void Shader::Compile()
{
    assert(glIsShader(handle));

    std::cerr << "Shader::compiling: ";

    GLint compilationSuccessful;

    glCompileShader(handle);

    GetParameter(GL_COMPILE_STATUS, &compilationSuccessful);
    if (GL_TRUE == compilationSuccessful)
    {
        isCompiled = true;

        std::cerr << "success" << std::endl << std::endl;
    }
    else
    {
        isCompiled = false;
        std::cerr << "failed, stopping execution and outputting shader log" << std::endl;

        std::string shaderLog;
        GetShaderLog(shaderLog);
        std::cerr << shaderLog << std::endl;

        exit(1);
    }
}

bool Shader::GetCompiled() const
{
    return isCompiled;
}

void Shader::SetShaderSource(std::string& code) const
{
    const char* sourceCode = code.c_str();

    glShaderSource(handle, 1, (const GLchar**)&sourceCode, NULL);
}

void Shader::GetShaderLog(std::string& log) const
{
    assert(GL_TRUE == glIsShader(handle));

    GLchar* infoLog;

    GLint infoLogLength;
    GetParameter(GL_INFO_LOG_LENGTH, &infoLogLength);

    infoLog = new GLchar[infoLogLength];

    glGetShaderInfoLog(handle, infoLogLength, &infoLogLength, infoLog);

    log = infoLog;

    delete [] infoLog;
}

void Shader::GetShaderSource(std::string& source) const
{
    assert(GL_TRUE == glIsShader(handle));

    GLchar* sourceCode;

    GLint sourceCodeLength;
    GetParameter(GL_SHADER_SOURCE_LENGTH, &sourceCodeLength);

    sourceCode = new GLchar[sourceCodeLength];
    glGetShaderSource(handle, sourceCodeLength, &sourceCodeLength, sourceCode);

    source = sourceCode;
    delete [] sourceCode;
}

void Shader::GetParameter(GLenum param, GLint* data) const
{
    assert(GL_TRUE == glIsShader(handle));
    glGetShaderiv(handle, param, data);
}

char* Shader::ReadShader(const std::string& fileName)
{
    std::cerr << "Shader::loading shader file " << fileName << ": ";
    std::string programString;
    std::ifstream shaderFile;

    // Check file extensions
    std::string shaderFileExtension = fileName.substr(fileName.size() - 3, 3);

    if (GL_VERTEX_SHADER == type && ".vp" != shaderFileExtension)
    {
        std::cerr << "attempting to load vertex shader source from a non .vp file ... ";
    }

    if (GL_FRAGMENT_SHADER == type && ".fp" != shaderFileExtension)
    {
        std::cerr << "attempting to load fragment shader source from a non .fp file ... ";
    }

    // Open file
    shaderFile.open(fileName.c_str());
    if (!shaderFile.is_open())
    {
        std::cerr << "failed [unable to open source file]" << std::endl;
        exit(1);
    }

    // Parse file contents
    while (!shaderFile.eof())
    {
        char currentChar;
        shaderFile.get(currentChar);

        programString += currentChar;

        shaderFile.peek();
    }

    shaderFile.close();

    std::cerr << "success" << std::endl;

    char* sourceCodeString  = new char[programString.size() + 1];
    memcpy(sourceCodeString, programString.c_str(), programString.size() + 1);

    return sourceCodeString;
}

GLuint Shader::GetHandle() const
{
	return handle;
}

GLuint Shader::GetType() const
{
	return type;
}

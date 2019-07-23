#include "ShaderProgram.h"
#include <cassert>
#include <iostream>
#include <cstdlib>

namespace RendererLib
{
GLuint ShaderProgram::currentlyActiveShaderProgram = 0;
}

using namespace RendererLib;

ShaderProgram::ShaderProgram(Shader* vertexShader, Shader* fragmentShader)
{
	std::cerr << "ShaderProgram::creating a new shader program: ";

	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;

	handle = glCreateProgram();
	assert(handle);					// glCreateProgram() returns 0 if unsucessful.

	// At least one shader must be provided
	assert(NULL != vertexShader ||
		   NULL != fragmentShader);

	if (NULL != vertexShader)
	{
		assert(GL_VERTEX_SHADER == vertexShader->GetType());
		glAttachShader(handle, vertexShader->GetHandle());
	}

	if (NULL != fragmentShader)
	{
		assert(GL_FRAGMENT_SHADER == fragmentShader->GetType());
		glAttachShader(handle, fragmentShader->GetHandle());
	}

	isLinked = false;

	std::cerr << "success" << std::endl << std::endl;
}

ShaderProgram::~ShaderProgram()
{
	if (currentlyActiveShaderProgram == handle)
	{
		currentlyActiveShaderProgram = 0;
		glUseProgram(0);
	}

	// Detach and destroy shaders
	if (NULL != vertexShader)
	{
		glDetachShader(handle, vertexShader->GetHandle());
		delete vertexShader;
	}
	if (NULL != fragmentShader)
	{
		glDetachShader(handle, fragmentShader->GetHandle());
		delete fragmentShader;
	}

	glDeleteProgram(handle);
}

void ShaderProgram::Link()
{
	std::cerr << "ShaderProgram::linking program object: ";

	// Optionally, compile the vertex shader if it isn't already compiled
	if (NULL != vertexShader && !(vertexShader->GetCompiled()))
	{
		vertexShader->Compile();
	}

	// Optionally, compile the fragment shader if it isn't already compiled
	if (NULL != fragmentShader && !(fragmentShader->GetCompiled()))
	{
		fragmentShader->Compile();
	}

	// Attempt to link the shader(s)
	glLinkProgram(handle);

	GLint linkSuccessful;
	GetParameter(GL_LINK_STATUS, &linkSuccessful);
	if (linkSuccessful)
	{
		isLinked = true;

		std::cerr << "success" << std::endl;
	}
	else
	{
		isLinked = false;

		std::cerr << "failed, stopping execution and outputting log" << std::endl;

		std::string programLog;
		GetProgramLog(programLog);
		std::cerr << programLog << std::endl;
		exit(1);
	}

	std::cerr << std::endl;
}

void ShaderProgram::Use()
{
	// Optionally, link the program if not already done
	if (!isLinked)
	{
		Link();
	}

	glUseProgram(handle);

	currentlyActiveShaderProgram = handle;
}

void ShaderProgram::GetParameter(GLenum param, GLint* data) const
{
	glGetProgramiv(handle, param, data);
}

void ShaderProgram::GetProgramLog(std::string& log) const
{
	assert(GL_TRUE == glIsProgram(handle));

	GLchar* infoLog;

	GLint infoLogLength;
	GetParameter(GL_INFO_LOG_LENGTH, &infoLogLength);

	infoLog = new GLchar[infoLogLength];
	glGetProgramInfoLog(handle, infoLogLength, &infoLogLength, infoLog);

	log = infoLog;

	delete [] infoLog;
}


void ShaderProgram::SetUniform1f(const std::string& uniformName, GLfloat v0) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex = glGetUniformLocation(handle, uniformName.c_str());
	if (uniformIndex == -1)
	{
		int some = 0;
	}

	if (-1 == uniformIndex)
	{
		assert(-1 != uniformIndex);
	}

    glUniform1f(uniformIndex, v0);
}

void ShaderProgram::SetUniform2f(const std::string& uniformName, GLfloat v0, GLfloat v1) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform2f(uniformIndex, v0, v1);
}

void ShaderProgram::SetUniform2fv(const std::string& uniformName, int32_t count, GLfloat* v) const
{
	assert(handle == currentlyActiveShaderProgram);

	GLint uniformIndex	= glGetUniformLocation(handle, uniformName.c_str());
	assert(-1 != uniformIndex);

	glUniform2fv(uniformIndex, count, v);
}

void ShaderProgram::SetUniform3f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform3f(uniformIndex, v0, v1, v2);
}

void ShaderProgram::SetUniform4f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform4f(uniformIndex, v0, v1, v2, v3);
}

void ShaderProgram::SetUniform1i(const std::string& uniformName, GLint v0) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform1i(uniformIndex, v0);
}

void ShaderProgram::SetUniform2i(const std::string& uniformName, GLint v0, GLint v1) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform2i(uniformIndex, v0, v1);
}

void ShaderProgram::SetUniform3i(const std::string& uniformName, GLint v0, GLint v1, GLint v2) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform3i(uniformIndex, v0, v1, v2);
}

void ShaderProgram::SetUniform4i(const std::string& uniformName, GLint v0, GLint v1, GLint v2, GLint v3) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniform4i(uniformIndex, v0, v1, v2, v3);
}

void ShaderProgram::SetUniformMatrix2fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniformMatrix2fv(uniformIndex, 1, transpose, values);
}

void ShaderProgram::SetUniformMatrix3fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniformMatrix3fv(uniformIndex, 1, transpose, values);
}

void ShaderProgram::SetUniformMatrix4fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose) const
{
    assert(handle == currentlyActiveShaderProgram);

    GLint uniformIndex  = glGetUniformLocation(handle, uniformName.c_str());
    assert(-1 != uniformIndex);

    glUniformMatrix4fv(uniformIndex, 1, transpose, values);
}

GLuint ShaderProgram::GetProgramHandle() const
{
	return handle;
}

GLuint ShaderProgram::GetCurrentlyActiveProgramHandle()
{
	return currentlyActiveShaderProgram;
}

void ShaderProgram::DeactivateShaders()
{
    glUseProgram(0);
    currentlyActiveShaderProgram = 0;
}

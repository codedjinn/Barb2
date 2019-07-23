#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "RendererLibCommon.h"
#include <string>

namespace RendererLib
{

class Shader
{
	public:

		/**
		* Initializes the shader to a specific shader type and loads the
		* shaders' source code from a file specified by @param fileName.
		*/
		Shader(const std::string& fileName, GLenum shaderType);

		/**
		* Initializes the shader to a specific type. No source code is loaded
		* at this point.
		*/
		Shader(GLenum shaderType);

		~Shader();

		/**
		* Returns is the last compilation of this shader object was successful or not.
		*/
		bool GetIsCompiled() const;

		/**
		* Returns the OpenGL handle for this shader.
		*/
		GLuint GetHandle() const;

		/**
		* Returns the shader type of this shader object i.e GL_VERTEX_SHADER or
		* GL_FRAGMENT_SHADER etc
		*/
		GLuint GetType() const;

		/**
		* Attempts to compile this shader object.
		*/
		void Compile();

		/**
		* Sets this shader's source code.
		* @warning A recompile is needed after calling this method before you can
		*          successfully use the new shader.
		*/
		void SetShaderSource(std::string& code) const;

		/**
		* Retrieves the shader's log. It is stored in @param log upon
		* return.
		*/
		void GetShaderLog(std::string& log) const;

		/**
		* Retrieves the shader's source code. It is stored in @param source upon
		* return.
		*/
		void GetShaderSource(std::string& source) const;

		bool GetCompiled() const;

	protected:

		/**
		* Indicates the type of shader program this is. Options include
		* GL_VERTEX_SHADER
		* GL_FRAGMENT_SHADER
		*/
		GLenum type;

		/**
		* Handle to the shader object provided by OpenGL.
		*/
		GLuint handle;

		/**
		* Flag set if the compilation was successful.
		*/
		bool isCompiled;

		/**
		* Reads the shader source code from a file.
		* @warning Returns a heap-allocated char string which needs to be released after use.
		*/
		char* ReadShader(const std::string& fileName);

		void VerifyAndGenerateShaderHandle();

        /**
        * Wrapper around glGetShaderiv.
        */
		void GetParameter(GLenum param, GLint* data) const;
};

}

#endif // SHADER_H_INCLUDED

#ifndef SHADERPROGRAM_H_INCLUDED
#define SHADERPROGRAM_H_INCLUDED

#include "RendererLibCommon.h"
#include "Shader.h"

namespace RendererLib
{

class ShaderProgram
{
	public:

		/**
		* Constructor which accepts two shaders, one vertex and one fragment.
		* Assumes that @param vertexShader and @param fragmentShader are of the types GL_VERTEX_SHADER and GL_FRAGMENT_SHADER respectively.
		* @note	If a field is NULL then that stage of the pipeline will be fixed function, however, we require that at least one shader
		*		be loaded with this program.
		*/
		ShaderProgram(Shader* vertexShader, Shader* fragmentShader);

		/**
		* On destruction, ShaderProgram objects delete any shaders attached to them.
		*/
		~ShaderProgram();

		/**
		* Attempts to link this program object. If the attached shaders have not been compiled then this method will call their compile
		* methods. The linked flag will be set to either true or false depending on whether the link operation was successful or not.
		*/
		void Link();

		/**
		* Sets this program as the currently active shader program in OpenGL.
		* If the programs' linked flag is false, this method will attempt to link the program object before use.
		*/
		void Use();

		/**
		* Wrapper around the glGetProgramiv function.
		*/
		void GetParameter(GLenum, GLint* data) const;

		/**
		* Retrieves the programs' log, the programs' log is stored in @param programLog upon return.
		*/
		void GetProgramLog(std::string& programLog) const;

		GLuint GetProgramHandle() const;

		/**
		* Methods to set the uniform values of this shader program.
		* @pre
		* - The program must have undergone a successful link.
		* - The program must be the currently active shader program.
		* - The @param uniformName must be a valid uniform identifier.
		*/
        void SetUniform1f(const std::string& uniformName, GLfloat v0) const;
        void SetUniform2f(const std::string& uniformName, GLfloat v0, GLfloat v1) const;
        void SetUniform2fv(const std::string& uniformName, int32_t count, GLfloat* v) const;
        void SetUniform3f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2) const;
        void SetUniform4f(const std::string& uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
        void SetUniform1i(const std::string& uniformName, GLint v0) const;
        void SetUniform2i(const std::string& uniformName, GLint v0, GLint v1) const;
        void SetUniform3i(const std::string& uniformName, GLint v0, GLint v1, GLint v2) const;
        void SetUniform4i(const std::string& uniformName, GLint v0, GLint v1, GLint v2, GLint v3) const;
		void SetUniformMatrix2fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose = false) const;
		void SetUniformMatrix3fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose = false) const;
		void SetUniformMatrix4fv(const std::string& uniformName, const GLfloat* values, GLboolean transpose = false) const;

		/**
		* Retrieves the currently active shader program handle. 0 is returned if no shaders are currently active.
		*/
		static GLuint GetCurrentlyActiveProgramHandle();

		/**
		* Deactivates all OpenGL shader functionality.
		*/
		static void DeactivateShaders();

	protected:

		Shader* vertexShader;
		Shader* fragmentShader;

		/**
		* The OpenGL shader program handle.
		*/
		GLuint handle;

		/**
		* Tracks whether the most recent link operation was successful.
		*/
		bool isLinked;

		/**
		* Static to keep track of the currently active shader program. If no shader program is currently active (i.e shaders are disabled)
		* this value is 0.
		*/
		static GLuint currentlyActiveShaderProgram;
};

}


#endif // SHADERPROGRAM_H_INCLUDED

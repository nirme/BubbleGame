#pragma once

#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <string>


constexpr static int loaderError = -200;





#define GL_ERROR_CHECK(glFunc) \
	{ \
		glFunc; \
		/*Logger::getSingleton().write(std::string("function run: ") + #glFunc + ", file: " + __FILE__ + ", line: " + std::to_string(__LINE__) );*/ \
		GLenum err = glGetError(); \
		if (err != GL_NO_ERROR) \
		{ \
			switch (err) \
			{ \
				case GL_INVALID_ENUM: \
					throw std::runtime_error(#glFunc " function failed with error GL_INVALID_ENUM"); \
				case GL_INVALID_VALUE: \
					throw std::runtime_error(#glFunc " function failed with error GL_INVALID_VALUE"); \
				case GL_INVALID_OPERATION : \
					throw std::runtime_error(#glFunc " function failed with error GL_INVALID_OPERATION"); \
				case GL_INVALID_FRAMEBUFFER_OPERATION : \
					throw std::runtime_error(#glFunc " function failed with error GL_INVALID_FRAMEBUFFER_OPERATION"); \
				case GL_OUT_OF_MEMORY : \
					throw std::runtime_error(#glFunc " function failed with error GL_OUT_OF_MEMORY"); \
				default: \
					throw std::runtime_error(std::string(#glFunc " function failed with error: ") + std::to_string(err)); \
			} \
		} \
	}\


#define EGL_ERROR_CHECK(eglFunc) \
	{ \
		eglFunc; \
		GLenum err = eglGetError(); \
		if (err != EGL_SUCCESS) \
        { \
            switch (err) \
            { \
                case EGL_NOT_INITIALIZED: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_NOT_INITIALIZED"); \
                case EGL_BAD_ACCESS: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_ACCESS"); \
                case EGL_BAD_ALLOC: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_ALLOC"); \
                case EGL_BAD_ATTRIBUTE: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_ATTRIBUTE"); \
                case EGL_BAD_CONTEXT: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_CONTEXT"); \
                case EGL_BAD_CONFIG: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_CONFIG"); \
                case EGL_BAD_CURRENT_SURFACE: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_CURRENT_SURFACE"); \
                case EGL_BAD_DISPLAY: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_DISPLAY"); \
                case EGL_BAD_SURFACE: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_SURFACE"); \
                case EGL_BAD_MATCH: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_MATCH"); \
                case EGL_BAD_PARAMETER: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_PARAMETER"); \
                case EGL_BAD_NATIVE_PIXMAP: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_NATIVE_PIXMAP"); \
                case EGL_BAD_NATIVE_WINDOW: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_BAD_NATIVE_WINDOW"); \
                case EGL_CONTEXT_LOST: \
                    throw std::runtime_error(#eglFunc " function failed with error EGL_CONTEXT_LOST"); \
            } \
        } \
	}\



/*
class GlException : public std::exception
{
protected:
	const char errorMsg[4096];

public:
	GlException(GLenum _error) :
		error(_error)
	{
		switch (_error)
		{
			errorMsg = "test";
			GL_INVALID_ENUM

				GL_INVALID_VALUE

				GL_INVALID_OPERATION

				GL_INVALID_FRAMEBUFFER_OPERATION
				The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete(i.e.the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE).The offending command is ignored and has no other side effect than to set the error flag.

				GL_OUT_OF_MEMORY

		}
		errorMsg = "test";
	};
	virtual const char* what() const noexcept;
};


*/



/*

class NotImplementedException : public std::logic_error
{
public:
	NotImplementedException() : std::logic_error("Function not yet implemented.") {};
};


namespace Graphic
{

	class ApplicationException : public std::exception
	{
	protected:
		std::string errorMsg;

	public:
		ApplicationException(std::string _errorMsg)
			: errorMsg(_errorMsg)
		{};

		virtual const char* what() const noexcept
		{
			return errorMsg.c_str();
		};
	};

	class AndroidException : public ApplicationException
	{
	protected:
		int32_t error;

	public:
		AndroidException(int32_t _error, std::string _functionName)
			: ApplicationException("Android function '" + _functionName + "' failed with code: "), error(_error)
		{
			//errorMsg += std::to_string(_error);

			char num[12];
			sprintf(num, "%#08X", error);
			errorMsg += num;
		};
	};



	class EGLException : public std::exception
	{
	protected:
		EGLint error;
		const char* errorMsg;

	public:
		EGLException(EGLint _error) {};
		virtual const char* what() const noexcept {};
	};



	class GlesException : public std::exception
	{
	protected:
		GLenum error;
		const char* errorMsg;

	public:
		GlesException(GLenum _error);
		virtual const char* what() const noexcept;
	};

	class GlesExceptionWithInfo : public GlesException
	{
	protected:
		std::string infoLog;

	public:
		GlesExceptionWithInfo(std::string& _infoLog);
		const std::string& getInfoLog() const;
	};

	class GlesCompileError : public GlesExceptionWithInfo
	{
	public:
		GlesCompileError(std::string& _infoLog);
	};

	class GlesLinkingError : public GlesExceptionWithInfo
	{
	public:
		GlesLinkingError(std::string& _infoLog);
	};

}
*/

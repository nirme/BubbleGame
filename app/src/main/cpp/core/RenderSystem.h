#pragma once

#include <android_native_app_glue.h>
#include <android/configuration.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <stdint.h>
#include <list>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "SingletonTemplate.h"
#include "Exceptions.h"

#include "RenderStateCashe.h"

#include "Logger.h"
#include "Color.h"

#include "_2d/Renderable.h"
#include "GraphicBuffer.h"
#include "_2d/MaterialManager.h"


/*
   0---1
1  | \ |  2
   2---3

   0,2,3
   0,3,1

*/


typedef std::list<std::pair<GLenum, GLint>> ParamList;


namespace core 
{

	class RenderSystem
	{
	protected:

		bool initialized;

		android_app* androidApp;

		static constexpr EGLint windowSurfaceAttribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_ALPHA_SIZE, 0, //no alpha for render end surface
			EGL_BUFFER_SIZE, 24, //RGB8 = 24
			EGL_DEPTH_SIZE, 0, //no depth buffer needed for 2D
			EGL_CONFORMANT, EGL_OPENGL_ES2_BIT, //GLES2
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //GLES2
			EGL_NONE
		};

		typedef std::set<EGLint> AttribList;
		typedef std::unordered_map<unsigned int, EGLint> AttribMap;
		AttribList wndSurfAttrPreferHighest;

		static constexpr EGLint glContextAttribs[] = {
			//EGL_CONTEXT_CLIENT_VERSION, 2, //GLES2 - older version
			EGL_CONTEXT_MAJOR_VERSION, 2, //GLES2
			EGL_NONE
		};

		EGLint cashedWindowConfigID;

		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;


		unsigned short screenWidth, screenHeight;
		unsigned int screenDensity;

		bool hwMultisampling;

		ColorF bgColor;


		_2d::MaterialManager materialManager;

		GraphicBuffer batchingVertexBuffer;
		unsigned int batchedSprites;

		GraphicBuffer singleVertexBuffer;
		GraphicBuffer indexBuffer;


		std::shared_ptr<RenderStateCashe> state;



		void createBuffers(unsigned int _bufferSize = 8192); //default to 8k - around 250 sprites ber batch
		void deleteBuffers();

		void applyVertexAttribs(const ShadingProgram::VertexAttribList &_attribList);


	public:

		static constexpr unsigned int BaseScreenDensity = ACONFIGURATION_DENSITY_MEDIUM;


		RenderSystem();

		// enable multisampling
		void enableMultisampling(bool _multisampling);

		bool initialize(android_app* _androidApp = nullptr);
		void uninitialize();
		void reinitialize();


		inline bool hasContext() const
		{
			return context != nullptr;
		};


		RenderStateCashe *getStateCashe();

		unsigned short getScreenWidth();
		unsigned short getScreenHeight();
		unsigned int getScreenDensity();



		void notifyOnContextLost()
		{};

		void notifyOnContextAquired()
		{};


		GraphicBuffer &getBatchingVertexBuffer()
		{
			return batchingVertexBuffer;
		};

		GraphicBuffer &getIndexVertexBuffer()
		{
			return indexBuffer;
		};



		void beforeRendering()
		{
            try
            {
                GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
                //GL_ERROR_CHECK(glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha));
                GL_ERROR_CHECK(glClearColor(0.2f, 0.6f, 0.2f, 1.0f));
                GL_ERROR_CHECK(glClear(clearMask));
			}
            catch (const std::exception &e)
            {
                Logger::getSingleton().write(e.what(), LL_ERROR);
            }
		};


		void afterRendering()
		{
            try
            {
            	if (eglSwapBuffers(display, surface) != EGL_TRUE)
					throw std::runtime_error("eglSwapBuffers function failed"); \
                //EGL_ERROR_CHECK(eglSwapBuffers(display, surface));
            }
            catch (const std::exception &e)
            {
                Logger::getSingleton().write(e.what(), LL_ERROR);
            }
		};



		void render(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);
		void singleRender(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);
		void bufferedRender(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);
		void flushRenderOp(const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);


	};

}
#include "RenderSystem.h"

namespace core
{

	void RenderSystem::createBuffers(unsigned int _bufferSize)
	{
		assert(hasContext() && "Cannot create batching buffer, no context available");

		batchingVertexBuffer = GraphicBuffer(this, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, GL_FLOAT);
		singleVertexBuffer = GraphicBuffer(this, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, GL_FLOAT);

		// round down to matching multiple of float size fitting in _bufferSize
		unsigned int vertexBuffSize = (unsigned int)(_bufferSize / sizeof(float));
		
		batchingVertexBuffer.resize(vertexBuffSize);
		batchingVertexBuffer.load();

		singleVertexBuffer.resize(vertexBuffSize);
		singleVertexBuffer.load();

		unsigned int indexBuffSize = (vertexBuffSize / (4*4)) + 1; //(amount of smallest sprites + 1)

		indexBuffer = GraphicBuffer(this, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_SHORT);
		indexBuffer.resize(indexBuffSize * 6);


		indexBuffer.load();

		// generate indices for use with batched sprites
		unsigned short _indices[6] = { 0, 2, 3, 0, 3, 1 };
			
		for (unsigned int i = 0; i < indexBuffSize; ++i)
		{
			indexBuffer.write(_indices, 6);

			_indices[0] += 4;
			_indices[1] += 4;
			_indices[2] += 4;
			_indices[3] += 4;
			_indices[4] += 4;
			_indices[5] += 4;
		}

		indexBuffer.uploadData();
	};


	void RenderSystem::deleteBuffers()
	{
		assert(!initialized && "Cannot delete batching buffer, renderer not initialized");

		batchingVertexBuffer.unload();
		batchingVertexBuffer.resize(0);

		singleVertexBuffer.unload();
		singleVertexBuffer.resize(0);

		indexBuffer.unload();
		indexBuffer.resize(0);
	};


    void RenderSystem::applyVertexAttribs(const ShadingProgram::VertexAttribList &_attribList)
    {
        for (unsigned int i = 0, iEnd = _attribList.size(); i < iEnd; ++i)
        {
            state->setVertexAtrib(_attribList[i].id);
            GL_ERROR_CHECK(glVertexAttribPointer(
                            _attribList[i].id,
                            _attribList[i].size,
                            _attribList[i].type,
                            GL_FALSE,           // normalized
                            _attribList[i].stride,
                            (void *)_attribList[i].offsetInBytes
            ));
        }
    };



    RenderSystem::RenderSystem() :
			initialized(false),
			androidApp(nullptr),
			cashedWindowConfigID(0),
			display(0),
			surface(0),
			context(0),
			screenWidth(0),
			screenHeight(0),
			screenDensity(0),
			hwMultisampling(false),
			batchedSprites(0),
            state(nullptr)
	{};


    void RenderSystem::enableMultisampling(bool _multisampling)
    {
        if (_multisampling)
        {
            wndSurfAttrPreferHighest.insert(EGL_SAMPLE_BUFFERS);
            wndSurfAttrPreferHighest.insert(EGL_SAMPLES);
        }
        else
        {
            wndSurfAttrPreferHighest.erase(EGL_SAMPLE_BUFFERS);
            wndSurfAttrPreferHighest.erase(EGL_SAMPLES);
        }

        if (hwMultisampling != _multisampling)
        {
            hwMultisampling = _multisampling;
            if (initialized)
            	reinitialize();
        }
    };


    bool RenderSystem::initialize(android_app* _androidApp)
	{
		if (initialized)
		{
			Logger::getSingleton().write("Render System already initialized");
			return true;
		}

		assert((_androidApp || androidApp ) && "android_app struct required to init rendering system");

        if (_androidApp)
            androidApp = _androidApp;


		try
		{
			EGLDisplay tmpDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

			EGLint eglVerMajor, eglVerMinor;
			EGL_ERROR_CHECK(eglInitialize(tmpDisplay, &eglVerMajor, &eglVerMinor));

			display = tmpDisplay;


			//check egl ver if needed

			EGLConfig eglConfig((EGLConfig)0);

			if (!cashedWindowConfigID)
			{
				EGLint numConfigs;
				EGL_ERROR_CHECK(eglChooseConfig(display, windowSurfaceAttribs, nullptr, 0, &numConfigs));

				std::vector<EGLConfig> configs(numConfigs);
				EGL_ERROR_CHECK(eglChooseConfig(display, windowSurfaceAttribs, configs.data(), numConfigs, &numConfigs));

				std::vector<EGLint> bestConfig;


				for (int i = 0; i < numConfigs; ++i)
				{
					const EGLint *attributePair = windowSurfaceAttribs;
					EGLint attributeValue;

					bool configMatching = true;
					while (attributePair[0] != EGL_NONE)
					{
						EGL_ERROR_CHECK(eglGetConfigAttrib(display, configs[i], attributePair[0], &attributeValue));

						// bitwise check for bitmasked vals
						if (attributePair[0] == EGL_RENDERABLE_TYPE || attributePair[0] == EGL_SURFACE_TYPE || attributePair[0] == EGL_CONFORMANT)
						{
							if (!(attributePair[1] & attributeValue))
							{
								configMatching = false;
								break;
							}
						}
						else if (attributePair[1] != attributeValue)
						{
							configMatching = false;
							break;
						}

						attributePair += 2;
					}

					if (configMatching)
					{
						std::vector<EGLint> attributesList(wndSurfAttrPreferHighest.size());

						int j = 0;
						for (auto it = wndSurfAttrPreferHighest.begin(), itEnd = wndSurfAttrPreferHighest.end();
							it != itEnd;
							++it, ++j)
						{
							EGL_ERROR_CHECK(eglGetConfigAttrib(display, configs[i], (*it), &(attributesList[j])));
						}


						if (!eglConfig ||
                            (eglConfig && bestConfig < attributesList))
						{
							bestConfig.swap(attributesList);
							eglConfig = configs[i];
						}
					}
				}

				if (!eglConfig)
					throw std::runtime_error("No frame buffer configuration matching selected attributes");

				// cache config id for quick resume
				EGL_ERROR_CHECK(eglGetConfigAttrib(display, eglConfig, EGL_CONFIG_ID, &cashedWindowConfigID));
			}
			else
			{
				EGLint windowAttribConfigID[] = {
					EGL_CONFIG_ID, cashedWindowConfigID,
					EGL_NONE
				};

				EGL_ERROR_CHECK(eglChooseConfig(display, windowAttribConfigID, &eglConfig, 1, nullptr));
			}


			EGLint nativeVisualFormat;
			EGL_ERROR_CHECK(eglGetConfigAttrib(display, eglConfig, EGL_NATIVE_VISUAL_ID, &nativeVisualFormat));


			// native window
			{
				ANativeWindow* window(nullptr);
				// leave w/h as 0 to reset to window base
				int32_t res = ANativeWindow_setBuffersGeometry(androidApp->window, 0, 0, nativeVisualFormat);
				if (res < 0)
				{
					std::string message("ANativeWindow_setBuffersGeometry failed with return value ");
					message += res;
					throw std::runtime_error(message);
				}
			}


			// egl surface
			{
				EGLSurface tmpSurface;
				EGL_ERROR_CHECK(tmpSurface = eglCreateWindowSurface(display, eglConfig, androidApp->window, NULL));
				surface = tmpSurface;
			}

			// egl context
			{
				EGLContext tmpContext;
				EGL_ERROR_CHECK(tmpContext = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, glContextAttribs));
				context = tmpContext;
			}

			if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
			{
				throw std::runtime_error("eglMakeCurrent function failed"); \
			}


			EGLint width(0), height(0);
			EGL_ERROR_CHECK(eglQuerySurface(display, surface, EGL_WIDTH, &width));
			EGL_ERROR_CHECK(eglQuerySurface(display, surface, EGL_HEIGHT, &height));

			// get density and save screen size
            screenDensity = AConfiguration_getDensity(androidApp->config);
            if (!screenDensity)
                screenDensity = ACONFIGURATION_DENSITY_MEDIUM;

			screenWidth = width;
			screenHeight = height;

			// Initialize GL state.
            state = std::shared_ptr<RenderStateCashe>(new RenderStateCashe());
//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//glClear(GL_COLOR_BUFFER_BIT);
			state->getCurrentState();
glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);

			// generate batch buffer
			createBuffers();
//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//glClear(GL_COLOR_BUFFER_BIT);
		}
		catch (const std::exception &e)
		{
			// egl/gles setup failed
			Logger::getSingleton().write(e.what(), LL_ERROR);

			screenWidth = 0;
			screenHeight = 0;

			if (context)
			{
				eglDestroyContext(display, context);
				eglGetError();
				context = (EGLContext)0;
			}

			if (surface)
			{
				eglDestroySurface(display, surface);
				eglGetError();
				surface = (EGLSurface)0;
			}

			androidApp->window = nullptr;

			if (display)
			{
				eglTerminate(display);
				eglGetError();
				display = (EGLDisplay)0;
			}

			// quit the function 
			return false;
		}

		initialized = true;
		return true;
	};



	void RenderSystem::uninitialize()
	{
		if (!initialized)
		{
			Logger::getSingleton().write("Render System already uninitialized");
			return;
		}

		initialized = false;

		deleteBuffers();

		screenWidth = 0;
		screenHeight = 0;
		androidApp->window = nullptr;

		if (context)
		{
			eglDestroyContext(display, context);
			context = (EGLContext)0;
			notifyOnContextLost();
		}

		if (surface)
		{
			eglDestroySurface(display, surface);
			surface = (EGLSurface)0;
		}

		if (display)
		{
			eglTerminate(display);
			display = (EGLDisplay)0;
		}


		notifyOnContextLost();

		// cleanup any errors that might have occured
		while (eglGetError() != EGL_SUCCESS);
	};



	void RenderSystem::reinitialize()
	{
		if (initialized)
			uninitialize();

		initialize();
	};




	RenderStateCashe *RenderSystem::getStateCashe()
	{
		assert(state);
		return state.get();
		//return state;
	};

	unsigned short RenderSystem::getScreenWidth()
	{
		return screenWidth;
	};
	unsigned short RenderSystem::getScreenHeight()
	{
		return screenHeight;
	};

	unsigned int RenderSystem::getScreenDensity()
	{
		return screenDensity;
	};






	void RenderSystem::render(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough)
	{
	    try
        {
            if (_renderable->isBufferable())
            {
                bufferedRender(_renderable, _program, _paramsPassthrough);
            }
            else
            {
                singleRender(_renderable, _program, _paramsPassthrough);
            }
		}
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
        }
	};


	void RenderSystem::singleRender(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough)
	{
		const ShadingProgram::VertexAttribList &attribs = _program->getAttribList();
		unsigned int spritesWritten = 0;
		_2d::BuffWriteResult res({0, false});

        try
        {
            state->setShadingProgram(_program->getId());

            state->setVertexBuffer(batchingVertexBuffer.getId());
            state->setIndexBuffer(indexBuffer.getId());

            // set vertex attribs and apply shader values
            for (unsigned int i = 0, iEnd = attribs.size(); i < iEnd; ++i)
            {
                state->setVertexAtrib(attribs[i].id);
                GL_ERROR_CHECK(glVertexAttribPointer(
                        attribs[i].id,
                        attribs[i].size,
                        attribs[i].type,
                        GL_FALSE,           // normalized
                        attribs[i].stride,
                        (void *)attribs[i].offsetInBytes
                ));
            }

            state->applyState();

            _program->getParams()->applyUniformValues(_paramsPassthrough);
        }
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
        }

        do
		{
			res = _renderable->writeVertexData(singleVertexBuffer, res.nextSpriteIndex);
			spritesWritten = res.nextSpriteIndex - spritesWritten;

			// upload written data
			singleVertexBuffer.uploadData();

            try
            {
                glDrawElements(GL_TRIANGLES, spritesWritten * 6, indexBuffer.getElementType(), nullptr);
            }
            catch (const std::exception &e)
            {
                Logger::getSingleton().write(e.what(), LL_ERROR);
            }
			// clear buffer when done
			singleVertexBuffer.rewind();
		}
		// loop untill everything is drawn
		while (!res.operComplete);
    };


	void RenderSystem::bufferedRender(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough)
	{
		_2d::BuffWriteResult res = _renderable->writeVertexData(batchingVertexBuffer, 0);
		batchedSprites += res.nextSpriteIndex;

		while (!res.operComplete)
		{
			//buffer full, need flushing to write more
			flushRenderOp(_program, _paramsPassthrough);

			res = _renderable->writeVertexData(batchingVertexBuffer, res.nextSpriteIndex);
			batchedSprites += res.nextSpriteIndex;
		}
	};


	void RenderSystem::flushRenderOp(const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough)
	{
		if (!batchedSprites)
			return;

		const ShadingProgram::VertexAttribList &attribs = _program->getAttribList();

		// upload written data
		batchingVertexBuffer.uploadData();

        // set buffers and apply cashed state
        try
        {
            state->setShadingProgram(_program->getId());

            state->setVertexBuffer(batchingVertexBuffer.getId());
            state->setIndexBuffer(indexBuffer.getId());

            // set vertex attribs and apply shader values
            for (unsigned int i = 0, iEnd = attribs.size(); i < iEnd; ++i)
            {
                state->setVertexAtrib(attribs[i].id);
                GL_ERROR_CHECK(glVertexAttribPointer(
                        attribs[i].id,
                        attribs[i].size,
                        attribs[i].type,
                        GL_FALSE,           // normalized
                        attribs[i].stride,
                        (void *)attribs[i].offsetInBytes
                        ));
            }

            state->applyState();

            _program->getParams()->applyUniformValues(_paramsPassthrough);

            GL_ERROR_CHECK(glDrawElements(GL_TRIANGLES, batchedSprites * 6, indexBuffer.getElementType(), nullptr));
        }
        catch (const std::exception &e)
        {
            Logger::getSingleton().write(e.what(), LL_ERROR);
        }

        // clear buffer when done
		batchingVertexBuffer.rewind();
		batchedSprites = 0;
	};

}

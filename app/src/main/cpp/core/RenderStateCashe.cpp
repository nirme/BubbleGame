#include "RenderStateCashe.h"



namespace core
{
	//RenderStateCashe::

	void RenderStateCashe::getCurrentState()
	{
		GLboolean valueBool;
		GLint valueInt;

		try
		{
			//blending
			GL_ERROR_CHECK(glGetBooleanv(GL_BLEND, &valueBool));
			currentRenderState.blending = (valueBool == GL_TRUE);


			//blending values
			GL_ERROR_CHECK(glGetIntegerv(GL_BLEND_SRC_RGB, &valueInt));
			//glGetIntegerv(GL_BLEND_SRC_ALPHA, &valueInt);
			currentRenderState.blendingSfactor = valueInt;

			GL_ERROR_CHECK(glGetIntegerv(GL_BLEND_DST_RGB, &valueInt));
			//glGetIntegerv(GL_BLEND_DST_ALPHA, &valueInt);
			currentRenderState.blendingDfactor = valueInt;

			GL_ERROR_CHECK(glGetIntegerv(GL_BLEND_EQUATION_RGB, &valueInt));
			//glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &valueInt);
			currentRenderState.blendingMode = valueInt;


			//face culling
			GL_ERROR_CHECK(glGetBooleanv(GL_CULL_FACE, &valueBool));
			currentRenderState.faceCulling = (valueBool == GL_TRUE);

			//face culling mode
			GL_ERROR_CHECK(glGetIntegerv(GL_CULL_FACE_MODE, &valueInt));
			currentRenderState.faceCullingMode = valueInt;


			//depth test
			GL_ERROR_CHECK(glGetBooleanv(GL_DEPTH_TEST, &valueBool));
			currentRenderState.depthTest = (valueBool == GL_TRUE);

			//depth range
			GLfloat depthRange[2];
			GL_ERROR_CHECK(glGetFloatv(GL_DEPTH_RANGE, depthRange));
			currentRenderState.depthTestNearVal = depthRange[0];
			currentRenderState.depthTestFarVal = depthRange[1];

			//depth function
			GL_ERROR_CHECK(glGetIntegerv(GL_DEPTH_FUNC, &valueInt));
			currentRenderState.depthTestFunction = valueInt;


			//dither
			GL_ERROR_CHECK(glGetBooleanv(GL_DITHER, &valueBool));
			currentRenderState.dither = (valueBool == GL_TRUE);


			//scissor test
			GL_ERROR_CHECK(glGetBooleanv(GL_SCISSOR_TEST, &valueBool));
			currentRenderState.scissorTest = (valueBool == GL_TRUE);

			//scissor test values
			GLint scissorTestValues[4];
			GL_ERROR_CHECK(glGetIntegerv(GL_SCISSOR_BOX, scissorTestValues));
			currentRenderState.scissorTestX = scissorTestValues[0];
			currentRenderState.scissorTestY = scissorTestValues[1];
			currentRenderState.scissorTestWidth = scissorTestValues[2];
			currentRenderState.scissorTestHeight = scissorTestValues[3];

		}
		catch (const std::exception &e)
		{
			Logger::getSingleton().write(e.what(), LL_ERROR);
			throw;
		}


		currentRenderState.shadingProgram = 0;
		currentRenderState.activeVertexAttribs = 0;

		memset(currentRenderState.texture2D, 0, sizeof(currentRenderState.texture2D));

		//copy to new state as base
		memcpy(&newRenderState, &currentRenderState, sizeof(newRenderState));
	};


	void RenderStateCashe::setDefaultBlending(bool _blending, GLenum _blendingSfactor, GLenum _blendingDfactor, GLenum _blendingMode)
	{
		defaultRenderState.blending = _blending;

		if (_blendingSfactor > 0)
			newRenderState.blendingSfactor = _blendingSfactor;

		if (_blendingDfactor > 0)
			newRenderState.blendingDfactor = _blendingDfactor;

		if (_blendingMode > 0)
			newRenderState.blendingMode = _blendingMode;
	};


	void RenderStateCashe::setDefaultFaceCulling(bool _faceCulling, GLenum _faceCullingMode)
	{
		defaultRenderState.faceCulling = _faceCulling;

		if (_faceCullingMode > 0)
			defaultRenderState.faceCullingMode = _faceCullingMode;
	};


	void RenderStateCashe::setDefaultDepthTest(bool _depthTest, GLenum _depthTestFunction, GLclampf _depthTestNearVal, GLclampf _depthTestFarVal)
	{
		defaultRenderState.depthTest = _depthTest;

		if (_depthTestFunction > 0)
			defaultRenderState.depthTestFunction = _depthTestFunction;

		if (_depthTestNearVal != HUGE_VALF)
			defaultRenderState.depthTestNearVal = _depthTestNearVal;

		if (_depthTestFarVal != HUGE_VALF)
			defaultRenderState.depthTestFarVal = _depthTestFarVal;
	};


	void RenderStateCashe::setDefaultDither(bool _dither)
	{
		defaultRenderState.dither = _dither;
	};


	void RenderStateCashe::setDefaultScissorTest(bool _scissorTest, GLint _scissorTestX, GLint _scissorTestY, GLsizei _scissorTestWidth, GLsizei _scissorTestHeight)
	{
		defaultRenderState.scissorTest = _scissorTest;

		if (_scissorTestX >= 0)
			defaultRenderState.scissorTestX = _scissorTestX;

		if (_scissorTestY >= 0)
			defaultRenderState.scissorTestY = _scissorTestY;

		if (_scissorTestWidth >= 0)
			defaultRenderState.scissorTestWidth = _scissorTestWidth;

		if (_scissorTestHeight >= 0)
			defaultRenderState.scissorTestHeight = _scissorTestHeight;
	};



	void RenderStateCashe::setDefaultTextures(unsigned int _textureId)
	{
		newRenderState.texture2D[0] = _textureId;
	};


	void RenderStateCashe::resetToDefault()
	{
		setBlending(defaultRenderState.blending, defaultRenderState.blendingSfactor, defaultRenderState.blendingDfactor, defaultRenderState.blendingMode);

		setFaceCulling(defaultRenderState.faceCulling, defaultRenderState.faceCullingMode);

		setDepthTest(defaultRenderState.depthTest, defaultRenderState.depthTestFunction, defaultRenderState.depthTestNearVal, defaultRenderState.depthTestFarVal);

		setDither(defaultRenderState.dither);

		setScissorTest(defaultRenderState.scissorTest, defaultRenderState.scissorTestX, defaultRenderState.scissorTestY, defaultRenderState.scissorTestWidth, defaultRenderState.scissorTestHeight);

		newRenderState.activeVertexAttribs = 0;

		setActiveTextures(8, defaultRenderState.texture2D);
	};


	void RenderStateCashe::setBlending(bool _blending, GLenum _blendingSfactor, GLenum _blendingDfactor, GLenum _blendingMode)
	{
		newRenderState.blending = _blending;

		if (newRenderState.blending != currentRenderState.blending)
			requiredStateChanges.insert(RENDERER_STATE::RS_BLENDING);
		else
			requiredStateChanges.erase(RENDERER_STATE::RS_BLENDING);


		if (_blendingSfactor + _blendingDfactor > 0)
		{
			if (_blendingSfactor > 0)
				newRenderState.blendingSfactor = _blendingSfactor;

			if (_blendingDfactor > 0)
				newRenderState.blendingDfactor = _blendingDfactor;

			if (newRenderState.blendingSfactor != currentRenderState.blendingSfactor ||
				newRenderState.blendingDfactor != currentRenderState.blendingDfactor)
				requiredStateChanges.insert(RENDERER_STATE::RS_BLENDING_FUNCTION);
			else
				requiredStateChanges.erase(RENDERER_STATE::RS_BLENDING_FUNCTION);
		}


		if (_blendingMode > 0)
		{
			newRenderState.blendingMode = _blendingMode;

			if (newRenderState.blendingMode != currentRenderState.blendingMode)
				requiredStateChanges.insert(RENDERER_STATE::RS_BLENDING_EQUATION);
			else
				requiredStateChanges.erase(RENDERER_STATE::RS_BLENDING_EQUATION);
		}
	};


	void RenderStateCashe::setFaceCulling(bool _faceCulling, GLenum _faceCullingMode)
	{
		newRenderState.faceCulling = _faceCulling;

		if (newRenderState.faceCulling != currentRenderState.faceCulling)
			requiredStateChanges.insert(RS_FACE_CULLING);
		else
			requiredStateChanges.erase(RS_FACE_CULLING);


		if (_faceCullingMode > 0)
		{
			newRenderState.faceCullingMode = _faceCullingMode;

			if (newRenderState.faceCullingMode != currentRenderState.faceCullingMode)
				requiredStateChanges.insert(RS_FACE_CULLING_MODE);
			else
				requiredStateChanges.erase(RS_FACE_CULLING_MODE);
		}
	};


	void RenderStateCashe::setDepthTest(bool _depthTest, GLenum _depthTestFunction, GLclampf _depthTestNearVal, GLclampf _depthTestFarVal)
	{
		newRenderState.depthTest = _depthTest;

		if (newRenderState.depthTest != currentRenderState.depthTest)
			requiredStateChanges.insert(RENDERER_STATE::RS_DEPTH_TEST);
		else
			requiredStateChanges.erase(RENDERER_STATE::RS_DEPTH_TEST);


		if (_depthTestFunction > 0)
		{
			newRenderState.depthTestFunction = _depthTestFunction;

			if (newRenderState.depthTestFunction != currentRenderState.depthTestFunction)
				requiredStateChanges.insert(RENDERER_STATE::RS_DEPTH_TEST_FUNCTION);
			else
				requiredStateChanges.erase(RENDERER_STATE::RS_DEPTH_TEST_FUNCTION);
		}


		if (_depthTestNearVal != HUGE_VALF || _depthTestFarVal != HUGE_VALF)
		{
			if (_depthTestNearVal != HUGE_VALF)
				newRenderState.depthTestNearVal = _depthTestNearVal;

			if (_depthTestFarVal != HUGE_VALF)
				newRenderState.depthTestFarVal = _depthTestFarVal;

			if (newRenderState.depthTestNearVal != currentRenderState.depthTestNearVal ||
				newRenderState.depthTestFarVal != currentRenderState.depthTestFarVal)
				requiredStateChanges.insert(RENDERER_STATE::RS_DEPTH_TEST_RANGE);
			else
				requiredStateChanges.erase(RENDERER_STATE::RS_DEPTH_TEST_RANGE);
		}
	};


	void RenderStateCashe::setDither(bool _dither)
	{
		newRenderState.dither = _dither;

		if (newRenderState.dither != currentRenderState.dither)
			requiredStateChanges.insert(RENDERER_STATE::RS_DITHER);
		else
			requiredStateChanges.erase(RENDERER_STATE::RS_DITHER);
	};


	void RenderStateCashe::setScissorTest(bool _scissorTest, GLint _scissorTestX, GLint _scissorTestY, GLsizei _scissorTestWidth, GLsizei _scissorTestHeight)
	{
		newRenderState.scissorTest = _scissorTest;

		if (newRenderState.scissorTest != currentRenderState.scissorTest)
			requiredStateChanges.insert(RENDERER_STATE::RS_SCISSOR_TEST);
		else
			requiredStateChanges.erase(RENDERER_STATE::RS_SCISSOR_TEST);


		if (_scissorTestX >= 0)
			newRenderState.scissorTestX = _scissorTestX;

		if (_scissorTestY >= 0)
			newRenderState.scissorTestY = _scissorTestY;

		if (_scissorTestWidth >= 0)
			newRenderState.scissorTestWidth = _scissorTestWidth;

		if (_scissorTestHeight >= 0)
			newRenderState.scissorTestHeight = _scissorTestHeight;

		if (newRenderState.scissorTestX != currentRenderState.scissorTestX ||
			newRenderState.scissorTestY != currentRenderState.scissorTestY ||
			newRenderState.scissorTestWidth != currentRenderState.scissorTestWidth ||
			newRenderState.scissorTestHeight != currentRenderState.scissorTestHeight)
			requiredStateChanges.insert(RENDERER_STATE::RS_SCISSOR_TEST_RECTANGLE);
		else
			requiredStateChanges.erase(RENDERER_STATE::RS_SCISSOR_TEST_RECTANGLE);
	};


	void RenderStateCashe::setShadingProgram(GLuint _shadingProgram)
	{
		newRenderState.shadingProgram = _shadingProgram;
        newRenderState.activeVertexAttribs = 0;
        requiredStateChanges.insert(RS_VERTEX_ATRIBS);

        if (newRenderState.shadingProgram != currentRenderState.shadingProgram)
			requiredStateChanges.insert(RENDERER_STATE::RS_PROGRAM);
		else
			requiredStateChanges.erase(RENDERER_STATE::RS_PROGRAM);
	};


    void RenderStateCashe::setVertexAtrib(unsigned int _activeVertexAttribs)
    {
        assert(_activeVertexAttribs <8 && "only 8 attribs available");

        newRenderState.activeVertexAttribs |= (0x01 << _activeVertexAttribs);

        if (newRenderState.activeVertexAttribs != currentRenderState.activeVertexAttribs)
            requiredStateChanges.insert(RS_VERTEX_ATRIBS);
        else
            requiredStateChanges.erase(RS_VERTEX_ATRIBS);
    };



	void RenderStateCashe::setActiveTextures(unsigned int _textureCount, GLuint _textures[])
	{
		assert(_textureCount > 0 && _textureCount < 8 && "only 1 to 8 concurent textures supported");

		if (_textures)
			memcpy(newRenderState.texture2D, _textures, sizeof(GLuint) * _textureCount);
		else
			memset(newRenderState.texture2D, 0, sizeof(GLuint) * _textureCount);

		if (memcmp(newRenderState.texture2D, currentRenderState.texture2D, sizeof(newRenderState.texture2D)))
			requiredStateChanges.insert(RS_TEXTURE_BINDING);
		else
			requiredStateChanges.erase(RS_TEXTURE_BINDING);
	};


	void RenderStateCashe::setActiveTexture(unsigned int _textureIndex, GLuint _textureId)
	{
		assert(_textureIndex >= 0 && _textureIndex < 8 && "only 0 to 7 texture index supported");

		newRenderState.texture2D[_textureIndex] = _textureId;
		if (newRenderState.texture2D[_textureIndex] != currentRenderState.texture2D[_textureIndex])
			requiredStateChanges.insert(RS_TEXTURE_BINDING);
	};


	void RenderStateCashe::applyState()
	{
		bool vertexBufferChanged = false;
		bool indexBufferChanged = false;

		try
		{
			for (auto it = requiredStateChanges.begin(), itEnd = requiredStateChanges.end();
				 it != itEnd; ++it) {
				switch (*it) {
					case RS_BLENDING: {
						if (newRenderState.blending) {
							GL_ERROR_CHECK(glEnable(GL_BLEND));
						} else {
							GL_ERROR_CHECK(glDisable(GL_BLEND));
						}

						currentRenderState.blending = newRenderState.blending;
						break;
					}
					case RS_BLENDING_FUNCTION: {
						GL_ERROR_CHECK(glBlendFunc(newRenderState.blendingSfactor,
												   newRenderState.blendingDfactor));

						currentRenderState.blendingSfactor = newRenderState.blendingSfactor;
						currentRenderState.blendingDfactor = newRenderState.blendingDfactor;
						break;
					}
					case RS_BLENDING_EQUATION: {
						GL_ERROR_CHECK(glBlendEquation(newRenderState.blendingMode));

						currentRenderState.blendingMode = newRenderState.blendingMode;
						break;
					}

					case RS_FACE_CULLING: {
						if (newRenderState.faceCulling) {
							GL_ERROR_CHECK(glEnable(GL_CULL_FACE));
						} else {
							GL_ERROR_CHECK(glDisable(GL_CULL_FACE));
						}

						currentRenderState.faceCulling = newRenderState.faceCulling;
						break;
					}
					case RS_FACE_CULLING_MODE: {
						GL_ERROR_CHECK(glCullFace(newRenderState.faceCullingMode));

						currentRenderState.faceCullingMode = newRenderState.faceCullingMode;
						break;
					}

					case RS_DEPTH_TEST: {
						if (newRenderState.depthTest) {
							GL_ERROR_CHECK(glEnable(GL_DEPTH_TEST));
						} else {
							GL_ERROR_CHECK(glDisable(GL_DEPTH_TEST));
						}

						currentRenderState.depthTest = newRenderState.depthTest;
						break;
					}
					case RS_DEPTH_TEST_FUNCTION: {
						GL_ERROR_CHECK(glDepthFunc(newRenderState.depthTestFunction));

						currentRenderState.depthTestFunction = newRenderState.depthTestFunction;
						break;
					}
					case RS_DEPTH_TEST_RANGE: {
						GL_ERROR_CHECK(glDepthRangef(newRenderState.depthTestNearVal,
													 newRenderState.depthTestFarVal));

						currentRenderState.depthTestNearVal = newRenderState.depthTestNearVal;
						currentRenderState.depthTestFarVal = newRenderState.depthTestFarVal;
						break;
					}

					case RS_DITHER: {
						if (newRenderState.dither) {
							GL_ERROR_CHECK(glEnable(GL_DITHER));
						} else {
							GL_ERROR_CHECK(glDisable(GL_DITHER));
						}

						currentRenderState.dither = newRenderState.dither;
						break;
					}

					case RS_SCISSOR_TEST: {
						if (newRenderState.scissorTest) {
							GL_ERROR_CHECK(glEnable(GL_SCISSOR_TEST));
						} else {
							GL_ERROR_CHECK(glDisable(GL_SCISSOR_TEST));
						}

						currentRenderState.scissorTest = newRenderState.scissorTest;
						break;
					}
					case RS_SCISSOR_TEST_RECTANGLE: {
						GL_ERROR_CHECK(glScissor(newRenderState.scissorTestX,
												 newRenderState.scissorTestY,
												 newRenderState.scissorTestWidth,
												 newRenderState.scissorTestHeight));

						currentRenderState.scissorTestX = newRenderState.scissorTestX;
						currentRenderState.scissorTestY = newRenderState.scissorTestY;
						currentRenderState.scissorTestWidth = newRenderState.scissorTestWidth;
						currentRenderState.scissorTestHeight = newRenderState.scissorTestHeight;
						break;
					}

					case RS_PROGRAM: {
						GL_ERROR_CHECK(glUseProgram(newRenderState.shadingProgram));

						currentRenderState.shadingProgram = newRenderState.shadingProgram;
						vertexBufferChanged = true;
						indexBufferChanged = true;
						currentRenderState.vertexBuffer = 0;
						currentRenderState.indexBuffer = 0;
						break;
					}

					case RS_VERTEX_ATRIBS: {

					    unsigned int mask = 1;
					    for (unsigned int i = 0; i < 8; ++i)
                        {
                            if ((mask << i) & (currentRenderState.activeVertexAttribs ^ newRenderState.activeVertexAttribs))
                            {
                                if ((mask << i) & newRenderState.activeVertexAttribs)
                                {
                                    GL_ERROR_CHECK(glEnableVertexAttribArray((GLuint) i));
                                }
                                else
                                {
                                    GL_ERROR_CHECK(glDisableVertexAttribArray((GLuint) i));
                                }
                            }
                        }

                        currentRenderState.activeVertexAttribs = newRenderState.activeVertexAttribs;
						break;
					}

					case RS_TEXTURE_BINDING: {
						for (unsigned int i = 0; i < 8; ++i) {
							if (newRenderState.texture2D[i] != currentRenderState.texture2D[i]) {
								GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + i));
								GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, newRenderState.texture2D[i]));

								currentRenderState.texture2D[i] = newRenderState.texture2D[i];
							}
						}

						break;
					}


					case RS_VERTEX_BUFFER_BINDING:
						vertexBufferChanged = false;
						break;

					case RS_INDEX_BUFFER_BINDING:
						indexBufferChanged = false;
						break;

				}
			}

			//setup draw buffers after, shader changes reset this setting
			if (vertexBufferChanged &&
				currentRenderState.vertexBuffer != newRenderState.vertexBuffer) {
				GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, newRenderState.vertexBuffer));
				currentRenderState.vertexBuffer = newRenderState.vertexBuffer;
			}

			if (indexBufferChanged &&
				currentRenderState.indexBuffer != newRenderState.indexBuffer) {
				GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newRenderState.indexBuffer));
				currentRenderState.indexBuffer = newRenderState.indexBuffer;
			}
		}
		catch (const std::exception &e)
		{
			Logger::getSingleton().write(e.what(), LL_ERROR);
		}


			requiredStateChanges.clear();
		//memcpy(&currentRenderState, &newRenderState, sizeof(RenderStateCashe));
	};

}

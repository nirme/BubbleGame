#pragma once

#include <memory>
#include <vector>
#include <GLES2/gl2.h>
#include "Exceptions.h"
#include "Logger.h"


namespace core
{
	class RenderSystem;


	class GraphicBuffer
	{
	protected:

		RenderSystem* renderer;

		GLuint bufferId;
		GLenum bufferUsageType; // GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
		GLenum bufferType; //v: GL_ARRAY_BUFFER, i: GL_ELEMENT_ARRAY_BUFFER

		GLenum elementType; // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, GL_FLOAT

		bool bufferInitiated;



		typedef std::vector<unsigned char> VertexArray;
		unsigned char elemTypeMultiplier;
		VertexArray localBuffer;
		unsigned int bufferCurrentPos;


		unsigned int getRemainingBytes() const;
		unsigned char getElementTypeMultiplier(GLenum _elementType) const;

	public:

		GraphicBuffer(RenderSystem *_renderer = nullptr, GLenum _bufferUsageType = GL_DYNAMIC_DRAW, GLenum _bufferType = GL_ARRAY_BUFFER, GLenum _elementType = GL_FLOAT);
		~GraphicBuffer();

		inline GLuint getId() const { return bufferId; };

		void load();
		void unload();
		void reload();

		int uploadData();

		void setBufferUsageType(GLenum _usage);
		void setBufferType(GLenum _type);
		GLenum getBufferType();

		void setElementType(GLenum _elementType);
		GLenum getElementType()  const;

		void resize(unsigned int _size);

		void rewind();
		unsigned int seek(unsigned int _pos);
		unsigned int getLength() const;
		unsigned int getRemainingLength() const;


		unsigned int position() const;

		unsigned char elementSize() const;



		template <typename T>
		unsigned int write(T* _elements, unsigned int _count)
		{
			assert(elemTypeMultiplier == sizeof(T) && "Incorrect type set for buffer");

			if (getRemainingBytes() < (_count * elemTypeMultiplier))
				return 0;

			memcpy(&localBuffer[bufferCurrentPos * elemTypeMultiplier], _elements, _count * elemTypeMultiplier);
			bufferCurrentPos += _count;

			return _count;
		};

		unsigned int write(const void* _elements, unsigned int _count);
	};


	typedef std::shared_ptr<GraphicBuffer> GraphicBufferPtr;
}

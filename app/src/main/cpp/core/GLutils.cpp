#include "GLutils.h"



namespace core
{

    unsigned int GLTypeSize(GLenum _type)
    {
        switch (_type)
        {
            case GL_FLOAT:
            case GL_FIXED:
                return sizeof(GLfloat) * 1;
                break;

            case GL_FLOAT_VEC2:
                return sizeof(GLfloat) * 2;
                break;

            case GL_FLOAT_VEC3:
                return sizeof(GLfloat) * 3;
                break;

            case GL_FLOAT_VEC4:
                return sizeof(GLfloat) * 4;
                break;

            case GL_FLOAT_MAT2:
                return sizeof(GLfloat) * 2 * 2;
                break;

            case GL_FLOAT_MAT3:
                return sizeof(GLfloat) * 3 * 3;
                break;

            case GL_FLOAT_MAT4:
                return sizeof(GLfloat) * 4 * 4;
                break;

            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
                return sizeof(GLshort);
                break;

            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
                break;
        };

        return 0;
    };

}

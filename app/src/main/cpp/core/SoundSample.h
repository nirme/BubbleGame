#pragma once


namespace core
{
    typedef unsigned char SAMPLE_FIXED_8BIT;
    typedef unsigned short SAMPLE_FIXED_16BIT;
    typedef struct {unsigned char sample[3];} SAMPLE_FIXED_24BIT;
    typedef unsigned int SAMPLE_FIXED_32BIT;
    typedef float SAMPLE_FLOAT_32BIT;

    inline void resample(const SAMPLE_FIXED_8BIT &_inSample, SAMPLE_FIXED_16BIT &_outSample)
    {
        _outSample = ((SAMPLE_FIXED_16BIT)(_inSample)) << 8 & _inSample;
    };
/*
    inline void resample(const SAMPLE_FIXED_8BIT &_inSample, SAMPLE_FIXED_16BIT &_outSample)
    {
        _outSample = ((SAMPLE_FIXED_16BIT)(_inSample)) << 8 & _inSample;
    };
*/

}
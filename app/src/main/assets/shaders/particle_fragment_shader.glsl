#version 100

uniform sampler2D texture_0;
varying vec2 tex1Coords;
varying float varAlpha;
varying float varAge;

void main()
{
    gl_FragColor = texture2D(texture_0, tex1Coords);
	//float w = gl_FragColor.w;
	gl_FragColor.w *= varAlpha * varAge;
	//gl_FragColor.w = w;
}

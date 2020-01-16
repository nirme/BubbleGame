#version 100

uniform mat3 viewProjMx3;

attribute vec2 position;
attribute vec2 tex1;
attribute float alpha;
attribute float age;

varying vec2 tex1Coords;
varying float varAlpha;
varying float varAge;

void main()
{
	tex1Coords = tex1;
	varAlpha = alpha;
	varAge = age;

    gl_Position = vec4(viewProjMx3 * vec3(position, 1.0), 1.0);
}

#version 100

uniform mat3 viewProjMx3;

attribute vec2 position;
attribute vec2 tex1;

varying vec2 tex1Coords;

void main()
{
	tex1Coords = tex1;
    gl_Position = vec4(viewProjMx3 * vec3(position, 1.0), 1.0);
    //gl_Position = vec4(position, 0.5f, 1.0);
}




/*
#version 100

//uniform mat3 worldMx3;
//uniform mat3 viewProjMx3;
//uniform mat3 worldViewProjMx3;

attribute vec2 position;
attribute vec2 tex1;
//attribute vec2 tex2;

varying vec2 tex1Coords;

void main()
{
	tex1Coords = tex1;
	//gl_Position = vec4((worldViewProjMx3 * vec3(position, 0.5)), 1.0);
	gl_Position = vec4(position, 0.5, 1.0);
}
*/
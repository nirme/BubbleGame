#version 100

uniform sampler2D texture_0;
varying vec2 tex1Coords;

void main()
{
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    gl_FragColor = texture2D(texture_0, tex1Coords);
    //gl_FragColor.w = 1.0 - gl_FragColor.w;
    //vec4 t = texture2D(texture_0, tex1Coords);
    //gl_FragColor = vec4(t.xyz, 0.5);
}


/*
#version 100

uniform sampler2D texture_0;

varying vec2 tex1Coords;

void main()
{
    gl_FragColor = vec4(texture2D(texture_0, tex1Coords), 1.0 );
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
*/
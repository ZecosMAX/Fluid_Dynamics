#version 330

uniform vec2 pos;

void main()
{
    // transform the vertex position
    gl_Position = vec4(pos, 0.0, 0.0);

    // transform the texture coordinates
    //gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    //gl_FrontColor = gl_Color;
}
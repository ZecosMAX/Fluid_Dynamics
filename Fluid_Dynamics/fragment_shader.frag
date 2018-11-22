uniform vec3 color;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = vec4(color, 0.5); //texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = vec4(0, 0, 0, 0.5);
}
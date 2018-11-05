#version 130

uniform int Limit;
uniform vec2 offset;
uniform vec2 position;
uniform float scale;

int mandelbrot(in int maxIterations)
{
	vec2 z = vec2(0.0f);
	vec2 c = (gl_TexCoord[0].xy * 2) / scale + offset;
	for(int iteration = 0; iteration < maxIterations; iteration++)
	{
		z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
		if(length(z) >= 4)
			return iteration;
	}
	return 0;
}

void main()
{
    // lookup the pixel in the texture
	float iter = mandelbrot(Limit);
    vec4 pixel = vec4(0.0, mod(iter, 1)/iter + Limit, 0.2, 1.0);//texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel;
}

varying vec2 TexCoord;
void main()
{
	const int max_iteration = 100;

	float x0 = TexCoord.x;
	float y0 = TexCoord.y;

	float x = 0.0;
	float y = 0.0;

	int iteration = 0;
	while ( (x*x + y*y) <= 4.0 )
	{
		float xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		++iteration;
		if ((++iteration) == max_iteration)
			break;
	}

	float color = iteration / 32.0;

	gl_FragColor = vec4(color, color, color, 1.0);

}


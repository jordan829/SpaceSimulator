//varying float intensity;
varying vec3 normal;
varying vec3 colorv;
varying vec3 camv;

void main()
{
	float intensity;
	float ink;

	vec4 color;
	vec3 n = normalize(normal);
	intensity = dot(normalize(vec3(gl_LightSource[0].position)), n);
	ink = dot(normalize(camv), n);

	if (intensity > 0.99 && intensity <= 1.0)
		color = vec4(1.0,1.0,1.0,1.0);
	else if (intensity > 0.6)
		color = vec4(colorv.x, colorv.y, colorv.z, 1.0);
	else if (intensity > 0.30)
		color = vec4(0.6 * colorv.x, 0.6 * colorv.y, 0.6 * colorv.z, 1.0);
	else if (intensity > 0.10)
		color = vec4(0.4 * colorv.x, 0.4 * colorv.y, 0.4 * colorv.z, 1.0);
	else
		color = vec4(0.2 * colorv.x, 0.2 * colorv.y, 0.2 * colorv.z, 1.0);

	if(ink >= 0.8)
		color = vec4(0.0, 0.0, 0.0, 1.0);

	gl_FragColor = color;

}


/**/
//varying float intensity;
varying vec3 normal;
varying vec3 colorv;
varying vec3 camv;

void main()
{
	//lightDir = normalize(vec3(gl_LightSource[0].position));
	//intensity = dot(lightDir,gl_Normal);

	//color = glColor;
	//position = gl_ModelViewMatrix * gl_Vertex;
	//gl_Position = gl_ProjectionMatrix * position;

	normal = gl_NormalMatrix * gl_Normal;
	gl_Position = ftransform();

	colorv = gl_Color;
}

/*******************************************/
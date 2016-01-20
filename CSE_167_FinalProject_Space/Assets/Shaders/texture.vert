varying vec4 position;
varying vec3 normal;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

    //Transform the normal into world coordinates
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    //Transform the vertex into world coordinates
    position = gl_ModelViewMatrix * gl_Vertex;
    
    //Set the gl position of this vertex
    gl_Position = ftransform();
}

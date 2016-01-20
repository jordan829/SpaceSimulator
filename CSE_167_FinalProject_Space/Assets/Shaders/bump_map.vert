uniform sampler2D normal_texture; 
varying vec4 position;
varying mat3 normalMatrix;
varying vec3 normalVector;

void main() {
	gl_TexCoord[0] = gl_MultiTexCoord0;  
	
	normalMatrix = gl_NormalMatrix;
	normalVector = gl_Normal;

	position = gl_ModelViewMatrix * gl_Vertex;

	// Set the position of the current vertex  
	gl_Position = ftransform();  
}  
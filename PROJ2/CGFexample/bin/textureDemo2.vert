/*uniform sampler2D secondImage;

void main() 
{
	vec4 offset=vec4(0.0,0.0,0.0,0.0);
	
	// change vertex offset based on texture information
	if (texture2D(secondImage, vec2(1.0,1.0)-gl_MultiTexCoord0.st).b > 0.5)
		offset.xyz=gl_Normal*normScale*0.1;

	// Set the position of the current vertex 
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex+offset);
	

	//newVertex.y= sin(...coord.s...time...)


	// pass texture coordinates from VS to FS.
	// "gl_MultiTexCoord0" has the texture coordinates assigned to this vertex in the first set of coordinates.

	// This index has to do with the set of texture COORDINATES, it is NOT RELATED to the texture UNIT.
	// "gl_TexCoord[0]" is a built-in varying that will be interpolated in the FS.
	gl_TexCoord[0] = gl_MultiTexCoord0;
}*/



//uniform float currentAngle;
uniform float time;
uniform float speed;
varying float phase;
//out vec3 vertex_to_fragment_color;

void main(){

	phase = (time+gl_Vertex.x)*speed;

	gl_Position = gl_ModelViewProjectionMatrix*(gl_Vertex + vec4(0.0, sin( (time+gl_Vertex.x)*speed), 0.0, 0.0)); 
	gl_TexCoord[0].xy = gl_MultiTexCoord0.st;
	//vTexCoord = gl_MultiTexCoord0.xy;
}
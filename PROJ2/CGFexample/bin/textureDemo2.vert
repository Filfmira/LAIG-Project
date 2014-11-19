uniform float time;
uniform float speed;
varying float phase;

void main(){

	phase = (time+gl_Vertex.x)*speed;

	gl_Position = gl_ModelViewProjectionMatrix*(gl_Vertex + vec4(0.0, sin( (time+gl_Vertex.x)*speed), 0.0, 0.0)); 
	gl_TexCoord[0].xy = gl_MultiTexCoord0.st;

}
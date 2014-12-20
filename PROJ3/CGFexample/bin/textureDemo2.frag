uniform sampler2D texture;
varying float phase;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].st);

	float temp = (cos(phase)+1.0)/2.0;


	gl_FragColor = (temp*0.6+0.4)*color; 

}

uniform sampler2D texture;
varying float phase;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].st);

	float temp = (cos(phase)+1.0)/2.0;
	// notice the coordinate conversion to flip the image horizontally and vertically
/*	vec4 filter=texture2D(secondImage, vec2(1.0,1.0)-gl_TexCoord[0].st);

	if (filter.b > 0.5)
		color=vec4(0.52,0.18,0.11,1.0);
	*/
	gl_FragColor = (temp*0.6+0.4)*color; 
	//gl_FragColor = texture2D(myTexture, vTexCoord).rgba;

}

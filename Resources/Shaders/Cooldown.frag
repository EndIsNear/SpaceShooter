varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float u_percent;

void main()
{
	gl_FragColor = texture2D(CC_Texture0, v_texCoord);
	if(u_percent + 0.001 < 0.999)
	{
		float angle =  atan(v_texCoord.y - 0.5, v_texCoord.x - 0.5)  + 3.1415;
		float grey = (gl_FragColor.x + gl_FragColor.y + gl_FragColor.z)/ 3.;
		gl_FragColor  = vec4(grey,grey,grey, 1.);
		if(angle < 6.2831 * u_percent)
		{
			gl_FragColor *= vec4(0.4, 0.4, 0.4, 1.);
		}
	}
}
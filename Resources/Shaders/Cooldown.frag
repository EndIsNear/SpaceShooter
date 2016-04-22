varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float u_percent;

void main()
{
  float angle =  atan(v_texCoord.y - 0.5, v_texCoord.x - 0.5)  + 3.1415;
  if(angle < 6.2831 * u_percent)
  	gl_FragColor = texture2D(CC_Texture0, v_texCoord);
  else
  {
  	float min = 0.1;
  	float max = 0.5;
  	float diff = max - min;

 	  float coef = min  + (max - min) * u_percent;
  	gl_FragColor = vec4(min + u_percent * diff, min + u_percent * diff, min + u_percent * diff, 1.) * texture2D(CC_Texture0, v_texCoord);
  }
}
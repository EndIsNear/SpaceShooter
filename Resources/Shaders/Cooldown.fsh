varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float u_percent;

void main()
{
  float angle =  atan(v_texCoord.y - 0.5f, v_texCoord.x - 0.5f)  + 3.14159265359f ;
  if(angle < 6.28318530718f * u_percent)
  	gl_FragColor = texture2D(CC_Texture0, v_texCoord);
  else
  {
  	const float min = 0.1;
  	const float max = 0.1;

 	float coef = min  + (max - min) * u_percent ;
  	gl_FragColor = vec4(u_percent, u_percent, u_percent, 1.) * texture2D(CC_Texture0, v_texCoord);
  }
}
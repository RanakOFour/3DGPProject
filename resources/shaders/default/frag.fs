varying vec2 v_texCoord;
uniform sampler2D u_Texture;

void main()
{
	vec4 tex = texture2D(u_Texture, v_texCoord);
	gl_FragColor = tex;
}
uniform sampler2D u_Sampler;
varying highp vec2 v_TexCoord;

void main(void)
{
		gl_FragColor = texture2D(u_Sampler, v_TexCoord);
}


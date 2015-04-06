attribute highp vec4 a_Position;
//attribute highp vec4 qt_MultiTexCoord0;
uniform highp mat4 u_MvpMatrix;
//uniform highp vec4 a_Color;
//attribute highp vec3 a_Normals;
attribute highp vec2 a_TexCoord;

//varying highp vec4 v_Color;
varying highp vec2 v_TexCoord;
//varying highp vec4 qt_TexCoord0;

void main(void)
{
		gl_Position = u_MvpMatrix * a_Position;
		v_TexCoord = a_TexCoord;
	//	v_Color = a_Color;
}


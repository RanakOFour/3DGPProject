attribute vec2 a_Position;
attribute vec2 a_PixelColor;

varying vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    gl_Position = u_Projection * u_View * vec4(a_Position, 0.0, 1.0);
    v_TexCoord = a_PixelColor;
}
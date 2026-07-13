#version 430
in vec2 f_TexCoord;
out vec4 FragColor;
uniform sampler2D u_image;
uniform vec4 u_spriteColor;
void main()
{
   FragColor =u_spriteColor*texture(u_image,f_TexCoord);
}
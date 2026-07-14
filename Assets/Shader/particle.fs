#version 430
in vec2 f_TexCoord;
out vec4 FragColor;
uniform vec4 u_color;
uniform sampler2D u_ParticleImage;
void main()
{
    FragColor =u_color* texture(u_ParticleImage,f_TexCoord);
    //FragColor = vec4(texture(u_ParticleImage,f_TexCoord).x,texture(u_ParticleImage,f_TexCoord).x,0.0f,1.0f);
}
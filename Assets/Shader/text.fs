#version 430
in vec2 f_TexCoord;
out vec4 FragColor;
uniform sampler2D u_TextImage;
uniform vec3 u_TextColor;
void main()
{
    vec4 sampled = vec4(1.0f,1.0f,1.0f,texture(u_TextImage,f_TexCoord).a);
    FragColor = vec4(u_TextColor,1.0f) * sampled;
}
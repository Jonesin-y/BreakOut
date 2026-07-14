#version 430
layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 TexCoord;
out vec2 f_TexCoord;
uniform vec3 u_color;
uniform float u_offset;
uniform mat4 u_projection;
uniform mat4 u_model;
void main()
{
    float scale = 10.0f;
    f_TexCoord = TexCoord;

    gl_Position = u_projection *u_model* vec4((Position.x * scale) + u_offset,(Position.y * scale) + u_offset,0.0f,1.0f);
}
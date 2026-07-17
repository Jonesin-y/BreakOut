#version 430
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 TexCoord;
out vec2 f_TexCoord;
uniform float u_time;
uniform bool u_shake;
uniform bool u_chaos;
uniform bool u_confuse;
void main()
{
    gl_Position = vec4(position,0.0f,1.0f);
    if(u_shake)
    {
        float strength = 0.01f;
        gl_Position.x+=cos(u_time*10)*strength;
        gl_Position.y+=cos(u_time*15)*strength;
    }
    if(u_chaos)
    {
        float strength = 0.3;
        f_TexCoord = vec2(TexCoord.x+sin(u_time)*strength,TexCoord.y+cos(u_time)*strength);
    }
    else if(u_confuse)
    {
        f_TexCoord = vec2(1-TexCoord.x,1-TexCoord.y);
    }
    else
    {
        f_TexCoord = TexCoord;
    }
}
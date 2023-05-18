#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TextCoords;


out vec4 v_pos;
out vec3 skyboxTC;
out vec2 noiseTc;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    noiseTc = TextCoords;
    skyboxTC = aPos;
    v_pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = v_pos.xyww;
}  
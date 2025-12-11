#version 400 core

// Static
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

// Instanced
layout (location = 2) in mat4 model;
layout (location = 6) in int aSpriteId;

uniform mat4 projection;
uniform mat4 view;
// uniform mat4 model;

out vec3 vertColor;
out vec2 TexCoord;
flat out int spriteId;

void main()
{

    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    vertColor = vec3(((gl_VertexID+1) % 5)*0.3,0,0 );// aColor.xyz;
    spriteId = aSpriteId;
}

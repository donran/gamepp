#ifndef SHADERS_H__
#define SHADERS_H__

#include <string>

const std::string vertex2 = R"(
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
)";

// some source for our fragment shader
const std::string fragment2 = R"(
#version 400 core

out vec4 fragColor;

in vec3 vertColor;
in vec2 TexCoord;
flat in int spriteId;

uniform sampler2D ourTexture;

void main()
{
    ivec2 texSize = textureSize(ourTexture, 0);
    int spriteWidth = texSize.x/8;
    int spriteHeight = texSize.y/8;
    vec2 texel = 1.0 / texSize;

    int row = spriteHeight - 1 - spriteId / spriteWidth;
    int col = spriteId % int(spriteWidth);

    vec2 uv = vec2(TexCoord);
    uv.x = (col + TexCoord.x)*8 / texSize.x;
    uv.y = ((row + TexCoord.y)*8 / texSize.y);

    vec4 t = texture(ourTexture, uv);

    if (t.a == 0.0) { discard; }
    fragColor = t; // vec4(uv,0.0, 1.0); // texture(ourTexture, TexCoord);
}
)";

#endif

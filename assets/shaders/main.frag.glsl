#version 400 core

out vec4 fragColor;

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

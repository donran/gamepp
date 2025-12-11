#version 400 core

// Instanced
layout (location = 6) in int aSpriteId;
flat out int spriteId;

// Static
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main_quad() {
    TexCoord = aTexCoord;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

// Instanced
uniform mat4 projection;
uniform mat4 view;
layout (location = 2) in mat4 model;
void main_object2d() {
    gl_Position = projection * view * model * gl_Position;
}

// {
void main_texturedObject (in int aSpriteId, out int spriteId) {
    spriteId = aSpriteId;
}
// }
void main()
{
    main_quad();
    main_object2d();
    main_texturedObject(aSpriteId, spriteId);
}

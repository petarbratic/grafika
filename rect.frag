#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTex;
uniform float uAlpha; 

void main()
{
    vec4 tex = texture(uTex, TexCoord);
    FragColor = vec4(tex.rgb, tex.a * uAlpha);
}

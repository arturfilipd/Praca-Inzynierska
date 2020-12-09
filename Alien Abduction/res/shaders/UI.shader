#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ProjectionMatrix;
uniform float u_Depth;
out vec2 vs_position;
out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ProjectionMatrix * vec4(position, u_Depth/10.f, 1.0f);
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};

#vertex
#version 300 es

layout(location = 0) in vec4 position;
uniform mat4 transform;
uniform mat4 camera;
void main()
{
	gl_Position = camera * transform * position;
}

#fragment
#version 300 es
precision mediump float;

layout(location = 0) out vec4 color;
void main()
{
	color = vec4(1.0, 0.3, 0.3, 1.0);
}
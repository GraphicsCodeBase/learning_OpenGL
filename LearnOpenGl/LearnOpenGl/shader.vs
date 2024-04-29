#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 inColor;


uniform mat4 gWorld;

out vec4 Color;

void main()
{
	gl_Position = gWorld * vec4(Position,1.0);
	Color = vec4(inColor,1.0f);
	//gl_Vertex_ID = this is a system ID which provides the index of the current vertex.
	//counts the number of verticies that has been proccessed, 
}
#version 330 core

in vec4 Color;//input

//single output variable which is the color of the pixel.
//when the fragment shader is called the the rastizier has already identified which are the pixels that needs to be colored.

//output.
out vec4 FragColor;//declare the FragColor to match the RGBA window.

void main()
{
	FragColor = Color;
	//we are expecting the rasterizer to interpolate the color across the triangle.
}
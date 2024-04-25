#version 330 core

//single output variable which is the color of the pixel.
//when the fragment shader is called the the rastizier has already identified which are the pixels that needs to be colored.
out vec4 FragColor;//declare the FragColor to match the RGBA window.

void main()
{
	FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}
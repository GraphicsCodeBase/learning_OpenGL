#version 330 core 

layout (location = 0) in vec3 Position;//declaring the attributes it expects to receive.

uniform mat4 gTranslation;

//shader main function.
void main()//takes no input or output since we are using the in and out qualifiers.
{
	// we are outputting out a vec4 and not a vec3.
	gl_Position = gTranslation * vec4(Position,1.0);

}

//takes no input or output since we are using the in and out qualifie

//(tells the Shader compiler that it needs to allocate some space for this vriable of the type specified.)
//if we dont initalise the system will init to zero.(good for consistency.)
//uniform: third storage qualifier 

//mat4 is a GLSL type of a 4x4 Matrix.
#include <stdio.h>
#include <glew.h>
#include <freeglut.h>
#include <ogldev_math_3d.h>


GLuint VBO;
GLuint gScaleLocation;
const char* VsFileName = "shader.vs";
const char* FsFileName = "Shader.fs";


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//creating a shader handle.
	GLuint ShaderObj = glCreateShader(ShaderType);
	//create a error checking.
	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(1);
	}
	const GLchar* p[1];
	p[0] = pShaderText;

	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	//loading the shader txt into the shader handle.
	//taking in 4 prarms: shader handle, pointers to an array of string pointers, and an array of lengths.
	glShaderSource(ShaderObj, 1, p, Lengths);

	//compiling shader. on the shader handle.
	glCompileShader(ShaderObj);

	GLint Success;
	//this is needed to check shader issues and errors of the shaders.
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &Success);

	//error checking success or not
	//log the info of the error. and an error msg
	if (!Success)
	{
		GLchar InfoLog[1024];
		//log the error info into the GLchar InfoLog.
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		//print error string , and exit the program.
		fprintf(stderr, "Error Compiling shader type %d: %s '\n'", ShaderType, InfoLog);
		exit(1);
	}
	//if no issues we can attatch the shaders, we would connect the shader handle to the shader program. 
	glAttachShader(ShaderProgram, ShaderObj);
}

static void CreateVertexBuffer()
{
	//first create an array now size of 3.
	Vector3f Verticies[3];
	//init a single vector with all components init to 0.0
	//defining the triangle's verticies in its own grid.
	Verticies[0] = Vector3f(-1.0f, -1.0f, 0.0f);	//bottom left
	Verticies[1] = Vector3f(0.0f, 1.0f, 0.0f);		// top
	Verticies[2] = Vector3f(1.0f, -1.0f, 0.0f);		//bttom right 
	//creating a handle for the vertex buffer.
	glGenBuffers(1, &VBO);//two params , number of handles we want to allocate , array of gluint elements that wiull be big enough to contain the number of handles.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verticies), Verticies, GL_STATIC_DRAW); 
}
//this is the render callback function.
static void RenderSceneCB()
{
	static float Scale = 0.0f;
	static float Delta = 0.005f;

	Scale += Delta;
	if ((Scale >= 1.0f) || (Scale <= -1.0f))
	{
		Delta *= -1.0f;
	}

	//sending the value of the scale to the shader.
	//params : gScaleLocation(index location), scale(value to pass into the index.)
	glUniform1f(gScaleLocation, Scale);

	//clear window color
	glClear(GL_COLOR_BUFFER_BIT);
	//bind the buffer object to the vertex
	//since there might be multiple handles its safer to bound the last handle before using it.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//enable vertex attrib zero. vertex attributes can be texture coords , positions.
	//think of it as a gate in order for the data to flow in.
	glEnableVertexAttribArray(0);
	// we specify the format of the vertex attrib.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//telling the gpu that it must create a triangle for every 3 consecutive coords. 
	//from the bound vertex buffer object.
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutPostRedisplay();//tells glut to continously call the render call back over and over again.
	glutSwapBuffers();
}


static void CompileShaders()
{
	//cllocate the handle for the program.
	GLuint ShaderProgram = glCreateProgram();
	//error checking for creating the shader.
	//making sure we are getting a non zero handle.
	if (ShaderProgram == 0)
	{
		fprintf(stderr, "Error Creating Shader program\n");
		exit(1);
	}
	//vs for vertex shader and fs for fragment shader.
	std::string vs, fs;
	//read from shader txt file. and throw into the string vs.
	if (!ReadFile(VsFileName,vs))
	{
		exit(1);// if file not found throw an error.
	}
	//the string::vs will have the entire contents of the shader.
	//this function will take in the program handle, the contents of the shader, and fineally enum to indicate the type of the shader.
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	//error checking for fragment shader. before adding shader.
	if (!ReadFile(FsFileName, fs))
	{
		exit(1);// if file not found throw an error.
	}
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};

	//link the program and link the program with the program handle as the prarm.
	glLinkProgram(ShaderProgram);

	//making sure we check for link errors.
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error Linking Shader program: '%s' \n",ErrorLog);
		exit(1);
	}

	//we assign the uniform location into a GLuint program.
	//make sure we assign the get uniform is after the linking of the program.
	//make sure to query for the uniform location once for every unifiorm in the shader then store it somewhere.
	//make sure that the uniform is being used or not the compiler will give error.
	gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	//error checking for gScaleLocation.
	if (gScaleLocation == -1)
	{
		printf("Error getting uniformlocation. of 'gScale' \n ");
		exit(1);
	}

	//make sure that the shader is compatible with the current state of the opengl runtime.
	//sometimes that the current program that is linked wont be compatible with the current state.

	glValidateProgram(ShaderProgram);//this function might provide hints on how to optimise the shader..
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid Shader program: '%s' \n", ErrorLog);
		exit(1);
	}
	//make sure that the program is active on the gpu.
	glUseProgram(ShaderProgram);
}



int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);
	//GLUT RGBA -> set an RGBA mode
	//setting the display mode to double buffering.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	//window configuration.
	int width = 800;
	int height = 600;
	glutInitWindowSize(width, height);

	int x = 200;
	int y = 100;
	glutInitWindowPosition(x,y);
	int win = glutCreateWindow("first opengl Window.");
	printf("window id: %d\n", win);

	//initalising glew
	//doing error checking if glew isnt inialised!
	//since we also need the opengl context that glut creates.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
		return 1;
	}

	GLclampf red = 0.0f, blue = 0.0f, green = 0.0f,alpha = 0.0f;
	glClearColor(red, green, blue,alpha); 

	CreateVertexBuffer();
	
	CompileShaders();

	//register a render callback function.
	//main entrypoint of the application.
	glutDisplayFunc(RenderSceneCB);


	glutMainLoop();

	return 0;
}

//understanding beginners to opengl will have to go through alot of stages in order to print a simple primitive to the screen
//since opengl is composed of a pipeline of stages.where each stage will have its own prep.
#include <stdio.h>
#include <glew.h>
#include <freeglut.h>
#include <ogldev_math_3d.h>
#include <stdlib.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;
const char* VsFileName = "shader.vs";
const char* FsFileName = "Shader.fs";

//adding a vertex struct
struct Vertex
{
	Vector3f pos;
	Vector3f color;
	//constructor.
	Vertex() {};
	//populate color and position into the struct.
	Vertex(float x, float y,float z)
	{	
		//init the position. using x,y,z
		pos = Vector3f(x,y,z);
		float red =		rand() / (float)RAND_MAX;
		float green =	rand() / (float)RAND_MAX;
		float blue =	rand() / (float)RAND_MAX;
		color = Vector3f(red, green, blue);
	}
};
static void createVertexBuffer()
{
	Vertex Vertices[8];
	// we are rendering a cube.
	Vertices[0] = Vertex(0.5f, 0.5f, 0.5f);
	Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f);
	Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);
	Vertices[3] = Vertex(0.5f, -0.5f, -0.5f);
	Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);
	Vertices[5] = Vertex(0.5f, 0.5f, -0.5f);
	Vertices[6] = Vertex(0.5f, -0.5f, 0.5f);
	Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f);
	

	glGenBuffers(1, &VBO);//create the buffer handle.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//bind the buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}
static void createIndexBuffer()
{
	unsigned int Indices[] = {
							  0, 1, 2,
							  1, 3, 4,
							  5, 6, 3,
							  7, 3, 6,
							  2, 4, 7,
							  0, 7, 6,
							  0, 5, 1,
							  1, 5, 3,
							  5, 0, 6,
							  7, 4, 3,
							  2, 1, 4,
							  0, 2, 7
	};

	//creating the index buffer.
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);//we are binding the index buffer.(GL_ELEMENT_ARRAY_BUFFER) is your index buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}


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
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	#ifdef _WIN64
		Scale += 0.001f;
	#else
		Scale += 0.02f;
	#endif

	//making the cube spin.
	Matrix4f Rotation(cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						sinf(Scale), 0.0f, cosf(Scale), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4f Translation(1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 2.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

	//world transformation matrix.
	Matrix4f World = Translation * Rotation;
	//camera position at the origin 
	Vector3f CameraPos(0.0f, 0.0f, -1.0f);
	Vector3f U(1.0f, 0.0f, 0.0f);
	Vector3f V(0.0f, 1.0f, 0.0f);
	Vector3f N(0.0f, 0.0f, 1.0f);
	// camera matrix
	Matrix4f Camera(U.x, U.y, U.z, -CameraPos.x,
					V.x, V.y, V.z, -CameraPos.y,
					N.x, N.y, N.z, -CameraPos.z,
					0.0f, 0.0f, 0.0f, 1.0f);

	//projection matrix
	float FOV = 90.0f;
	float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));// make sure we change the degrees to radian.
	float d = 1 / tanHalfFOV;//calculate the distance.

	float ar = (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT;
	float NearZ = 1.0f;
	float FarZ = 10.0f;
	float zRange = NearZ - FarZ;
	float A = (-FarZ - NearZ) / zRange;
	float B = 2.0f * FarZ * NearZ / zRange;


	Matrix4f Projection(d/ar, 0.0f, 0.0f, 0.0f,
						0.0f, d, 0.0f, 0.0f,
						0.0f, 0.0f, A, B,
						0.0f, 0.0f, 1.0f, 0.0f);

	//we create the WVP Matrix 
	Matrix4f WVP = Projection * Camera * World;


	//calculating the final matrix.
	//Matrix4f FinalMatrix = Projection * Translation * Rotation;
	//sending the matrix into the shader as a uniform.
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutPostRedisplay();

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
	if (!ReadFile(VsFileName, vs))
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
	GLchar ErrorLog[1024] = { 0 };

	//link the program and link the program with the program handle as the prarm.
	glLinkProgram(ShaderProgram);

	//making sure we check for link errors.
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error Linking Shader program: '%s' \n", ErrorLog);
		exit(1);
	}

	//we assign the uniform location into a GLuint program.
	//make sure we assign the get uniform is after the linking of the program.
	//make sure to query for the uniform location once for every unifiorm in the shader then store it somewhere.
	//make sure that the uniform is being used or not the compiler will give error. 
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	//error checking for gScaleLocation.
	if (gWorldLocation == -1)
	{
		printf("Error getting uniformlocation. of 'gWorld' \n ");
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
	srand(GetCurrentProcessId());

	glutInit(&argc, argv);
	//GLUT RGBA -> set an RGBA mode
	//setting the display mode to double buffering.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	//window configuration.
	int width = WINDOW_WIDTH;
	int height = WINDOW_HEIGHT;
	glutInitWindowSize(width, height);

	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);
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

	GLclampf red = 0.0f, blue = 0.0f, green = 0.0f, alpha = 0.0f;
	glClearColor(red, green, blue, alpha);

	// we are enabling face culling
	// making sure we are only rendering the exterior of the mesh and not everyting.
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);//set the indicies to be clockwise direction.
	glCullFace(GL_BACK);//tell opengl to cull the back facing triangles.

	createVertexBuffer();
	createIndexBuffer();

	CompileShaders();

	//register a render callback function.
	//main entrypoint of the application.
	glutDisplayFunc(RenderSceneCB);


	glutMainLoop();

	return 0;
}

//understanding beginners to opengl will have to go through alot of stages in order to print a simple primitive to the screen
//since opengl is composed of a pipeline of stages.where each stage will have its own prep.
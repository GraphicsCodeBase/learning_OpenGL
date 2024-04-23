#include <stdio.h>
#include <glew.h>
#include <freeglut.h>
#include <ogldev_math_3d.h>


GLuint VBO;
const char* VsFileName = "shader.vs";
const char* FsFileName = "Shader.fs";


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);
	//create a error checking.
	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	const GLchar* p[1];
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
	if (!ReadFile(VsFileName,vs))
	{
		exit(1);
	}
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
}



int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);
	//GLUT RGBA -> set an RGBA mode
	//setting the display mode to double buffering.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	//window configuration.
	int width = 1920;
	int height = 1080;
	glutInitWindowPosition(width, height);
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
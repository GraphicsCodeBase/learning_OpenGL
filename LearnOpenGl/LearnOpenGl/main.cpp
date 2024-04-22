#include <stdio.h>
#include <glew.h>
#include <glut.h>


static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();//swapping the front and back buffers.
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
	GLclampf red = 1.0f, blue = 0.0f, green = 0.0f,alpha = 0.0f;
	glClearColor(red, green, blue,alpha);

	//register a render callback function.
	//main entrypoint of the application.
	glutDisplayFunc(RenderSceneCB);
	glutMainLoop();

	return 0;
}

//understanding beginners to opengl will have to go through alot of stages in order to print a simple primitive to the screen
//since opengl is composed of a pipeline of stages.where each stage will have its own prep.
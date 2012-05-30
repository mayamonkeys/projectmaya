#include <stdexcept>
#include <GL/glfw.h>

#include <config.h>

#include "UserInterface.hpp"
#include "DummyModels.hpp"

using std::runtime_error;

using namespace ProjectMaya;


UserInterface::UserInterface() {
	initGLFW();
	initOpenGL();
	render();
}

void UserInterface::initGLFW() {
	if(!glfwInit()) {
		throw new runtime_error("could not initialize glfw");
	}

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);

	if(!glfwOpenWindow(800, 480, 0, 0, 0, 0, GLFW_DEPTH_BITS, 0, GLFW_WINDOW)) {
		glfwTerminate();
		throw new runtime_error("could not create glfw window");
	}

	glfwSetWindowTitle(NAME " - " REVISION);
}

void UserInterface::initOpenGL() {
	/* transparency */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

/* main render loop */
void UserInterface::render() {
	int running = GL_TRUE;

	double oldTime = glfwGetTime();
	double currentTime;
	double deltaRotate = 0.0;

	while(running) {
		currentTime = glfwGetTime();
		deltaRotate += (currentTime - oldTime) * 0.1 * 360;
		oldTime = currentTime;

		renderOpenGL(deltaRotate);
		renderScene();

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwTerminate();
}

/* for now pass the rotation value as argument */
void UserInterface::renderOpenGL(const double& deltaRotate) {
	glLoadIdentity();

	/* rotate around the y and z axes */
	glRotatef(deltaRotate, 0, 1, 0);
	glRotatef(deltaRotate, 0, 0, 1);
}

/* render some examples, by using different methods */
void UserInterface::renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColor4f(1.0f, 0.3f, 0.0f, 0.6f);

	/* scale our box down */
	glScalef(0.3f, 0.3f, 0.3f);


	/* TODO: Method 1: Use glDrawElements with indexed triangles, best performance */


	/* Method 2: Copy arrays to graphics card and draw at once, medium performance */

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, DummyBox::front);
	glDrawArrays(GL_LINE_STRIP, 0, (sizeof(DummyBox::front) / sizeof(GLfloat)) / 3);

	glVertexPointer(3, GL_FLOAT, 0, DummyBox::back);
	glDrawArrays(GL_LINE_STRIP, 0, (sizeof(DummyBox::back) / sizeof(GLfloat)) / 3);

	glVertexPointer(3, GL_FLOAT, 0, DummyBox::join);
	glDrawArrays(GL_LINES, 0, (sizeof(DummyBox::join) / sizeof(GLfloat)) / 3);

	glDisableClientState(GL_VERTEX_ARRAY);


	/* Method 3: Draw each strip, bad performance */

	glFrontFace(GL_CW);

	glBegin(GL_TRIANGLE_STRIP);

	for(std::size_t i = 0; i < sizeof(DummyTriangle::stripes) / sizeof(GLfloat); i += 3) {
		glVertex3f(DummyTriangle::stripes[i + 0], DummyTriangle::stripes[i + 1], DummyTriangle::stripes[i + 2]);
	}

	glEnd();
}

#include <functional>
#include <stdexcept>
#include <GL/glfw.h>
#include <iostream>
#include <sstream>

#include <config.h>

#include "CodeTables/UserEventsTable.hpp"
#include "MessageTypes/IntMessage.hpp"
#include "MessageTypes/StringMessage.hpp"
#include "UserInterface.hpp"
#include "DummyModels.hpp"

using std::bind;
using std::function;
using std::noskipws;
using std::runtime_error;
using std::shared_ptr;
using std::stringstream;

using namespace std::placeholders;
using namespace ProjectMaya;


// c callbacks
typedef function<void(int, int)> TkeyCallbackFunc;
TkeyCallbackFunc keyCallbackFunc;
void keyCallbackC(int id, int state) {
	keyCallbackFunc(id, state);
}

typedef function<void(int, int)> TcharCallbackFunc;
TcharCallbackFunc charCallbackFunc;
void charCallbackC(int codepoint, int state) {
	charCallbackFunc(codepoint, state);
}

UserInterface::UserInterface() {
}

UserInterface::~UserInterface() {
	glfwTerminate();
}

void UserInterface::operator()() {
	initGLFW();
	initOpenGL();

	render();
}

void UserInterface::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	ModulePayload::setupMessageDriver(messageDriver, firstTime);

	if (firstTime) {
		this->getMessageDriver()->createSlot("keyEvents");
		this->getMessageDriver()->createSlot("log");
		this->getMessageDriver()->createSlot("user");
	}
}

void UserInterface::initGLFW() {
	if(!glfwInit()) {
		throw new runtime_error("could not initialize glfw");
	}

	/// \todo Implement mode selection, not just querying.
	queryVideoModes();

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);

	if(!glfwOpenWindow(800, 480, 0, 0, 0, 0, GLFW_DEPTH_BITS, 0, GLFW_WINDOW)) {
		glfwTerminate();
		throw new runtime_error("could not create glfw window");
	}

	glfwSetWindowTitle(NAME " - " REVISION);

	// set callbacks
	keyCallbackFunc = bind(&UserInterface::keyCallback, this, _1, _2);
	glfwSetKeyCallback(&keyCallbackC);
	charCallbackFunc = bind(&UserInterface::charCallback, this, _1, _2);
	glfwSetCharCallback(&charCallbackC);
}

void UserInterface::initOpenGL() const {
	/* transparency */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

/* main render loop */
void UserInterface::render() {
	double oldTime = glfwGetTime();
	double currentTime;
	double deltaRotate = 0.0;
	shared_ptr<MessageSlot> userSlot(this->getMessageDriver()->getSlot("user"));

	while(glfwGetWindowParam(GLFW_OPENED) && !this->shouldShutdown()) {
		// handle events
		shared_ptr<Message> m;
		while ((m = userSlot->get()).get() != nullptr) {
			// check type
			if (m->isType("int")) {
				IntMessage* m2 = dynamic_cast<IntMessage*>(m.get());

				// security check
				if (m2 != nullptr) {
					int code = m2->getData();

					if (code == UserEventTable::EXIT) {
						return;
					}
				}
			}
		}

		currentTime = glfwGetTime();
		deltaRotate += (currentTime - oldTime) * 0.1 * 360;
		oldTime = currentTime;

		renderOpenGL(deltaRotate);
		renderScene();

		glfwSwapBuffers();
	}
}

/* for now pass the rotation value as argument */
void UserInterface::renderOpenGL(const double& deltaRotate) const {
	glLoadIdentity();

	/* rotate around the y and z axes */
	glRotatef(deltaRotate, 0, 1, 0);
	glRotatef(deltaRotate, 0, 0, 1);
}

/* render some examples, by using different methods */
void UserInterface::renderScene() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColor4f(1.0f, 0.3f, 0.0f, 0.6f);

	/* scale our box down */
	glScalef(0.3f, 0.3f, 0.3f);


	/// \todo Method 1: Use glDrawElements with indexed triangles, best performance */


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

void UserInterface::keyCallback(int id, int state) {
	int code = id;
	if (state == GL_FALSE) {
		code *= -1;
	}
	this->getMessageDriver()->getSlot("keyEvents")->emit(IntMessage(code));
}

void UserInterface::charCallback(int codepoint, int state) {
	if ((state == GLFW_PRESS) && (codepoint <= 255)) {
		stringstream stream;
		stream << static_cast<unsigned char>(codepoint);
		this->getMessageDriver()->getSlot("keyEvents")->emit(StringMessage(stream.str()));
	}
}

void UserInterface::queryVideoModes() {
	GLFWvidmode list[200];
	int nummodes = glfwGetVideoModes(list, 200);

	stringstream stream;

	stream << noskipws << "detected video modes: ";

	for(int i = 0; i < nummodes; ++i)
		 stream << list[i].Width << "x" << list[i].Height << " ";

	this->getMessageDriver()->getSlot("log")->emit(StringMessage(stream.str()));
}

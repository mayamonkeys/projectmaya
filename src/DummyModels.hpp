#ifndef DUMMYOBJECTS_H
#define DUMMYOBJECTS_H

#include <GL/gl.h>


namespace ProjectMaya {

	/**
	 * Data for a const Dummy Box.
	 * You probably have to scale the values to your needs
	 */
	namespace DummyBox {
		static const GLfloat back[] = {
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0};

		static const GLfloat front[] = {
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, -1.0, 1.0};

		static const GLfloat join[] = {
			-1.0, -1.0, 1.0,
			-1.0, -1.0, -1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0,
			1.0, -1.0, 1.0,
			1.0, -1.0, -1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, -1.0};
	};

}

#endif

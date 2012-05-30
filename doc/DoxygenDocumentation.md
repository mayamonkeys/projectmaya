Doxygen documentation
=====================

Preparation
-----------

In order to build the doxygen documentation, you need the latest doxygen and dot from graphviz for svg diagrams.

	svn co https://doxygen.svn.sourceforge.net/svnroot/doxygen/trunk doxygen-svn
	cd doxygen-svn
	./configure
	make -j 2
	sudo make install


Documenting the source
----------------------

Example usage of doxygen commands.

	/**
	 * Handles the terrain creation and management.
	 */
	class TerrainRenderer : public Renderer {
		public:
			explicit TerrainRenderer();
			/// \bug Memory leak here, do proper cleanup in destructor.
			~TerrainRenderer();

		private:
			/// \todo implement the loading
			void loadTerrain();
	};

 * Use \bug and \todo where possible, because a todolist and a buglist will be created from them
 * Use \brief, \note, \warning and \attention, where appropriate
 * You are able to use the Markdown language, but use it only if needed

Doxygen [commands](http://www.stack.nl/~dimitri/doxygen/commands.html)


Building the documentation
--------------------------

Now you are able to build the html documentation and view it in your browser.

	cd doc/doxygen
	doxygen Doxyfile
	chrome html/index.html

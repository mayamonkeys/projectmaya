Doxygen documentation
=====================

In order to build the doxygen documentation, you need the latest doxygen and dot from graphviz for svg diagrams.

	svn co https://doxygen.svn.sourceforge.net/svnroot/doxygen/trunk doxygen-svn
	cd doxygen-svn
	./configure
	make -j 2
	sudo make install


Now you are able to build the html documentation and view it in your browser.
	cd doc/doxygen
	doxygen Doxyfile
	chrome html/index.html


[Doxygen commands](http://www.stack.nl/~dimitri/doxygen/commands.html)

Developement Tools
==================



Cppcheck
--------

Cppcheck checks the source code for performance, style, and portability issues.

Get the latest copy of Cppcheck from github, in order to check against the C++11 standard.

	git clone --depth 1 git://github.com/danmar/cppcheck.git
	cd cppcheck
	make

Now check the source code.

	cd src
	cppcheck --enable=all --std=c++11 .


Valgrind
--------

Valgrind checks for memory leaks and helps profiling.

	cd build
	valgrind --leak-check=full ./projectmaya

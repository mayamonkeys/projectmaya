Developement Tools
==================



Cppcheck
--------

Cppcheck checks the source code for performance, style, and portability issues.

Get the latest copy of Cppcheck from github, in order to check against the C++11 standard.

	git clone --depth 1 git://github.com/danmar/cppcheck.git
	cd cppcheck
	git checkout 1.54
	make

Now check the source code.

	cd src
	cppcheck --enable=all --std=c++11 .


Valgrind
--------

Valgrind checks for memory leaks and helps profiling.

	cd build
	valgrind --leak-check=full ./projectmaya


GDB
---

GDB is the GNU Debugger. We'll need a recent version of it, in order to work with our LLVM tools. Adapt the configure command to your needs.

	git clone --depth 1 git://sourceware.org/git/gdb.git
	cd gdb
	git checkout gdb_7_4_1-2012-04-26-release
	CC="gcc-4.7" CXX="g++-4.7" ./configure --prefix=/usr/local --program-suffix=-7.4 --enable-gold --enable-cloog-backend=isl --enable-lto --with-mpc=/usr/local --with-mpfr=/usr/local --with-gmp=/usr/local --with-ppl=/usr/local --with-cloog=/usr/local
	make
	sudo make install

In order to debug the program, we have to tell waf to build it in debug mode (--debug). After that, we are able to issue the command.

	gdb-7.4 build/projectmaya

Now your should see "Reading symbols from...done." If not, you haven't compiled the program in debug mode.

Set your breakpoints and specific values (see help for more information on this), start the program with the "start" command and e.g. step forward one instruction with "ni".

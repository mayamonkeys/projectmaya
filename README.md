Project Maya
============

This is an experimental game, which will combine flavored 2D graphic with social interaction and real world informations.


Requirements
------------

Software:
 * Python 2.4+/3.0+
 * llvm + clang 3.1
 * libc++

 
Build
-----

To build we project, we use a system called waf. First, you have to configure the project:

	python waf configure

Fix all reported erros and run the command again (as long as everyting works). Then cross your fingers and build the project:

	python waf build

If waf reported a successful build, you can start the game using the following mystical command:

	./build/projectmaya

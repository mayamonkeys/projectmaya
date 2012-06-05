Project Maya
============

Experimental game, combining flavored 2D graphics with social interaction and real world information.


Requirements
------------

 * Python 2.4+/3.0+
 * llvm + clang 3.1
 * libc++


Building
--------

To build we project, use the provided waf buildsystem.

	python waf configure
	python waf build

You are now able to start the game.

	./build/projectmaya


Documentation
-------------

To get you startet, you should read the first two documents. They are about the toolchain setup and the overall design.

* [Toolchain Preparation](https://github.com/mayamonkeys/projectmaya/blob/master/doc/ToolchainPreparation.md) How to build the toolchain, consisting of llvm/clang/libc++/libc++abi
* [Structural Design](https://github.com/mayamonkeys/projectmaya/blob/master/doc/StructuralDesign.md) Overall design decisions

If you want additional information, read the following documents.

* [Developement Tools](https://github.com/mayamonkeys/projectmaya/blob/master/doc/DevelopementTools.md) Helper tools for the developement process
* [Coding Conventions](https://github.com/mayamonkeys/projectmaya/blob/master/doc/CodingConventions.md) Conventions for source code developement
* [Doxygen Documentation](https://github.com/mayamonkeys/projectmaya/blob/master/doc/DoxygenDocumentation.md) Building the Doxygen documentation from the sources

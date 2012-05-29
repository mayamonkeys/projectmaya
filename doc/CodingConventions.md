Coding Conventions
==================



C++ specific
------------
* Order of includes: C header, C++ header, other library header, project header
* Avoid macros, they can't be type checked by the compiler
* No using directive in header, since this pollutes the namespace
* Avoid multiple inheritance
* Order of declaration: public, protected, private
* Use smart pointer if you need pointer semantics
* Pass arguments by constant reference where possible
* Use C++ casts, e.g. static_cast<int>() not C casts, like c = (int) i;
* Use include header guards to avoid double inclusion, name them like the file, e.g. PRETTY_PRINT_H


Code style
------------
* Indent with 1 tab
* BSD style braces, i.e.

	void PrettyPrinter::prettyPrint(const container& c) {
	  for(const auto& x : c) {
	    cout << x << endl;
	  }
	}
 * Line length soft limit 120 characters


Naming Conventions
------------------
* Classes upper CamelCase, e.g. PrettyPrinter
* Methods lower CamelCase, e.g. prettyPrint
* Variables lower CamelCase, e.g. int speed = 0;
* Acronyms as is, e.g. float GPAScore = 2.4f;

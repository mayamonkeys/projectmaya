TOOLCHAIN
=========

libc++
------

This lib is the C++ STL. First, download it:

	svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx
	cd libcxx

If you want to use some multicore stuff, open *include/atomic* and remove the `!` from the following line `#if !__has_feature(cxx_atomic)`:

	sed -i 's/#if !__has_feature(cxx_atomic)/#if __has_feature(cxx_atomic)/' include/atomic

Now, build it:

	cd lib
	./buildit

Because there is no install script, you have to it manually:

	sudo mkdir -p /usr/local/lib
	sudo cp libc++.so.1.0 /usr/local/lib/libc++.so.1.0
	sudo ln -s /usr/local/lib/libc++.so.1.0 /usr/local/lib/libc++.so.1
	sudo ln -s /usr/local/lib/libc++.so.1 /usr/local/lib/libc++.so
	sudo ldconfig
	cd ..
	sudo mkdir -p /usr/local/include/c++
	sudo cp -r include /usr/local/include/c++/v1
	sudo mkdir -p /usr/include/c++
	sudo ln -s /usr/local/include/c++/v1 /usr/include/c++/v1
	cd ..

After this, you can use the lib:

	clang++ -stdlib=libc++ main.cpp
	clang++ -std=c++0x -stdlib=libc++ main_with_c++11_features.cpp


libc++abi
---------

If you get an ABI link error, when you try to use the library, you have to compile on own ABI. First, download a helper lib:

	wget http://download.savannah.gnu.org/releases/libunwind/libunwind-1.0.1.tar.gz
	tar -xzvf libunwind-1.0.1.tar.gz

Now, you can download libc++abi:

	svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi
	cd libcxxabi

Next, compile it using libunwind:

	cd lib
	CPATH=../../libunwind-1.0.1/include:$CPATH ./buildit

Finally, install it:

	sudo mkdir -p /usr/local/lib
	sudo cp libc++abi.so.1.0 /usr/local/lib/libc++abi.so.1.0
	sudo ln -s /usr/local/lib/libc++abi.so.1.0 /usr/local/lib/libc++abi.so.1
	sudo ln -s /usr/local/lib/libc++abi.so.1 /usr/local/lib/libc++abi.so
	sudo ldconfig

To use the libs and link your program correctly, use the following command structure:

	clang++ -std=c++0x -stdlib=libc++ -lc++abi main.cpp
	clang++ -std=c++0x -stdlib=libc++ -lc++abi main_with_c++11_features.cpp


ldconfig problems
-----------------

If your linker/compiler builds a program correct, but the libs weren't found, when you try to start the program, your distribution does not search libs in */usr/local/lib*. You can fix this using the following workaround:

	sudo echo "/usr/local/lib" > /etc/lc.so.conf.d/usr-local-lib.conf
	sudo ldconfig


gold linker
-----------

If you use the gold linker, you have to install the libs to */usr/lib*:

	sudo ln -s /usr/local/lib/libc++.so /usr/lib/libc++.so
	sudo ln -s /usr/local/lib/libc++abi.so /usr/lib/libc++abi.so


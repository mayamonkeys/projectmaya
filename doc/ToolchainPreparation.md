Toolchain Preparation
=====================

In order to get you startet, you will have to compile the toolchain, consisting of llvm/clang/libc++/libc++abi.


LLVM and Clang
--------------

We need to checkout the LLVM, clang and compiler-rt tree, all into a directory, ~/llvm.

          svn co http://llvm.org/svn/llvm-project/llvm/trunk ~/llvm
          cd ~/llvm/tools
          svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
          cd ~/llvm/projects
          svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt

I had to use gcc to compile llvm, because my local copy of clang was too old and couldn't build the snapshot. We use a seperate directory for the building, ~/llvm-build.

          export CC="gcc" CXX="g++"
          mkdir ~/llvm-build
          cd ~/llvm-build
          ../llvm/configure --enable-optimized --enable-jit
          make -j 2
          sudo make install

Updating LLVM (and all subprojects and tools) is as simple as issuing make update in the root directory. From now on you should be able to let clang build itself.

          cd ~/llvm-build
          make update


libc++
------

In order to use the latest C++11 features like regular expressions (which are still not fully implemented in libstdc++), or in case we have a libstdc++ which is not compatible with clang, we have to compile LLVM's libc++.

We do not checkout libc++ into the root LLVM tree, because the buildsystem is not compatible with Linux. I have written a quick and dirty replacement, which automates the build. Get it [from here](https://github.com/downloads/mayamonkeys/projectmaya/Makefile.libcxx.linux) and customize it, if needed.

          svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx
          cd libcxx
          make -f Makefile.libcxx.linux
          sudo make -f Makefile.libcxx.linux install

From now on we are able to tell clang to use libc++ instead of libstdc++.

          clang++ -stdlib=libc++ -std=c++11 regex.cpp


libc++abi
---------

If you want to compile your program with libc++, but your ABI is incompatible, you have to manually link against libc++abi. In case you installed GCC 4.7, you normally do not have to do this. Again, grab a simple Makefile [from here](https://github.com/downloads/mayamonkeys/projectmaya/Makefile.libcxxabi.linux), and follow the steps below.

          mkdir libcxxabi-libunwind
          cd libcxxabi-libunwind
          svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi
          git clone git://git.sv.gnu.org/libunwind.git
          cd libcxxabi
          make -f Makefile.libcxxabi.linux
          sudo make -f Makefile.libcxxabi.linux install

Link against it like this.

          clang++ -stdlib=libc++ -lc++abi -std=c++11 regex.cpp

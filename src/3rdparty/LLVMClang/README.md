libOpenCOR only needs a subset of the LLVM+Clang libraries, so we do everything we can to minimise the LLVM+Clang footprint.

Otherwise, LLVM+Clang requires the changes captured in patches/llvm-lib-support-commandline.patch to build and work correctly with libOpenCOR.

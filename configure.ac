AC_PREREQ([2.57])

AC_INIT([distributedcl],[0.1.0])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign dist-bzip2])

LT_INIT([dlopen])
LT_PREREQ([1.0])

AC_LANG(C++)
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_HEADER_RESOLV

AX_BOOST_BASE([1.40.0])
AX_BOOST_THREAD
AX_BOOST_PROGRAM_OPTIONS

CXXFLAGS=" -g -O3 -Wall -fno-strict-aliasing "

# check for include files
AC_CHECK_HEADERS([\
CL/opencl.h \
CL/cl_gl.h \
CL/cl_gl_ext.h \
GL/gl.h \
GL/glu.h \
boost/scoped_ptr.hpp \
boost/shared_ptr.hpp \
boost/tokenizer.hpp \
boost/random/uniform_int.hpp \
boost/random/variate_generator.hpp \
boost/random/mersenne_twister.hpp \
boost/interprocess/shared_memory_object.hpp \
boost/interprocess/sync/named_mutex.hpp \
boost/interprocess/sync/scoped_lock.hpp \
boost/interprocess/sync/interprocess_mutex.hpp],
[], [AC_MSG_ERROR([unable to find the required header])])

# check libraries
AC_SEARCH_LIBS([dlopen], [dl], [], [AC_MSG_ERROR([unable to find the dlopen() function])])


AC_OUTPUT([Makefile
           src/libdistributedcl/Makefile
           src/distributedcld/Makefile
           src/libopencl/Makefile
           src/CLInfo/Makefile])

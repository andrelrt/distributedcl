lib_LTLIBRARIES = libdcl.la
libdcl_la_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/inc -I$(top_srcdir)/src/libdistributedcl
libdcl_la_LDFLAGS = @LIBS@ $(BOOST_LDFLAGS) $(BOOST_THREAD_LIB) $(BOOST_PROGRAM_OPTIONS_LIB)
libdcl_la_LIBADD = ../libdistributedcl/libdistributedcl.la $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
libdcl_la_SOURCES = main.cpp \
icd/icd_object_manager.cpp \
icd/icd_object_manager.h \
interface/cl_command_queue.cpp \
interface/cl_context.cpp \
interface/cl_event.cpp \
interface/cl_device.cpp \
interface/cl_kernel.cpp \
interface/cl_memory.cpp \
interface/cl_platform.cpp \
interface/cl_program.cpp \
interface/cl_utils.cpp \
interface/cl_utils.h

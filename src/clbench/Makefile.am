bin_PROGRAMS = clbench
clbench_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/inc
clbench_LDFLAGS = @LIBS@ $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_THREAD_LIB) $(BOOST_SYSTEM_LIB)
clbench_LDADD =  /usr/lib/libOpenCL.so -lboost_chrono -lboost_timer
clbench_SOURCES = \
main.cpp \
clbench.h \
clbench.cpp

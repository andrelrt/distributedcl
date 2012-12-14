bin_PROGRAMS = clbench
clbench_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/inc
clbench_LDFLAGS = @LIBS@ $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB)
clbench_LDADD =  -lOpenCL
clbench_SOURCES = \
main.cpp \
clbench.cpp

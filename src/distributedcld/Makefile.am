bin_PROGRAMS = dcld
dcld_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/inc -I$(top_srcdir)/src/libdistributedcl
dcld_LDFLAGS = @LIBS@ $(BOOST_LDFLAGS) $(BOOST_THREAD_LIB) $(BOOST_PROGRAM_OPTIONS_LIB)
dcld_LDADD = ../libdistributedcl/libdistributedcl.la
dcld_SOURCES = main.cpp

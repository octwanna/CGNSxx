####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libtest.a

## INCLUDES were found outside kdevelop specific part

libtest_a_METASOURCES = AUTO

libtest_a_SOURCES = adfppTest.cpp 


EXTRA_DIST = adfppTest.cpp 

####### kdevelop will overwrite this part!!! (end)############
#noinst_LIBRARIES =

check_PROGRAMS = adfppTest

adfppTest_SOURCES = $(libtest_a_SOURCES)

adfppTest_DEPENDENCIES = ../libadfpp.a

# set the include path found by configure
INCLUDES = $(all_includes) -I..

# CXXLD = @CXXLD@

# the library search path.
adfppTest_LDADD = $(all_libraries) -L.. -ladfpp

TESTS = adfppTest

####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libtest.a

## INCLUDES were found outside kdevelop specific part

libtest_a_METASOURCES = AUTO

libtest_a_SOURCES = test5.cpp test4.cpp test0.cpp test3.cpp test2.cpp test1.cpp cgnsppTest.cpp 


EXTRA_DIST = Test5blocks.out Testbump.out Testdelta.out Testjet.out Testmulti.out Testtut21.out runtests updatetests Test1.out README cgnsppTest.cpp test.h test1.cpp test2.cpp test3.cpp test0.cpp Test2.out Test3.out test4.cpp test5.cpp 

####### kdevelop will overwrite this part!!! (end)############
#noinst_LIBRARIES =

check_PROGRAMS = cgnsppTest

cgnsppTest_SOURCES = $(libtest_a_SOURCES)

cgnsppTest_DEPENDENCIES = ../libcgnspp.a ../../adfpp/libadfpp.a

# set the include path found by configure
INCLUDES = $(all_includes) -I.. -I../../adfpp

# the library search path.
cgnsppTest_LDADD = $(all_libraries) -L.. -L../../adfpp -lcgnspp -ladfpp

TESTS = runtests

update_tests: updatetests
	updatetests

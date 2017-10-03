####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libexample.a

## INCLUDES were found outside kdevelop specific part

libexample_a_METASOURCES = AUTO

libexample_a_SOURCES = ug6.cpp ug5.cpp ug4.cpp example_main.cpp ug3.cpp ug2.cpp ug1.cpp 


EXTRA_DIST = ug1.cpp ug2.cpp ug3.cpp example_main.cpp ug4.cpp ug5.cpp ug6.cpp runexamples 

####### kdevelop will overwrite this part!!! (end)############
#noinst_LIBRARIES =

check_PROGRAMS = ug1 ug2 ug3 ug4 ug5 ug6

ug1_SOURCES = ug1.cpp
ug2_SOURCES = ug2.cpp
ug3_SOURCES = ug3.cpp
ug4_SOURCES = ug4.cpp
ug5_SOURCES = ug5.cpp
ug6_SOURCES = ug6.cpp

INCLUDES = $(all_includes) -I.. -I../../adfpp

LDADD = $(all_libraries) -L.. -L../../adfpp -lcgnspp -ladfpp 

ug1_LDFLAGS = $(INCLUDES) -DEXAMPLE=example_ug1 example_main.cpp 
ug2_LDFLAGS = $(INCLUDES) -DEXAMPLE=example_ug2 example_main.cpp
ug3_LDFLAGS = $(INCLUDES) -DEXAMPLE=example_ug3 example_main.cpp
ug4_LDFLAGS = $(INCLUDES) -DEXAMPLE=example_ug4 example_main.cpp
ug5_LDFLAGS = $(INCLUDES) -DEXAMPLE=example_ug5 example_main.cpp
ug6_LDFLAGS = $(INCLUDES) -DEXAMPLE=example_ug6 example_main.cpp

TESTS = runexamples

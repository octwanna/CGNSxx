####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libadfpp.a

INCLUDES = $(all_includes)

libadfpp_a_METASOURCES = AUTO

libadfpp_a_SOURCES = adfutil.cpp adfnode.cpp adffile.cpp adfexception.cpp ADF_internals.c ADF_interface.c 

SUBDIRS = . test 

EXTRA_DIST = adf++.h ADF.h ADF_interface.c ADF_internals.c ADF_internals.h adfexception.cpp adffile.cpp adfnode.cpp adfutil.h adfutil.cpp adfnode.h

install-data-local:
	$(mkinstalldirs) $(includedir)/
	$(INSTALL_DATA) $(srcdir)/adf++.h $(includedir)/adf++.h

uninstall-local:
	-rm -f $(includedir)/adf++.h

####### kdevelop will overwrite this part!!! (end)############

#noinst_LIBRARIES =
lib_LIBRARIES = libadfpp.a

#SUBDIRS = . test


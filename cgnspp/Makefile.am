####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libcgnspp.a

## INCLUDES were found outside kdevelop specific part

libcgnspp_a_METASOURCES = AUTO

libcgnspp_a_SOURCES = cgnsgeometryreference.cpp cgnsmodel.cpp cgnsnodeutil.cpp cgnsparented.cpp cgnsoversetholes.cpp cgnsgridconnectivity.cpp cgnsgridconnectivity1to1.cpp cgnsbcdata.cpp cgnsbcdataset.cpp cgnsbc.cpp cgnsuserdefineddata.cpp cgnsindexarray.cpp cgnsarbitrarygridmotion.cpp cgnsrigidgridmotion.cpp cgnszonebc.cpp cgnszonegridconnectivity.cpp cgnszoneiterativedata.cpp cgnsdiscretedata.cpp cgnsdiscreted.cpp cgnselements.cpp cgnsfamily.cpp cgnsbaseiterativedata.cpp cgnsvalued.cpp cgnsreferencestate.cpp cgnsequationset.cpp cgnsconvergence.cpp cgnsconverged.cpp cgnsgridded.cpp cgnsarrayed.cpp cgnsstated.cpp cgnsdataarray.cpp cgnsdimensioned.cpp cgnsbroker.cpp cgnsutil.cpp cgnssolution.cpp cgnsgridcoordinates.cpp cgnszone.cpp cgnsbase.cpp cgnsexception.cpp cgnsfile.cpp cgnsnode.cpp 

SUBDIRS = . docs test example 

EXTRA_DIST = cgns++.h cgnsnode.cpp cgnsfile.cpp cgnsexception.cpp cgnsbase.cpp cgnszone.cpp cgnsgridcoordinates.cpp cgnssolution.cpp cgnsutil.h cgnsutil.cpp cgnsbroker.cpp cgnsdimensioned.cpp cgnsdataarray.cpp cgnsstated.cpp cgns.dot cgns++.dot cgnsarrayed.cpp cgnsgridded.cpp cgnsnode.h cgnsdimensioned.h cgnsstated.h cgnsbase.h cgnszone.h cgnsconverged.cpp cgnsconverged.h cgnsdataarray.h cgnsgridded.h cgnsarrayed.h cgnssolution.h cgnsgridcoordinates.h cgnsconvergence.h cgnsconvergence.cpp cgnsequationset.cpp cgnsequationset.h cgnsbroker.h cgnsreferencestate.cpp cgnsreferencestate.h cgnsvalued.cpp cgnsvalued.h cgnsbaseiterativedata.cpp cgnsbaseiterativedata.h cgnsfamily.cpp cgnsfamily.h cgnselements.cpp cgnselements.h cgnsdiscreted.cpp cgnsdiscreted.h cgnsdiscretedata.cpp cgnsdiscretedata.h cgnszoneiterativedata.cpp cgnszoneiterativedata.h cgnszonegridconnectivity.cpp cgnszonegridconnectivity.h cgnszonebc.cpp cgnszonebc.h cgnsrigidgridmotion.cpp cgnsrigidgridmotion.h cgnsarbitrarygridmotion.cpp cgnsarbitrarygridmotion.h cgnsindexarray.cpp cgnsindexarray.h cgnsuserdefineddata.cpp cgnsuserdefineddata.h cgnsbc.cpp cgnsbc.h cgnsbcdataset.cpp cgnsbcdataset.h cgnsbcdata.cpp cgnsbcdata.h cgnsgridconnectivity1to1.cpp cgnsgridconnectivity1to1.h cgnsgridconnectivity.cpp cgnsgridconnectivity.h cgnsoversetholes.cpp cgnsoversetholes.h cgnsparented.cpp cgnsparented.h cgnsnodeutil.cpp cgnsnodeutil.h cgnstraits.h cgnsmodel.h cgnsmodel.cpp cgnsgeometryreference.cpp cgnsgeometryreference.h 

install-data-local:
	$(mkinstalldirs) $(includedir)/
	$(INSTALL_DATA) $(srcdir)/cgns++.h $(includedir)/cgns++.h

uninstall-local:
	-rm -f $(includedir)/cgns++.h

####### kdevelop will overwrite this part!!! (end)############

#noinst_LIBRARIES =
lib_LIBRARIES = libcgnspp.a

#SUBDIRS = . docs test example

# set the include path found by configure
INCLUDES= $(all_includes) -I../adfpp


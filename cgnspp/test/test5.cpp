/***************************************************************************
                          test5.cpp  -  description
                             -------------------
    begin                : Mon Feb 17 2003
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgns++.h"

#include "test.h"

#include <iostream>

#if _MSC_VER > 1300
	#include "process.h"
#else
	#include <unistd.h>
#endif

using std::cerr;
using std::endl;

int elements[][3] = {
	{1, 2, 3}
};

int elementEdges[][3] = {
	{1, 2, 3}
};

void CreateTest5(TempFile & file) {
	cerr << "Now we try to create a new CGNS database.\n";
	CGNS::File tempFile(file.mName, CGNS::File::WRITE);
	cerr << "CGNS Open of temporary file (" << file.mName
		<< ") seems to have been successfull.\n";
	CGNS::Base base=tempFile.writeBase("Base", 1, 1);
    int numE=1, numP=3;
	CGNS::Zone zone=base.writeUnstructuredZone("Zone", numP, numE, 3);
	cerr << "zone has been written\n";
	CGNS::Elements elems=zone.writeElements("Triangles", CGNS::Elements::TRI_3);
    CGNS::IndexRange1D elemRange;
    elemRange.mMinI=1;
    elemRange.mMaxI=numE;
   	elems.writeElementRange(elemRange);
    cerr << "range has been written\n";
   	CGNS::DataArray connect=elems.getElementConnectivity();
   	connect.setDataTypeDimensions(CGNS::DataArray::INT, numE*3);
   	connect.writeData(&elements[0][0]);
//cerr << "Please copy file " << file.mName << endl;
//sleep(10);
	cerr << "########### Test5: Finished. Ok! ###########\n";
}


/***************************************************************************
                          test1.cpp  -  description
                             -------------------
    begin                : Thu Mar 28 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgns++.h"

#include "test.h"

#include <iostream>

using std::cerr;
using std::endl;

void CreateTest1(TempFile & file) {
	cerr << "Now we try to create a new CGNS database.\n";
	CGNS::File tempFile(file.mName, CGNS::File::WRITE);
	cerr << "CGNS Open of temporary file (" << file.mName
		<< ") seems to have been successfull.\n";
	CGNS::Base linearBase=tempFile.writeBase("LinearBase", 1, 1);
	int const linearVertices=6;
	cerr << "base has been written\n";
	CGNS::Zone linearStrucZone=linearBase.writeStructured1DZone("LinearStructured", linearVertices);
	CGNS::Zone linearUnstrZone=linearBase.writeUnstructuredZone("LinearUnstructured",
		linearVertices, linearVertices-1, 2);
	cerr << "zones have been written\n";
	CGNS::GridCoordinates linearStrucGrid=linearStrucZone.writeGridCoordinates();
	CGNS::GridCoordinates linearUnstrGrid=linearUnstrZone.writeGridCoordinates();
	cerr << "GridCoordinates have been written\n";
	CGNS::DataArray linearStrucX=linearStrucGrid.writeCoordinate(CGNS::GridCoordinates::COORDINATE_X);
	CGNS::DataArray linearUnstrX=linearUnstrGrid.writeCoordinate(CGNS::GridCoordinates::COORDINATE_X);
	cerr << "coordinates have been created\n";
	std::vector<double> coorX(linearVertices, 0);
	for (unsigned i=0; i<coorX.size(); ++i)
		coorX[i]=i;
	linearStrucX.writeData(&coorX[0]);
	coorX[0]=-linearVertices+1;
	for (unsigned i=1; i<coorX.size(); ++i)
		coorX[i]=1.0-i;
	linearUnstrX.writeData(&coorX[0]);
	cerr << "coordinates have been written\n";
	CGNS::FlowSolution linearStrucSol=linearStrucZone.writeFlowSolution("Solution");
	cerr << "solution has been created\n";
	CGNS::DataArray linearStrucVx=linearStrucSol.writeField(CGNS::FlowSolution::VELOCITY_X);
	std::vector<double> vX(linearVertices, 0);
	for (unsigned i=0; i<vX.size(); ++i)
		vX[i]=i*0.5;
	linearStrucVx.writeData(&vX[0]);
	cerr << "solution has been written\n";
	cerr << "########### Test1: Finished. Ok! ###########\n";
}


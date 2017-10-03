/***************************************************************************
                          test2.cpp  -  description
                             -------------------
    begin                : Fri Oct 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgns++.h"

#include "test.h"

#include <iostream>

using std::cerr;
using std::endl;

static int const cellDim=3, physicalDim=3;
static int const verticesI=21, verticesJ=17, verticesK=9;
typedef std::vector<double> array_t;
static array_t cX, cY, cZ;
static array_t vX, vY, vZ, r, p, T;
static CGNS::Rind3D rind;

static void fillData() {
	int numV=verticesI*verticesJ*verticesK;
	// coordinates
	cX.resize(numV);
	cY.resize(numV);
	cZ.resize(numV);
	for (int i=0; i<numV; ++i)
		cX[i]=cY[i]=cZ[i]=i;
	// velocities
	vX.resize(numV);
	vY.resize(numV);
	vZ.resize(numV);
	for (int i=0; i<numV; ++i)
		vX[i]=vY[i]=vZ[i]=i;
	// density, pressure (cell centered)
	int numC=(verticesI-1)*(verticesJ-1)*(verticesK-1);
	r.resize(numC);
	p.resize(numC);
	for (int i=0; i<numC; ++i)
		r[i]=p[i]=i;
	
	rind.mMinI=rind.mMaxI=1;
	rind.mMinJ=rind.mMaxJ=1;
	rind.mMinK=rind.mMaxK=1;
	// temperature (cell centered with rind)
	int numT=(verticesI-1-rind.mMinI-rind.mMaxI)
		*(verticesJ-1-rind.mMinJ-rind.mMaxJ)
		*(verticesK-1-rind.mMinK-rind.mMaxK);
	T.resize(numT);
	for (int i=0; i<numT; ++i)
		T[i]=i;
}

void CreateTest2(TempFile & file) {
	fillData();
	cerr << "Now we try to create a new CGNS database.\n";
	CGNS::File tempFile(file.mName, CGNS::File::WRITE);
	cerr << "CGNS Open of temporary file (" << file.mName
		<< ") seems to have been successfull.\n";
	CGNS::Base base=tempFile.writeBase("ExampleBase#1", cellDim, physicalDim);
	base.writeDescriptor("README", "Simple 3D structured grid example");
	cerr << "base has been written\n";
	CGNS::Zone zone=base.writeStructured3DZone("ExampleZone#1",
		verticesI, verticesJ, verticesK);
	cerr << "zone has been written\n";
	CGNS::GridCoordinates grid=zone.writeGridCoordinates();
	cerr << "grid has been written\n";
	grid.writeData(CGNS::GridCoordinates::COORDINATE_X, &cX[0]);
	grid.writeData(CGNS::GridCoordinates::COORDINATE_Y, &cY[0]);
	grid.writeData(CGNS::GridCoordinates::COORDINATE_Z, &cZ[0]);
	cerr << "coordinates have been written\n";
	CGNS::FlowSolution solVertex=zone.writeFlowSolution("SolutionVertex", CGNS::VERTEX);
	CGNS::FlowSolution solCell=zone.writeFlowSolution("SolutionCellCenter", CGNS::CELL_CENTER);
	CGNS::FlowSolution solRind=zone.writeFlowSolution("SolutionRind", CGNS::CELL_CENTER, rind);
	cerr << "solutions have been created\n";
	solVertex.writeData(CGNS::FlowSolution::VELOCITY_X, &vX[0]);
	solVertex.writeData(CGNS::FlowSolution::VELOCITY_Y, &vY[0]);
	solVertex.writeData(CGNS::FlowSolution::VELOCITY_Z, &vZ[0]);
	solCell.writeData(CGNS::FlowSolution::DENSITY, &r[0]);
	solCell.writeData(CGNS::FlowSolution::PRESSURE, &p[0]);
	solRind.writeData(CGNS::FlowSolution::TEMPERATURE, &T[0]);
	cerr << "fields have been written\n";
	cerr << "########### Test2: Finished. Ok! ###########\n";
}


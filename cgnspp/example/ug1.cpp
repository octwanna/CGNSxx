/***************************************************************************
                          ug1.cpp  -  description
                             -------------------
    begin                : Fri Oct 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "../cgns++.h"

#include <iostream>

void example_ug1() {
	int const I=21, J=17, K=9;
	double x[K][J][I],y[K][J][I],z[K][J][I];
	// create grid points
	for (int i=0; i<I; ++i)
		for (int j=0; j<J; ++j)
			for (int k=0; k<K; ++k) {
				x[k][j][i]=i;
				y[k][j][i]=j;
				z[k][j][i]=k;
			}
	// WRITE X, Y, Z GRID POINTS TO CGNS FILE
	CGNS::File file("grid_c++.cgns", CGNS::File::WRITE);
	int const cellDim=3, physicalDim=3;
	CGNS::Base base=file.writeBase("Base", cellDim, physicalDim);
	CGNS::Zone zone=base.writeStructured3DZone("Zone", I, J, K);
	CGNS::GridCoordinates grid=zone.writeGridCoordinates();
	// this is a shortcut way to write data
	grid.writeData(CGNS::GridCoordinates::COORDINATE_X, &x[0][0][0]);
	grid.writeData(CGNS::GridCoordinates::COORDINATE_Y, &y[0][0][0]);
	// we can do it as well in two steps
	CGNS::DataArray coordZ=grid.writeCoordinate(CGNS::GridCoordinates::COORDINATE_Z);
	coordZ.writeData(&z[0][0][0]);
	std::cerr << "Successfully wrote grid to file grid_c++.cgns\n";
}

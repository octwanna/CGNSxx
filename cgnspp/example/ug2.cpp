/***************************************************************************
                          ug2.cpp  -  description
                             -------------------
    begin                : Mon Oct 29 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "../cgns++.h"

#include <iostream>

void example_ug2() {
	// READ X, Y, Z GRID POINTS FROM CGNS FILE
	CGNS::File file("grid_c++.cgns", CGNS::File::READ);
	CGNS::Base base=*file.beginBase();
	CGNS::Zone zone=*base.beginZone();
	int const V=zone.getTotalVertices();
	CGNS::GridCoordinates grid=zone.getGridCoordinates();
	std::vector<double> x(V), y(V), z(V);
	grid.readData(CGNS::GridCoordinates::COORDINATE_X, &x[0]);
	grid.readData(CGNS::GridCoordinates::COORDINATE_Y, &y[0]);
	grid.readData(CGNS::GridCoordinates::COORDINATE_Z, &z[0]);
	std::cerr << "Successfully read grid from file grid_c++.cgns\n";
}

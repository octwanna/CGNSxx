/***************************************************************************
                          ug6.cpp  -  description
                             -------------------
    begin                : Mon Oct 29 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "../cgns++.h"

#include <iostream>

void example_ug6() {
	// READ CELL CENTER FLOW SOLUTION FROM CGNS FILE
	CGNS::File file("grid_c++.cgns", CGNS::File::READ);
	CGNS::Base base=*file.beginBase();
	CGNS::Zone zone=*base.beginZone();
	int const C=zone.getTotalCells();
	std::vector<double> density(C, 1.0), pressure(C, 1.0/1.4);
	CGNS::FlowSolution solution=zone.getFlowSolution("CellSolution");
	if (solution.getLocation()!=CGNS::CELL_CENTER) {
		std::cerr << "Error, GridLocation must be vertex!\n";
		return;
	}
	solution.readData(CGNS::FlowSolution::DENSITY, &density[0]);
	solution.readData(CGNS::FlowSolution::PRESSURE, &pressure[0]);
	std::cerr << "Successfully read cell center solution from file grid_c++.cgns\n";
}

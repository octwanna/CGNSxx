/***************************************************************************
                          ug3.cpp  -  description
                             -------------------
    begin                : Mon Oct 29 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "../cgns++.h"

#include <iostream>

void example_ug3() {
	// WRITE SOLUTION TO CGNS FILE
	CGNS::File file("grid_c++.cgns", CGNS::File::MODIFY);
	CGNS::Base base=*file.beginBase();
	CGNS::Zone zone=*base.beginZone();
	CGNS::FlowSolution solution=zone.writeFlowSolution("FlowSolution", CGNS::VERTEX);
	int const V=zone.getTotalVertices();
	std::vector<double> density(V, 1.0), pressure(V, 1.0/1.4);
	solution.writeData(CGNS::FlowSolution::DENSITY, &density[0]);
	solution.writeData(CGNS::FlowSolution::PRESSURE, &pressure[0]);
	std::cerr << "Successfully added solution to file grid_c++.cgns\n";
}

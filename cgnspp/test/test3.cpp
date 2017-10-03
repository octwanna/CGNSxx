/***************************************************************************
                          test3.cpp  -  description
                             -------------------
    begin                : Wed March 27 2002
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
static int const nVertices = verticesI*verticesJ*verticesK;
static int const nCells    = (verticesI-1)*(verticesJ-1)*(verticesK-1);
typedef std::vector<double> array_t;
static array_t cX, cY, cZ;
static array_t vX, vY, vZ, r, p;


//-----------------------------------------------------------------------------
//
//  fillData
//

static void fillData() 
{
  // coordinates
  cX.resize(nVertices);
  cY.resize(nVertices);
  cZ.resize(nVertices);

  for (int i=0; i<nVertices; ++i){
    cX[i] = cY[i] = cZ[i] = i;
  }
  
  // velocities
  vX.resize(nVertices);
  vY.resize(nVertices);
  vZ.resize(nVertices);

  for (int i=0; i<nVertices; ++i){
    vX[i] = vY[i] = vZ[i] = i;
  }
  
  // density, pressure (cell centered)
  r.resize(nCells);
  p.resize(nCells);

  for (int i=0; i<nCells; ++i){
    r[i] = p[i] = i;
  }
}


//-----------------------------------------------------------------------------
//
//  CreateExample2
//

void CreateTest3(TempFile & file)
{
  fillData();

  cerr << "Now we try to create a new CGNS database.\n";
  
  CGNS::File tempFile(file.mName, CGNS::File::WRITE);

  cerr << "CGNS Open of temporary file (" << file.mName
       << ") seems to have been successfull.\n";
  
  cerr << "File version " << tempFile.getFileVersion() << endl;

  CGNS::Base base = tempFile.writeBase("ExampleBase#2", cellDim, physicalDim);
  
  base.writeDescriptor("README", "Single-Zone unstructured grid example");

  cerr << "base has been written\n";

  CGNS::Zone zone = base.writeUnstructuredZone("ExampleZone#2",
					       nVertices, nCells);

  cerr << "zone has been written\n";

  CGNS::GridCoordinates grid = zone.writeGridCoordinates();

  cerr << "grid has been written\n";

  grid.writeData(CGNS::GridCoordinates::COORDINATE_X, &cX[0]);
  grid.writeData(CGNS::GridCoordinates::COORDINATE_Y, &cY[0]);
  grid.writeData(CGNS::GridCoordinates::COORDINATE_Z, &cZ[0]);

  cerr << "coordinates have been written\n";

  CGNS::Elements elements = zone.writeElements("Elements", CGNS::Elements::HEXA_8);

  CGNS::IndexRange1D elementRange;
  elementRange.mMinI = 1;
  elementRange.mMaxI = nCells;
 
  elements.writeElementRange(elementRange);
  

  /*
  CGNS::FlowSolution solVertex = zone.writeFlowSolution("SolutionVertex", CGNS::VERTEX);
  CGNS::FlowSolution solCell   = zone.writeFlowSolution("SolutionCellCenter", CGNS::CELL_CENTER);
  
  cerr << "solutions have been created\n";

  solVertex.writeData(CGNS::FlowSolution::VELOCITY_X, &vX[0]);
  solVertex.writeData(CGNS::FlowSolution::VELOCITY_Y, &vY[0]);
  solVertex.writeData(CGNS::FlowSolution::VELOCITY_Z, &vZ[0]);

  solCell.writeData(CGNS::FlowSolution::DENSITY, &r[0]);
  solCell.writeData(CGNS::FlowSolution::PRESSURE, &p[0]);
  
  cerr << "fields have been written\n";
  */

  cerr << "########### Test3: Finished. Ok! ###########\n";
}

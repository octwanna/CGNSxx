/***************************************************************************
                          adfppTest.cpp  -  description
                             -------------------
    begin                : Mon Mär 12 15:39:20 CET 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "adf++.h"

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>

using std::ostream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::vector;

template<typename T>
inline bool isLast(T iter, T const & end) {
  return ++iter==end;
}


//-----------------------------------------------------------------------------
//
//  main
//

int main(int argc, char *argv[]) 
{
  cerr << "Test program for libADF++" << endl;

  cerr << "Running " << ADF::File::getLibraryVersionString() << '('
       << ADF::File::getLibraryVersionMajor() << ':'
       << ADF::File::getLibraryVersionMinor() << ")" << endl;

  try {

    {
      // --- simple open & close ---
      
      ADF::File adfFile; // <--- status == SCRATCH

      cerr << "Status of adf-file: " << ADF::File::asString(adfFile.getStatus()) << endl;
      cerr << "Format of adf-file: " << ADF::File::asString(adfFile.getFormat()) << endl;
    }

    {  
      // --- open, create/delete/rename nodes and close  ---
      
      ADF::File adfFile; // <--- status == SCRATCH

      ADF::Node rootNode = adfFile.getRoot();
      
      const int nChildren = 10;
      
      cerr << "Creating/deleting/labeling/renaming/'reparenting' nodes ... " << endl;
      
      for(int i=0; i<nChildren; i++){
	char aNumber[99]; sprintf(aNumber, "%d", i);
	string childName = "child_";
	childName += aNumber;

	cerr << "Creating...\n";
	rootNode.createChild(childName);
	cerr << "Deleting...\n";
	rootNode.deleteChild(childName);

	cerr << "Creating...\n";
	ADF::Node node = rootNode.createChild(childName);
	cerr << "Deleting...\n";
	rootNode.deleteChild(node);

	ADF::Node newNode = rootNode.createChild(childName);

	cerr << "setLabel...\n";
	newNode.setLabel("myLabel");

	cerr << "getLabel...\n";
	if(newNode.getLabel() != "myLabel"){
	  cerr << "OOOPS, wrong label!" << endl;
	  exit(-1);	  
	}

	string newChildName1 = "new1_";
	string newChildName2 = "new2_";
	string newChildName3 = "new3_";
	newChildName1 += aNumber;
	newChildName2 += aNumber;
	newChildName3 += aNumber;

//	cerr << "renameChild 1...\n";
	rootNode.renameChild(newNode, newChildName1);
//	cerr << "renameChild 2...\n";
	rootNode.renameChild(newNode, newChildName2);
//	cerr << "renameChild 3...\n";
	rootNode.renameChild(newChildName2, newChildName3);

//	cerr << "hasChild...\n";
	if(!rootNode.hasChild(newChildName3)){
	  cerr << "OOOPS, not a child!" << endl;
	  exit(-1);
	}

//	cerr << "getChild...\n";
	ADF::Node newParent = rootNode.getChild(newChildName3);

	if(!rootNode.hasChild(newParent)){
	  cerr << "OOOPS, not a child of the root node!" << endl;
	  exit(-1);
	}

	ADF::Node childToBeMoved = rootNode.createChild("toBeMoved");

//	cerr << "reparentChild...\n";
	rootNode.reparentChild(childToBeMoved, newParent);

	if(!newParent.hasChild(childToBeMoved)){
	  cerr << "OOOPS, not a child of the new parent!" << endl;
	  exit(-1);
	}
	
	if(rootNode.hasChild(childToBeMoved)){
	  cerr << "OOOPS, still a child of the root node!" << endl;
	  exit(-1);
	}

//	cerr << "reparentChild...\n";
	newParent.reparentChild(childToBeMoved, rootNode);

	if(newParent.hasChild(childToBeMoved)){
	  cerr << "OOOPS, still a child of the old parent!" << endl;
	  exit(-1);
	}
	
	if(!rootNode.hasChild(childToBeMoved)){
	  cerr << "OOOPS, not a child of the root node!" << endl;
	  exit(-1);
	}

	rootNode.deleteChild(childToBeMoved);
      }

      if(rootNode.getNumChildren() != nChildren){
	cerr << "OOOPS, wrong number of children!" << endl;	
	exit(-1);
      }

      rootNode.deleteChildren();

      cerr << "Creating/deleting/labeling/renaming/'reparenting' nodes ok. " << endl;
    }
    
    {
      // --- read/write data  ---
      
      ADF::File adfFile; // <--- status == SCRATCH

      ADF::Node rootNode = adfFile.getRoot();

      ADF::Node node_I4 = rootNode.createChild("node_I4");
      ADF::Node node_I8 = rootNode.createChild("node_I8");
      ADF::Node node_U4 = rootNode.createChild("node_U4");
      ADF::Node node_U8 = rootNode.createChild("node_U8");
      ADF::Node node_R4 = rootNode.createChild("node_R4");
      ADF::Node node_R8 = rootNode.createChild("node_R8");
      ADF::Node node_C1 = rootNode.createChild("node_C1");
      ADF::Node node_B1 = rootNode.createChild("node_B1");

      const int dimI = 27;
      const int dimJ = 13;
      const int dimK = 9;
      
      const int nDims = 3;
      const int dims[nDims] = { dimI, dimJ, dimK };

      const int size = dimI * dimJ * dimK;

      node_I4.setDataTypeDimensions(ADF::I4, nDims, dims);
      node_I8.setDataTypeDimensions(ADF::I8, nDims, dims);
      node_U4.setDataTypeDimensions(ADF::U4, nDims, dims);
      node_U8.setDataTypeDimensions(ADF::U8, nDims, dims);
      node_R4.setDataTypeDimensions(ADF::R4, nDims, dims);
      node_R8.setDataTypeDimensions(ADF::R8, nDims, dims);
      node_C1.setDataTypeDimensions(ADF::C1, nDims, dims);
      node_B1.setDataTypeDimensions(ADF::B1, nDims, dims);

      node_I4.setDataType(ADF::I8); node_I4.setDataType(ADF::I4);
      node_I8.setDataType(ADF::I4); node_I8.setDataType(ADF::I8);
      node_U4.setDataType(ADF::U8); node_U4.setDataType(ADF::U4);
      node_U8.setDataType(ADF::U4); node_U8.setDataType(ADF::U8);
      node_R4.setDataType(ADF::R8); node_R4.setDataType(ADF::R4);
      node_R8.setDataType(ADF::R4); node_R8.setDataType(ADF::R8);
      node_C1.setDataType(ADF::B1); node_C1.setDataType(ADF::C1);
      node_B1.setDataType(ADF::C1); node_B1.setDataType(ADF::B1);
      
      const int nDims2 = 6;
      const int dims2[nDims2] = { 1, 2, 3, 4, 5, 6 };

      node_I4.setDimensions(nDims2, dims2); node_I4.setDimensions(nDims, dims);
      node_I8.setDimensions(nDims2, dims2); node_I8.setDimensions(nDims, dims);
      node_U4.setDimensions(nDims2, dims2); node_U4.setDimensions(nDims, dims);
      node_U8.setDimensions(nDims2, dims2); node_U8.setDimensions(nDims, dims);
      node_R4.setDimensions(nDims2, dims2); node_R4.setDimensions(nDims, dims);
      node_R8.setDimensions(nDims2, dims2); node_R8.setDimensions(nDims, dims);
      node_C1.setDimensions(nDims2, dims2); node_C1.setDimensions(nDims, dims);
      node_B1.setDimensions(nDims2, dims2); node_B1.setDimensions(nDims, dims);

      int nDimsCheck = 0;
      int dimsCheck[nDims];
      
      nDimsCheck=node_I4.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_I4!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_I8.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_I8!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_U4.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_U4!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_U8.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_U8!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_R4.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_R4!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_R8.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_R8!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_C1.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_C1!" << endl;	
	exit(-1);	
      }

      nDimsCheck=node_B1.getDimensions(dimsCheck);

      if(nDimsCheck != 3 || 
	 dimsCheck[0] != dims[0] || 
	 dimsCheck[1] != dims[1] ||
	 dimsCheck[2] != dims[2]){
	cerr << "OOOPS, wrong dimensions for node_B1!" << endl;	
	exit(-1);	
      }

      vector<ADF::I4_t> vec_I4(size), vec_I4_check(size);
      vector<ADF::I8_t> vec_I8(size), vec_I8_check(size);
      vector<ADF::U4_t> vec_U4(size), vec_U4_check(size);
      vector<ADF::U8_t> vec_U8(size), vec_U8_check(size);
      vector<ADF::R4_t> vec_R4(size), vec_R4_check(size);
      vector<ADF::R8_t> vec_R8(size), vec_R8_check(size);
      vector<ADF::C1_t> vec_C1(size), vec_C1_check(size);
      vector<ADF::B1_t> vec_B1(size), vec_B1_check(size);
      
      const int cMaxChar = 128;
      
      for(int i=0; i<size; i++){
	vec_I4[i] = i;
	vec_I8[i] = i;
	vec_U4[i] = i;
	vec_U8[i] = i;
	vec_R4[i] = i;
	vec_R8[i] = i;
	vec_C1[i] = i % cMaxChar;
	vec_B1[i] = i % cMaxChar;
      }

      cerr << "Writing data ... " << endl;
    
      node_I4.writeData(&vec_I4[0]);
      node_I8.writeData(&vec_I8[0]);
      node_U4.writeData(&vec_U4[0]);
      node_U8.writeData(&vec_U8[0]);
      node_R4.writeData(&vec_R4[0]);
      node_R8.writeData(&vec_R8[0]);
      node_C1.writeData(&vec_C1[0]);
      node_B1.writeData(&vec_B1[0]);

      cerr << "Writing data ok. " << endl;

      cerr << "Reading data ... " << endl;
    
      node_I4.readData(&vec_I4_check[0]);
      node_I8.readData(&vec_I8_check[0]);
      node_U4.readData(&vec_U4_check[0]);
      node_U8.readData(&vec_U8_check[0]);
      node_R4.readData(&vec_R4_check[0]);
      node_R8.readData(&vec_R8_check[0]);
      node_C1.readData(&vec_C1_check[0]);
      node_B1.readData(&vec_B1_check[0]);

      for(int i=0; i<size; i++){
	if(vec_I4_check[i] != i) { cerr << "vec_I4[i] != i" << endl; exit(-1); }
	if(vec_I8_check[i] != i) { cerr << "vec_I8[i] != i" << endl; exit(-1); }
	if(vec_U4_check[i] != i) { cerr << "vec_U4[i] != i" << endl; exit(-1); }
	if(vec_U8_check[i] != i) { cerr << "vec_U8[i] != i" << endl; exit(-1); }
	if(vec_R4_check[i] != i) { cerr << "vec_R4[i] != i" << endl; exit(-1); }
	if(vec_R8_check[i] != i) { cerr << "vec_R8[i] != i" << endl; exit(-1); }
	if(vec_C1_check[i] != (i % cMaxChar)) { cerr << "vec_C1[i] != (i % cMaxChar)" << endl; exit(-1); }
	if(vec_B1_check[i] != (i % cMaxChar)) { cerr << "vec_B1[i] != (i % cMaxChar)" << endl; exit(-1); }
      }
      
      cerr << "Reading data ok. " << endl;
    }

    {
      // --- partial read/write data  ---
      
      ADF::File adfFile; // <--- status == SCRATCH

      ADF::Node rootNode = adfFile.getRoot();

      ADF::Node node_I4 = rootNode.createChild("node_I4");
      ADF::Node node_I8 = rootNode.createChild("node_I8");
      ADF::Node node_U4 = rootNode.createChild("node_U4");
      ADF::Node node_U8 = rootNode.createChild("node_U8");
      ADF::Node node_R4 = rootNode.createChild("node_R4");
      ADF::Node node_R8 = rootNode.createChild("node_R8");
      ADF::Node node_C1 = rootNode.createChild("node_C1");
      ADF::Node node_B1 = rootNode.createChild("node_B1");

      const int dimI = 130;
      const int dimJ = 110;
      const int dimK = 90;
      
      const int nDims = 3;
      const int dims[nDims] = { dimI, dimJ, dimK };

      const int size = dimI * dimJ * dimK;

      node_I4.setDataTypeDimensions(ADF::I4, nDims, dims);
      node_I8.setDataTypeDimensions(ADF::I8, nDims, dims);
      node_U4.setDataTypeDimensions(ADF::U4, nDims, dims);
      node_U8.setDataTypeDimensions(ADF::U8, nDims, dims);
      node_R4.setDataTypeDimensions(ADF::R4, nDims, dims);
      node_R8.setDataTypeDimensions(ADF::R8, nDims, dims);
      node_C1.setDataTypeDimensions(ADF::C1, nDims, dims);
      node_B1.setDataTypeDimensions(ADF::B1, nDims, dims);

      node_I4.setDataType(ADF::I8); node_I4.setDataType(ADF::I4);
      node_I8.setDataType(ADF::I4); node_I8.setDataType(ADF::I8);
      node_U4.setDataType(ADF::U8); node_U4.setDataType(ADF::U4);
      node_U8.setDataType(ADF::U4); node_U8.setDataType(ADF::U8);
      node_R4.setDataType(ADF::R8); node_R4.setDataType(ADF::R4);
      node_R8.setDataType(ADF::R4); node_R8.setDataType(ADF::R8);
      node_C1.setDataType(ADF::B1); node_C1.setDataType(ADF::C1);
      node_B1.setDataType(ADF::C1); node_B1.setDataType(ADF::B1);
      
      vector<ADF::I4_t> vec_I4(size), vec_I4_check(size);
      vector<ADF::I8_t> vec_I8(size), vec_I8_check(size);
      vector<ADF::U4_t> vec_U4(size), vec_U4_check(size);
      vector<ADF::U8_t> vec_U8(size), vec_U8_check(size);
      vector<ADF::R4_t> vec_R4(size), vec_R4_check(size);
      vector<ADF::R8_t> vec_R8(size), vec_R8_check(size);
      vector<ADF::C1_t> vec_C1(size), vec_C1_check(size);
      vector<ADF::B1_t> vec_B1(size), vec_B1_check(size);
      
      const int cMaxChar = 128;
      
      for(int i=0; i<size; i++){
	vec_I4[i] = i;
	vec_I8[i] = i;
	vec_U4[i] = i;
	vec_U8[i] = i;
	vec_R4[i] = i;
	vec_R8[i] = i;
	vec_C1[i] = i % cMaxChar;
	vec_B1[i] = i % cMaxChar;
      }
      
      cerr << "Writing data partially ... " << endl;

      const int nBlocks = 10;

      int length = size / (nBlocks-1);
      
      int start = 0;             // C/C++ convention: start at 0 !!!
      int end   = start+length;  // C/C++ convention: one past the end !!!

      for(int b=0; b<nBlocks; b++){
	
      ADF::Range range(start, end);
	node_I4.writeData(range, &vec_I4[start]); // <--- C convention: start at 0 !!!
	node_I8.writeData(range, &vec_I8[start]);
	node_U4.writeData(range, &vec_U4[start]);
	node_U8.writeData(range, &vec_U8[start]);
	node_R4.writeData(range, &vec_R4[start]);
	node_R8.writeData(range, &vec_R8[start]);
	node_C1.writeData(range, &vec_C1[start]);
	node_B1.writeData(range, &vec_B1[start]);

	start += length;
	end   += length;
	
	if (end > size){
	  end = size;
	  if (start >= end) break;
	}
      }
      
      cerr << "Writing data partially ok. " << endl;

      cerr << "Reading data partially ... " << endl;
    
      start = 0;       // C/C++ convention: start at 0 !!!
      end   = start+length;

      for(int b=0; b<nBlocks; b++){

      ADF::Range range(start, end);
	node_I4.readData(range, &vec_I4_check[start]);
	node_I8.readData(range, &vec_I8_check[start]);
	node_U4.readData(range, &vec_U4_check[start]);
	node_U8.readData(range, &vec_U8_check[start]);
	node_R4.readData(range, &vec_R4_check[start]);
	node_R8.readData(range, &vec_R8_check[start]);
	node_C1.readData(range, &vec_C1_check[start]);
	node_B1.readData(range, &vec_B1_check[start]);

	start += length;
	end   += length;
	
	if(end > size){
	  end = size;
	  if (start >= end) break;
	}
      }
      
      for(int i=0; i<size; i++){
	if(vec_I4_check[i] != i) { cerr << "vec_I4[i] != i" << endl; exit(-1); }
	if(vec_I8_check[i] != i) { cerr << "vec_I8[i] != i" << endl; exit(-1); }
	if(vec_U4_check[i] != i) { cerr << "vec_U4[i] != i" << endl; exit(-1); }
	if(vec_U8_check[i] != i) { cerr << "vec_U8[i] != i" << endl; exit(-1); }
	if(vec_R4_check[i] != i) { cerr << "vec_R4[i] != i" << endl; exit(-1); }
	if(vec_R8_check[i] != i) { cerr << "vec_R8[i] != i" << endl; exit(-1); }
	if(vec_C1_check[i] != (i % cMaxChar)) { cerr << "vec_C1[i] != (i % cMaxChar)" << endl; exit(-1); }
	if(vec_B1_check[i] != (i % cMaxChar)) { cerr << "vec_B1[i] != (i % cMaxChar)" << endl; exit(-1); }
      }
      
      cerr << "Reading data partially ok. " << endl;
    }

    /*{  
      // --- check links  ---
      
      ADF::File adfFile; // <--- status == SCRATCH
      
      ADF::Node rootNode = adfFile.getRoot();
      
      ADF::Node child1 = rootNode.createChild("child1");
      ADF::Node child2 = rootNode.createChild("child2");
      ADF::Node child3 = rootNode.createChild("child3");
      ADF::Node child4 = rootNode.createChild("child4");
      ADF::Node child5 = rootNode.createChild("child5");
      
      cerr << "Creating links ... " << endl;
      
      // ... to do ...
      
      cerr << " ... checking links: to do! " << endl;
      
      cerr << "Creating links ok. " << endl;
    }*/

  }
  catch (ADF::Exception const & e) {
    cerr << "OOOPS, ADF::Exception catched." << endl
	 << "\tError Code:    " << e.getErrorCode() << endl
	 << "\tError Message: " << e.getErrorMessage() << endl;
  }
  catch (std::exception const & e) {
    cerr << "OOOPS, std::exception catched." << endl
	 << "\tWhat: " << e.what() << endl;
  }
  catch (char const * e) {
    cerr << "OOOPS, char * catched." << endl
	 << "\tContent: " << e << endl;
  }
  catch (...) {
    cerr << "OOOPS, unknown exception catched." << endl;
  }
	
  return EXIT_SUCCESS;
}

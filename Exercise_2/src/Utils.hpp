#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh);


bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);


bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);


bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);


//the lenghts of the edges of the mesh need to be non zero
bool CheckNonZeroEdgeLengths(PolygonalMesh& mesh);


//the area of polygon of the mesh need to be non zero
bool CheckNonZeroPolygonArea(PolygonalMesh& mesh);

}

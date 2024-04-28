#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace PolygonalLibrary {

bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); // rimuovo prima riga (intestazione)

    mesh.NumberCell0D = listLines.size(); //ogni riga rappresenta una cella

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> marker >> coord(0) >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {

            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;

}

bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >> vertices(0) >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}

bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }
    file.close();

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DNumVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DNumEdges.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int num_vertices;
        unsigned int num_edges;

        converter >>  id >> marker >> num_vertices;

        vector<unsigned int> vertices(num_vertices);

        for(unsigned int i = 0; i < num_vertices; i++)
            converter >> vertices[i];

        converter >> num_edges;
        vector<unsigned int> edges(num_edges);

        for(unsigned int i = 0; i < num_edges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DNumVertices.push_back(num_vertices);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DNumEdges.push_back(num_edges);
        mesh.Cell2DEdges.push_back(edges);
    }

    return true;
}

bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell2D marker:" << endl;
        for(int i = 0; i < int(mesh.Cell2DId.size()); ++i)
        {
            cout << "Id: " << mesh.Cell2DId[i] << "   values: ";
            for(int j = 0; j < int(mesh.Cell2DNumEdges[i]); ++j)
                cout << mesh.Cell2DEdges[i][j] << "   ";

            cout << endl;
        }
    }
    cout << "From the output you can see that all the markers have been stored correctly" << endl;
    return true;
}


bool CheckNonZeroEdgeLengths(PolygonalMesh& mesh)
{
    for (const auto& cell1D : mesh.Cell1DVertices) {
        Vector2d origin = mesh.Cell0DCoordinates[cell1D(0)];
        Vector2d end = mesh.Cell0DCoordinates[cell1D(1)];
        Vector2d edge = end - origin;
        if (edge.norm() <= 2*numeric_limits<double>::epsilon()) {
            return false; // Se la norma del lato è 0, restituisci false
        }
    }
    return true; // Se tutte le lunghezze dei lati sono non nulle, restituisci true
}


bool CheckNonZeroPolygonArea(PolygonalMesh& mesh)
{
    double FinalArea = 0.0;
    for (const auto& cell2D : mesh.Cell2DVertices) {
        if (cell2D.size() < 3) {
            return false; // if the polygon have less than three vertices i don't consider it
        }

        unsigned int n = cell2D.size();
        double area = 0.0;
        list<Vector2d> listVectors;
        for (unsigned int i = 0; i < n; ++i) {
            listVectors.push_back(mesh.Cell0DCoordinates[cell2D[i]]);
        }
        auto it = listVectors.begin();
        auto jt = next(it); // Ottiene un iteratore al secondo elemento

        for (unsigned int i = 0; i < n; ++i, ++it, ++jt) {
            if (jt == listVectors.end())
                jt = listVectors.begin();

            area += (it->x() * jt->y() - jt->x() * it->y());    //calcolus for the area with gauss formula
        }

        double TotalArea = abs(area)*0.5; // L'area deve essere sempre positiva
        FinalArea += TotalArea;
        if (TotalArea <= 1e-6) {
            return false;
        }
    }
    cout << "Total area of the mesh: (need to be equal to 1 for the correct result) : " << FinalArea << endl;
    return true;    // if Polygon areas are not zero return true
}

}

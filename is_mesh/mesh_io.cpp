//
//  Deformabel Simplicial Complex (DSC) method
//  Copyright (C) 2013  Technical University of Denmark
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  See licence.txt for a copy of the GNU General Public License.

#include "mesh_io.h"
#include "is_mesh.h"
#include <fstream>

namespace is_mesh {

    namespace {
        // http://stackoverflow.com/a/236803/420250
        std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                elems.push_back(item);
            }
            return elems;
        }

        // http://stackoverflow.com/a/236803/420250
        std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, elems);
            return elems;
        }
    }

    
    void scale(std::vector<vec3>& points, double size)
    {
        vec3 p_min(INFINITY), p_max(-INFINITY);
        for (vec3 p : points) {
            for (int i = 0; i < 3; i++) {
                p_min[i] = Util::min(p[i], p_min[i]);
                p_max[i] = Util::max(p[i], p_max[i]);
            }
        }
        
        vec3 center = 0.5*(p_max + p_min);
        double scale = -INFINITY;
        for (int i = 0; i < 3; i++) {
            scale = Util::max(p_max[i] - p_min[i], scale);
        }
        
        for (vec3& p : points) {
            p = size*(p - center)/scale;
        }
    }
    
    void import_tet_mesh(const std::string & filename, std::vector<vec3>& points, std::vector<int>&  tets, std::vector<int>& tet_labels)
    {
        std::ifstream file(filename.data());
        if (!file.is_open()){
            std::cerr << "Could not open file "<<filename<<std::endl;
            throw;
        }
        while (!file.eof())
        {
            char c;
            file >> c;
            if (c == 'v')
            {
                double x,y,z; // The (x,y,z) coordinates of a vertex.
                file >> x;
                file >> y;
                file >> z;
                points.push_back(vec3(x,y,z));
            }
            else if (c == 't')
            {
                int v1, v2, v3, v4; // The indeces of the four vertices of a tetrahedron.
                int label; // The label of a tetrahedron.
                file >> v1;
                file >> v2;
                file >> v3;
                file >> v4;
                file >> label;
                
                tets.push_back(v1);
                tets.push_back(v2);
                tets.push_back(v3);
                tets.push_back(v4);
                
                tet_labels.push_back(label);
            }
            c = '\n';
        }
        file.close();
//        scale(points, 3.);
    }

    void import_surface_mesh(const std::string& filename, std::vector<vec3>& points, std::vector<int>& triangleIndices)
    {
        std::ifstream file(filename.data());
        if (!file.is_open()){
            std::cerr << "Could not open file "<<filename<<std::endl;
            throw;
        }
        if(file)
        {
            while(!file.eof())
            {
                std::string tok;
                file >> tok;
                if(tok == "v")
                {
                    double x,y,z;
                    file >> x;
                    file >> y;
                    file >> z;
                    points.push_back(vec3(x,y,z));
                    char line[1000];
                    file.getline(line, 998);
                }
                else if(tok == "f")
                {
                    char line[1000];
                    file.getline(line, 998);
                    auto tokens = split(line, ' ');
                    // find all polygon indices
                    std::vector<int> lineIndices;
                    for (auto t : tokens){
                        if (t.length()>0){
                            auto indices = split(t, '/'); // obj index may be V or V/U/N, where U and N is optional. We only need V = vertex index
                            lineIndices.push_back(atoi(indices[0].c_str())-1);
                        }
                    }
                    assert(lineIndices.size()>=3);
                    // convert polygon to triangles
                    for (int i=2;i<lineIndices.size();i++){
                        triangleIndices.push_back(lineIndices[0]);
                        triangleIndices.push_back(lineIndices[i-1]);
                        triangleIndices.push_back(lineIndices[i]);
                    }
                }
                else
                {
                    char line[1000];
                    file.getline(line, 998);
                }
            }
            file.close();
        }
//        scale(points, 2.);
    }
    
    void import_voxel_grid(const std::string& filename, vec3& origin, vec3& voxel_size, int& Ni, int& Nj, int& Nk, std::vector<int>& voxels)
    {
        std::ifstream file(filename.data());
        if (!file.is_open()){
            std::cerr << "Could not open file "<<filename<<std::endl;
            throw;
        }
        if(file)
        {
            while(!file.eof())
            {
                std::string tok;
                file >> tok;
                if(tok == "n")
                {
                    file >> Ni;
                    file >> Nj;
                    file >> Nk;
                }
                else if(tok == "s")
                {
                    double x, y, z;
                    file >> x;
                    file >> y;
                    file >> z;
                    voxel_size = vec3(x,y,z);
                }
                else if(tok == "o")
                {
                    double x, y, z;
                    file >> x;
                    file >> y;
                    file >> z;
                    origin = vec3(x,y,z);
                }
                else {
                    int l = atoi(tok.c_str());
                    int n;
                    file >> n;
                    for (unsigned int i = 0; i < n; i++) {
                        voxels.push_back(l);
                    }
                }
            }
        }
    }

    std::shared_ptr<Geometry> load_geometry(std::ifstream& file)
    {
        std::string tok;
        file >> tok;
        if(tok == "cube")
        {
            double x, y, z;
            file >> x;
            file >> y;
            file >> z;
            vec3 center(x,y,z);
            file >> x;
            file >> y;
            file >> z;
            vec3 size(x,y,z);
            return std::make_shared<Cube>(center, size);
        }
        else if(tok == "circle")
        {
            double x, y, z;
            file >> x;
            file >> y;
            file >> z;
            vec3 center(x,y,z);
            double radius;
            file >> radius;
            file >> x;
            file >> y;
            file >> z;
            vec3 normal(x,y,z);
            return std::make_shared<Circle>(center, radius, normal);
        }
        else if(tok == "cylinder")
        {
            double x, y, z;
            file >> x;
            file >> y;
            file >> z;
            vec3 center(x,y,z);
            double radius;
            file >> radius;
            file >> x;
            file >> y;
            file >> z;
            vec3 up(x,y,z);
            return std::make_shared<Cylinder>(center, radius, up);
        }
        else if(tok == "sphere")
        {
            double x, y, z;
            file >> x;
            file >> y;
            file >> z;
            vec3 center(x,y,z);
            double radius;
            file >> radius;
            bool inverted;
            file >> inverted;
            auto res = std::make_shared<Sphere>(center, radius);
            if (inverted){
                res->invert();
            }
            return res;
        }
        else if(tok == "plane")
        {
            double x, y, z;
            file >> x;
            file >> y;
            file >> z;
            vec3 point(x,y,z);
            file >> x;
            file >> y;
            file >> z;
            vec3 normal(x,y,z);
            return std::make_shared<Plane>(point, normal);
        }
        else if(tok == "square")
        {
            double x, y, z;
            file >> x;
            file >> y;
            file >> z;
            vec3 center(x,y,z);
            file >> x;
            file >> y;
            file >> z;
            vec3 width(x,y,z);
            file >> x;
            file >> y;
            file >> z;
            vec3 height(x,y,z);
            return std::make_shared<Square>(center, width, height);
        } else {
            std::cout << "Warning unknown token "<<tok<<std::endl;
        }
        return std::make_shared<Geometry>();
    }
    
    void import_geometry(const std::string& filename, vec3& origin, vec3& size, double& discretization, std::vector<unsigned int>& labels, std::vector<std::shared_ptr<Geometry>>& geometries)
    {
        std::ifstream file(filename.data());
        if (!file.is_open()){
            std::cerr << "Could not open file "<<filename<<std::endl;
            throw;
        }
        if(file)
        {
            while(!file.eof())
            {
                std::string tok;
                file >> tok;
                if(tok == "discretization")
                {
                    file >> discretization;
                }
                else if(tok == "origin")
                {
                    double x, y, z;
                    file >> x;
                    file >> y;
                    file >> z;
                    origin = vec3(x,y,z);
                }
                else if(tok == "size")
                {
                    double x, y, z;
                    file >> x;
                    file >> y;
                    file >> z;
                    size = vec3(x,y,z);
                }
                else if(tok == "label")
                {
                    int l;
                    file >> l;
                    labels.push_back(l);
                    geometries.push_back(load_geometry(file));
                }
            }
        }
    }
    
    void export_tet_mesh(const std::string& filename, std::vector<vec3>& points, std::vector<int>& tets, std::vector<int>& tet_labels)
    {
//        scale(points, 3.);
        std::ofstream file(filename.data());
        if (!file.is_open()){
            std::cerr << "Cannot open file for export_surface_mesh "<<filename<<std::endl;
        }
        
        for (auto &p : points)
        {
            file << "v " << p[0] << " " << p[1] << " " << p[2] << std::endl;
        }
        
        for (unsigned int i = 0; i < tet_labels.size(); i++)
        {
            file << "t ";
            file << tets[4*i] << " " << tets[4*i+1] << " " << tets[4*i+2] << " " << tets[4*i+3] << " ";
            file << tet_labels[i] << std::endl;
        }
    }
    
    void export_surface_mesh(const std::string& filename, std::vector<vec3>& points, std::vector<int>& faces)
    {
//        scale(points, 2.);
        std::ofstream obj_file;
        obj_file.open(filename.data());
        if (!obj_file.is_open()){
            std::cerr << "Cannot open file for export_surface_mesh "<<filename<<std::endl;
        }
        
        for (unsigned int i = 0; i < points.size(); ++i)
        {
            obj_file << "v "  <<   points[i][0] << " " <<   points[i][1] << " " <<   points[i][2] << std::endl;
        }
        
        for (unsigned int i = 0; i < faces.size(); ++i)
        {
            if (i%3 == 0)
            {
                obj_file << "f ";
            }
            obj_file << faces[i];
            if (i%3 == 2)
            {
                obj_file << std::endl;
            }
            else {
                obj_file << " ";
            }
        }
        
        obj_file.close();
    }

    void export_tet_mesh(const std::string &filename, ISMesh &mesh) {
        using namespace std;
        vector<vec3> points;
        vector<int> tets;
        vector<int> tet_labels;
        mesh.extract_tet_mesh(points, tets, tet_labels);
        export_tet_mesh(filename, points, tets, tet_labels);
    }

    void export_surface_mesh(const std::string &filename, ISMesh &mesh) {
        using namespace std;
        vector<vec3> points;
        vector<int> faces;
        mesh.extract_surface_mesh(points, faces);
        export_surface_mesh(filename, points, faces);
    }

    void export_surface_mesh_debug(const std::string &filename, ISMesh &mesh) {
        using namespace std;
        vector<vec3> points;
        vector<int> faces;
        mesh.extract_surface_mesh_debug(points, faces);
        export_surface_mesh(filename, points, faces);
    }
}

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

#include "tetralizer.h"
#include "tetgen.h"

static char* tetgen_flags = "pq1.5Y";

namespace {
    double lerp(
            double y1,double y2,
            double mu)
    {
        return(y1*(1-mu)+y2*mu);
    }
}

int get_index(int i, int j, int k, int Ni, int Nj, int Nk)
{
    return i + j*Ni + k*Ni*Nj;
}

void Tetralizer::tetralize_cube1(int i, int j, int k, int Ni, int Nj, int Nk, std::vector<int>& tets)
{
    // First tetrahedron:
    tets.push_back(get_index(i, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k+1, Ni, Nj, Nk));
    
    // Second tetrahedron:
    tets.push_back(get_index(i, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k, Ni, Nj, Nk));
    
    // Third tetrahedron:
    tets.push_back(get_index(i, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k+1, Ni, Nj, Nk));
    
    // Fourth tetrahedron:
    tets.push_back(get_index(i+1, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k, Ni, Nj, Nk));
    
    // Fifth tetrahedron:
    tets.push_back(get_index(i, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k+1, Ni, Nj, Nk));
}

void Tetralizer::tetralize_cube2(int i, int j, int k, int Ni, int Nj, int Nk, std::vector<int>& tets)
{
    // First tetrahedron:
    tets.push_back(get_index(i, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k, Ni, Nj, Nk));
    
    // Second tetrahedron:
    tets.push_back(get_index(i, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k, Ni, Nj, Nk));
    
    // Third tetrahedron:
    tets.push_back(get_index(i, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i, j, k+1, Ni, Nj, Nk));
    
    // Fourth tetrahedron:
    tets.push_back(get_index(i+1, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k+1, Ni, Nj, Nk));
    
    // Fifth tetrahedron:
    tets.push_back(get_index(i, j, k+1, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j, k, Ni, Nj, Nk));
    tets.push_back(get_index(i, j+1, k, Ni, Nj, Nk));
    tets.push_back(get_index(i+1, j+1, k+1, Ni, Nj, Nk));
}

void Tetralizer::create_tets(int Ni, int Nj, int Nk, std::vector<int>& tets)
{
    for (int k = 0; k < Nk; k++) {
        for (int j = 0; j < Nj; j++) {
            for (int i = 0; i < Ni; i++)
            {
                if((i + j + k)%2 == 0)
                {
                    tetralize_cube1(i, j, k, Ni+1, Nj+1, Nk+1, tets);
                }
                else {
                    tetralize_cube2(i, j, k, Ni+1, Nj+1, Nk+1, tets);
                }
            }
        }
    }
}

void Tetralizer::create_points(const vec3& origin, const vec3& voxel_size, int Ni, int Nj, int Nk, std::vector<vec3>& points)
{
    for (int k = 0; k < Nk+1; k++) {
        for (int j = 0; j < Nj+1; j++) {
            for (int i = 0; i < Ni+1; i++)
            {
                points.push_back(origin + vec3(i*voxel_size[0], j*voxel_size[1], k*voxel_size[2]));
            }
        }
    }
}

void Tetralizer::build_boundary_mesh(std::vector<double>& points_boundary, double avg_edge_length, std::vector<int>& faces_boundary, const vec3& min, const vec3& max)
{
    vec3 size = max - min;
    size_t slices = (size_t)std::ceil(std::max({size[0]/ avg_edge_length,size[1]/ avg_edge_length,size[2]/ avg_edge_length}));

    std::vector<std::vector<int> > face_xp_points(slices +1),
        face_xm_points(slices +1),
        face_yp_points(slices +1),
        face_ym_points(slices +1),
        face_zp_points(slices +1),
        face_zm_points(slices +1);
    
    for (int i = 0; i < slices +1; ++i)
    {
        face_xp_points[i].resize(slices +1);
        face_xm_points[i].resize(slices +1);
        face_yp_points[i].resize(slices +1);
        face_ym_points[i].resize(slices +1);
        face_zp_points[i].resize(slices +1);
        face_zm_points[i].resize(slices +1);
    }
    
    double x,y,z;
    int counter = 0;
    
    x = min[0];
    for (int iy = 0; iy < slices +1; ++iy)
    {
        for (int iz = 0; iz < slices +1; ++iz)
        {
            y = lerp(min[1], max[1], iy/(double)(slices));
            z = lerp(min[2], max[2], iz/(double)(slices));
            points_boundary.push_back(x);
            points_boundary.push_back(y);
            points_boundary.push_back(z);
            face_xm_points[iy][iz] = counter;
            if (iy == 0) face_ym_points[0][iz] = counter;
            if (iy == slices) face_yp_points[0][iz] = counter;
            if (iz == 0) face_zm_points[0][iy] = counter;
            if (iz == slices) face_zp_points[0][iy] = counter;
            counter++;
        }
    }
    
    x = max[0];
    for (int iy = 0; iy < slices +1; ++iy)
    {
        for (int iz = 0; iz < slices +1; ++iz)
        {
            y = lerp(min[1], max[1], iy/(double)(slices));
            z = lerp(min[2], max[2], iz/(double)(slices));
            points_boundary.push_back(x);
            points_boundary.push_back(y);
            points_boundary.push_back(z);
            face_xp_points[iy][iz] = counter;
            if (iy == 0) face_ym_points[slices][iz] = counter;
            if (iy == slices) face_yp_points[slices][iz] = counter;
            if (iz == 0) face_zm_points[slices][iy] = counter;
            if (iz == slices) face_zp_points[slices][iy] = counter;
            counter++;
        }
    }
    
    y = min[1];
    for (int ix = 1; ix < slices; ++ix)
    {
        for (int iz = 0; iz < slices +1; ++iz)
        {
            x = lerp(min[0], max[0], ix/(double)(slices));
            z = lerp(min[2], max[2], iz/(double)(slices));
            points_boundary.push_back(x);
            points_boundary.push_back(y);
            points_boundary.push_back(z);
            face_ym_points[ix][iz] = counter;
            if (iz == 0) face_zm_points[ix][0] = counter;
            if (iz == slices) face_zp_points[ix][0] = counter;
            counter++;
        }
    }
    
    y = max[1];
    for (int ix = 1; ix < slices; ++ix)
    {
        for (int iz = 0; iz < slices +1; ++iz)
        {
            x = lerp(min[0], max[0], ix/(double)(slices));
            z = lerp(min[2], max[2], iz/(double)(slices));
            points_boundary.push_back(x);
            points_boundary.push_back(y);
            points_boundary.push_back(z);
            face_yp_points[ix][iz] = counter;
            if (iz == 0) face_zm_points[ix][slices] = counter;
            if (iz == slices) face_zp_points[ix][slices] = counter;
            counter++;
        }
    }
    
    z = min[2];
    for (int ix = 1; ix < slices; ++ix)
    {
        for (int iy = 1; iy < slices; ++iy)
        {
            x = lerp(min[0], max[0], ix/(double)(slices));
            y = lerp(min[1], max[1], iy/(double)(slices));
            points_boundary.push_back(x);
            points_boundary.push_back(y);
            points_boundary.push_back(z);
            face_zm_points[ix][iy] = counter;
            counter++;
        }
    }
    
    z = max[2];
    for (int ix = 1; ix < slices; ++ix)
    {
        for (int iy = 1; iy < slices; ++iy)
        {
            x = lerp(min[0], max[0], ix/(double)(slices));
            y = lerp(min[1], max[1], iy/(double)(slices));
            points_boundary.push_back(x);
            points_boundary.push_back(y);
            points_boundary.push_back(z);
            face_zp_points[ix][iy] = counter;
            counter++;
        }
    }
    
    for (int i = 0; i < slices; ++i)
        for (int j = 0; j < slices; ++j)
        {
            faces_boundary.push_back(face_xm_points[i  ][j  ]);
            faces_boundary.push_back(face_xm_points[i  ][j+1]);
            faces_boundary.push_back(face_xm_points[i+1][j+1]);
            
            faces_boundary.push_back(face_xm_points[i  ][j  ]);
            faces_boundary.push_back(face_xm_points[i+1][j+1]);
            faces_boundary.push_back(face_xm_points[i+1][j  ]);
            
            faces_boundary.push_back(face_xp_points[i  ][j  ]);
            faces_boundary.push_back(face_xp_points[i  ][j+1]);
            faces_boundary.push_back(face_xp_points[i+1][j+1]);
            
            faces_boundary.push_back(face_xp_points[i  ][j  ]);
            faces_boundary.push_back(face_xp_points[i+1][j+1]);
            faces_boundary.push_back(face_xp_points[i+1][j  ]);
            
            faces_boundary.push_back(face_ym_points[i  ][j  ]);
            faces_boundary.push_back(face_ym_points[i  ][j+1]);
            faces_boundary.push_back(face_ym_points[i+1][j+1]);
            
            faces_boundary.push_back(face_ym_points[i  ][j  ]);
            faces_boundary.push_back(face_ym_points[i+1][j+1]);
            faces_boundary.push_back(face_ym_points[i+1][j  ]);
            
            faces_boundary.push_back(face_yp_points[i  ][j  ]);
            faces_boundary.push_back(face_yp_points[i  ][j+1]);
            faces_boundary.push_back(face_yp_points[i+1][j+1]);
            
            faces_boundary.push_back(face_yp_points[i  ][j  ]);
            faces_boundary.push_back(face_yp_points[i+1][j+1]);
            faces_boundary.push_back(face_yp_points[i+1][j  ]);
            
            faces_boundary.push_back(face_zm_points[i  ][j  ]);
            faces_boundary.push_back(face_zm_points[i  ][j+1]);
            faces_boundary.push_back(face_zm_points[i+1][j+1]);
            
            faces_boundary.push_back(face_zm_points[i  ][j  ]);
            faces_boundary.push_back(face_zm_points[i+1][j+1]);
            faces_boundary.push_back(face_zm_points[i+1][j  ]);
            
            faces_boundary.push_back(face_zp_points[i  ][j  ]);
            faces_boundary.push_back(face_zp_points[i  ][j+1]);
            faces_boundary.push_back(face_zp_points[i+1][j+1]);
            
            faces_boundary.push_back(face_zp_points[i  ][j  ]);
            faces_boundary.push_back(face_zp_points[i+1][j+1]);
            faces_boundary.push_back(face_zp_points[i+1][j  ]);
        }
}

void Tetralizer::tetrahedralize_inside(const std::vector<double>& points_interface, const std::vector<int>& faces_interface, std::vector<double>& points_inside, std::vector<int>& tets_inside)
{
    tetgenio in, out;
    
    in.firstnumber = 0;
    in.mesh_dim = 3;
    
    in.numberofpoints = (int)(points_interface.size()/3);
    in.pointlist = new double[points_interface.size()];
    
    for (unsigned int i = 0; i < points_interface.size(); ++i)
    {
        in.pointlist[i] = points_interface[i];
    }
    
    in.numberoffacets = (int)(faces_interface.size()/3);
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];
    
    for (int i = 0; i < in.numberoffacets; ++i)
    {
        tetgenio::facet* f = &in.facetlist[i];
        f->numberofpolygons = 1;
        f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
        f->numberofholes = 0;
        f->holelist = NULL;
        tetgenio::polygon* p = &(f->polygonlist[0]);
        p->numberofvertices = 3;
        p->vertexlist = new int[p->numberofvertices];
        for (int j = 0; j < p->numberofvertices; ++j)
            p->vertexlist[j] = faces_interface[3*i+j];
    }
    
    tetrahedralize(tetgen_flags, &in, &out);
    
    points_inside.resize(3 * out.numberofpoints);
    for (unsigned int i = 0; i < points_inside.size(); ++i)
    {
        points_inside[i] = out.pointlist[i];
    }
    
    tets_inside.resize(4 * out.numberoftetrahedra);
    for (unsigned int i = 0; i < tets_inside.size(); ++i)
    {
        tets_inside[i] = out.tetrahedronlist[i];
    }
}

void Tetralizer::tetrahedralize_outside(const std::vector<double>& points_interface, const std::vector<int>&  faces_interface, std::vector<double>& points_boundary, std::vector<int>&  faces_boundary, std::vector<double>& points_outside, std::vector<int>& tets_outside, const vec3& inside_pts)
{
    tetgenio in, out;
    
    in.firstnumber = 0;
    in.mesh_dim = 3;
    
    in.numberofpoints = (int)(points_interface.size()/3+points_boundary.size()/3);
    in.pointlist = new double[points_interface.size()+points_boundary.size()];
    for (unsigned int i = 0; i < points_interface.size(); ++i)
        in.pointlist[i] = points_interface[i];
    for (unsigned int i = points_interface.size(); i < points_interface.size()+points_boundary.size(); ++i)
        in.pointlist[i] = points_boundary[i-points_interface.size()];
    
    in.numberoffacets = static_cast<int>(faces_interface.size()/3+faces_boundary.size()/3);
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];
    
    for (int i = 0; i < in.numberoffacets; ++i)
    {
        tetgenio::facet* f = &in.facetlist[i];
        f->numberofpolygons = 1;
        f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
        f->numberofholes = 0;
        f->holelist = NULL;
        tetgenio::polygon* p = &(f->polygonlist[0]);
        p->numberofvertices = 3;
        p->vertexlist = new int[p->numberofvertices];
        for (int j = 0; j < p->numberofvertices; ++j)
        {
            if ((unsigned int)i < faces_interface.size()/3)
                p->vertexlist[j] = faces_interface[3*i+j];
            else
                p->vertexlist[j] = faces_boundary[3*i+j-faces_interface.size()]+points_interface.size()/3;
        }
    }
    
    in.numberofholes = 1;
    in.holelist = new double[3*in.numberofholes];
    in.holelist[0] = inside_pts[0];
    in.holelist[1] = inside_pts[1];
    in.holelist[2] = inside_pts[2];
    
    tetrahedralize(tetgen_flags, &in, &out);
    
    points_outside.resize(3*out.numberofpoints);
    for (unsigned int i = 0; i < points_outside.size(); ++i)
        points_outside[i] = out.pointlist[i];
    
    tets_outside.resize(4*out.numberoftetrahedra);
    for (unsigned int i = 0; i < tets_outside.size(); ++i)
    {
        tets_outside[i] = out.tetrahedronlist[i];
    }
}

void Tetralizer::merge_inside_outside(const std::vector<double>& points_interface, const std::vector<int>&  faces_interface, std::vector<double>& points_inside, std::vector<int>&  tets_inside, std::vector<double>& points_outside, std::vector<int>&  tets_outside, std::vector<double>& output_points, std::vector<int>&  output_tets, std::vector<int>&  output_tet_flags)
{
    int no_interface_points = static_cast<int>(points_interface.size()/3);
    int no_outside_points = static_cast<int>(points_outside.size()/3);
    
    output_points.resize(points_outside.size() + points_inside.size() - points_interface.size());
    output_tets.resize(tets_inside.size() + tets_outside.size());
    output_tet_flags.resize(output_tets.size()/4);
    
    unsigned int ip, it;
    for (ip = 0; ip < points_outside.size(); ++ip)
    {
        output_points.at(ip) = points_outside[ip];
    }
    int i = 0;
    for (; ip < output_points.size(); ++ip, ++i)
    {
        output_points.at(ip) = points_inside[points_interface.size() + i];
    }
    
    for (it = 0; it < tets_outside.size(); ++it)
    {
        output_tets[it] = tets_outside[it];
        if (it%4 == 0) output_tet_flags[it/4] = 0;
    }
    for (; it < output_tets.size(); ++it)
    {
        if (tets_inside[it-tets_outside.size()] < no_interface_points)
            output_tets.at(it) = tets_inside[it-tets_outside.size()];
        else
            output_tets.at(it) = tets_inside[it-tets_outside.size()]-no_interface_points+no_outside_points;
        if (it%4 == 0) output_tet_flags[it/4] = 1;
    }
}

void Tetralizer::tetralize(double padding, double avg_edge_length, const std::vector<vec3>& points_interface, const std::vector<int>& faces_interface, std::vector<vec3>& points, std::vector<int>& tets, std::vector<int>& tet_labels) {
    std::vector<double> points_interface_real;
    for (vec3 p : points_interface) {
        points_interface_real.push_back(p[0]);
        points_interface_real.push_back(p[1]);
        points_interface_real.push_back(p[2]);
    }

    vec3 interface_min = vec3{FLT_MAX};
    vec3 interface_max = vec3{-FLT_MAX};
    for (vec3 p : points_interface){
        for (int i=0;i<3;i++){
            interface_min[i] = std::min(interface_min[i], p[i]);
            interface_max[i] = std::max(interface_max[i], p[i]);
        }
    }
    interface_min -= vec3{padding,padding,padding};
    interface_max += vec3{padding,padding,padding};
    std::cout << "Creating boundary mesh "<<interface_min<<" to "<<interface_max<<std::endl;

    std::vector<double>    points_boundary;
    std::vector<int>  faces_boundary;
    build_boundary_mesh(points_boundary, avg_edge_length, faces_boundary, interface_min, interface_max);

    std::vector<double> points_inside;
    std::vector<int> tets_inside;
    tetrahedralize_inside(points_interface_real, faces_interface, points_inside, tets_inside);

    std::vector<double> points_outside;
    std::vector<int> tets_outside;
    tetrahedralize_outside(points_interface_real, faces_interface, points_boundary, faces_boundary, points_outside, tets_outside, vec3(points_inside[0], points_inside[1], points_inside[2]));

    std::vector<double> points_real;
    merge_inside_outside(points_interface_real, faces_interface, points_inside, tets_inside, points_outside, tets_outside, points_real, tets, tet_labels);

    points.resize(points_real.size()/3);
    for (unsigned int i = 0; i < points_real.size()/3; i++) {
        points[i] = vec3(points_real[i*3], points_real[i*3+1], points_real[i*3+2]);
    }
}

void Tetralizer::tetralize(const vec3& origin, const vec3& voxel_size, int Ni, int Nj, int Nk, const std::vector<int>& voxel_labels, std::vector<vec3>& points, std::vector<int>& tets, std::vector<int>& tet_labels) {
    create_points(origin, voxel_size, Ni, Nj, Nk, points);
    create_tets(Ni, Nj, Nk, tets);

    for (int l : voxel_labels) {
        for (int i = 0; i < 5; i++) {
            tet_labels.push_back(l);
        }
    }
}

void Tetralizer::tetralize(const vec3& origin, const vec3& size, double avg_edge_length, std::vector<unsigned int>& labels, std::vector<std::shared_ptr<is_mesh::Geometry>>& geometries, std::vector<vec3>& points, std::vector<int>& tets, std::vector<int>& tet_labels) {
    int Ni = (int)std::ceil(size[0]/avg_edge_length);
    int Nj = (int)std::ceil(size[1]/avg_edge_length);
    int Nk = (int)std::ceil(size[2]/avg_edge_length);

    create_points(origin, vec3(avg_edge_length), Ni, Nj, Nk, points);
    create_tets(Ni, Nj, Nk, tets);

    for (unsigned int i = 0; i < tets.size(); i += 4)
    {
        int label = 0;
        for (auto g = 0; g < labels.size(); g++) {
            vec3 bc = Util::barycenter(points[tets[i]], points[tets[i+1]], points[tets[i+2]], points[tets[i+3]]);
            if(geometries[g]->is_inside(bc))
            {
                label = labels[g];
            }
        }
        tet_labels.push_back(label);
    }
}
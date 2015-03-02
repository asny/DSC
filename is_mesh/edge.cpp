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

#include "edge.h"
#include "is_mesh.h"

namespace is_mesh
{
    Edge::Edge(ISMesh *owner) noexcept : Simplex<NodeKey, FaceKey>(owner) {

    }

    Edge::Edge(Edge&& other) noexcept
            :crossing(other.crossing),
            boundary(other.boundary),
            interface(other.interface),
            Simplex<NodeKey, FaceKey>(std::move(other))
    {
    }

    Edge &Edge::operator=(Edge&& other) noexcept {
        if (this != &other){
            crossing = other.crossing;
            boundary = other.boundary;
            interface = other.interface;
            ((Simplex<NodeKey, FaceKey>*)this)->operator=(std::move(other));
        }
        return *this;
    }

    const SimplexSet<NodeKey> &Edge::node_keys()  const noexcept {
        return get_boundary();
    }

    const SimplexSet<FaceKey> &Edge::face_keys()  const noexcept {
        return get_co_boundary();
    }

    bool Edge::is_crossing() noexcept {
        return crossing;
    }

    bool Edge::is_boundary() noexcept {
        return boundary;
    }

    bool Edge::is_interface() noexcept {
        return interface;
    }

    void Edge::set_crossing(bool b) {
        crossing = b;
    }

    void Edge::set_boundary(bool b) {
        boundary = b;
    }

    void Edge::set_interface(bool b) {
        interface = b;
    }

    double Edge::length() const {
        const SimplexSet<NodeKey> & nids = node_keys();
        return Util::length(m_mesh->get(nids[0]).get_pos() - m_mesh->get(nids[1]).get_pos());
    }

    double Edge::length_destination() const {
        const SimplexSet<NodeKey> & nids = node_keys();
        return Util::length(m_mesh->get(nids[0]).get_destination() - m_mesh->get(nids[1]).get_destination());
    }

    EdgeKey Edge::key() const noexcept {
        long index = ((char*)this - m_mesh->m_edge_kernel.data())/sizeof(util::kernel_element<EdgeKey, Edge>);
        assert(index >= 0);
        assert(index < m_mesh->m_edge_kernel.capacity());
        return EdgeKey((unsigned int) index);
    }

    vec3 Edge::get_center() const {
        const SimplexSet<NodeKey> & nids = node_keys();
        return (m_mesh->get(nids[0]).get_pos() + m_mesh->get(nids[1]).get_pos()) * 0.5;
    }
}
//
//  rotate_function.h
//  3D_DSC
//
//  Created by Asger Nyman Christiansen on 2/14/13.
//  Copyright (c) 2013 Asger Nyman Christiansen. All rights reserved.
//

#ifndef ___D_DSC__rotate_func__
#define ___D_DSC__rotate_func__

#include "velocity_function.h"


/**
 A rotating velocity function.
 */
template <class MT>
class RotateFunc: public VelocityFunc<MT>
{
    
    
public:
    /**
     Creates a rotating velocity function.
     */
    RotateFunc(DeformableSimplicialComplex<MT> *dsc_, double velocity, double accuracy, int max_time_steps = 500):
        VelocityFunc<MT>(dsc_, M_PI*velocity/180., accuracy, max_time_steps)
    {
        
    }
    
    /**
     Returns the name of the velocity function.
     */
    virtual std::string get_name() const
    {
        return std::string("ROTATE MOTION");
    }
    
    /**
     Computes the motion of each interface vertex and stores the new position in new_pos in the simplicial complex class.
     */
    virtual void deform()
    {
        typedef typename MT::vector3_type V;
        clock_t init_time = clock();
        
        V center = VelocityFunc<MT>::dsc->get_center();
        typename MT::matrix3x3_type mrot = MT::get_rotation_matrix(MT::axis_type::ZAXIS, VelocityFunc<MT>::VELOCITY);
        V p;
        V new_pos;
        for(auto vi = VelocityFunc<MT>::dsc->nodes_begin(); vi != VelocityFunc<MT>::dsc->nodes_end(); vi++)
        {
            if(vi->is_interface())
            {
                new_pos = center + mrot * (VelocityFunc<MT>::dsc->get_pos(vi.key()) - center);
                VelocityFunc<MT>::dsc->set_destination(vi.key(), new_pos);
            }
        }
        VelocityFunc<MT>::update_compute_time(clock() - init_time);
        init_time = clock();
        
        VelocityFunc<MT>::dsc->deform();
        
        VelocityFunc<MT>::update_deform_time(clock() - init_time);
    }
    
    /**
     Returns wether the motion has finished.
     */
    virtual bool is_motion_finished()
    {
        return false;
    }
    
    virtual void test()
    {
        std::vector<typename DeformableSimplicialComplex<MT>::edge_key_type> edges;
        for (auto eit = VelocityFunc<MT>::dsc->edges_begin(); eit != VelocityFunc<MT>::dsc->edges_end(); eit++)
        {
            if (eit->is_interface())
            {
                edges.push_back(eit.key());
            }
        }
        
        //        std::vector<typename SimplicialComplex<MT>::node_key_type> nodes;
        //        complex->get_nodes(edges[0], nodes);
        //        complex->collapse_edge(edges[0], nodes[0], nodes[1]);
        
        for (auto e : edges) {
            VelocityFunc<MT>::dsc->split_edge(e);
        }
    }
    
};

#endif /* defined(___D_DSC__rotate_func__) */

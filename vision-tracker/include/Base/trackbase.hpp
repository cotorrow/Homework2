#pragma once
#include<iostream>
#include "data_types.hpp"
#include<vector>

using rm::InputFrame;
using rm::Result;
using rm::TargetType;
using rm::TargetStatus;
using rm::TargetSnapshot;

class Trackbase

{
    private:

    int id;
    TargetStatus status;
    double x , y , predic_x , predic_y , angle;
    int lost_count = 0;
    std::vector<double> v={0,0};
    TargetType type;

    public:

    Trackbase();

    virtual ~Trackbase() = default;

    int get_id();
    
    TargetStatus get_status();

    TargetType get_type();
    
    std::vector<double> get_x_y();

    std::vector<double> get_v();
    
    std::vector<double> get_predic();
    
    double get_angle();
    
    int get_miscount();

    void write_x_y(double init_x , double init_y);
    
    void write_predic(double x1 , double y1);

    void write_angle(double init_angle);
    
    void write_miscount(int miscount);

    virtual void circlulate(std::int64_t t_ns , std::int64_t t_prev_ns , std::int64_t time3 , double x_n , double y_n , TargetStatus st , double angle_n);
    
    void write_status(TargetStatus s);

    void write_v(double x_v , double y_v);

    void write_id(int x);

    void write_type(TargetType x);

    virtual TargetSnapshot out();
};
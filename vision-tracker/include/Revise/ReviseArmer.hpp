#pragma once
#include<iostream>
#include "data_types.hpp"
#include<vector>
#include "Base/trackbase.hpp"

using rm::InputFrame;
using rm::Result;
using rm::TargetType;
using rm::TargetStatus;
class Revise:public Trackbase
{
    private:

    int count;
    double angle_v;
    double predic_angle;

    public:

    explicit Revise(TargetSnapshot d , int i);

    ~Revise() = default;

    void circlulate(std::int64_t t_ns , std::int64_t t_prev_ns , std::int64_t time3 , double x_n , double y_n , TargetStatus st , double angle_n);

    TargetSnapshot out();

    int check();
};

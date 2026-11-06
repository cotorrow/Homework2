#include<iostream>
#include "data_types.hpp"
#include<vector>
#include"Base/trackbase.hpp"

using rm::InputFrame;
using rm::Result;
using rm::TargetType;
using rm::TargetStatus;
using rm::TargetSnapshot;

Trackbase::Trackbase() = default;

int Trackbase::get_id()
    {
        return id;
    }    
TargetStatus Trackbase::get_status()
    {
        return status;
    }
TargetType Trackbase::get_type()
    {
        return type;
    }
std::vector<double> Trackbase::get_x_y()
    {
        return {x , y};
    }
std::vector<double> Trackbase::get_predic()
    {
        return {predic_x , predic_y};
    }
double Trackbase::get_angle()
    {
        return angle;
    }
int Trackbase::get_miscount()
    {
        return lost_count;
    }
std::vector<double> Trackbase::get_v()
    {
        return v;
    }



void Trackbase::write_type(TargetType x)
    {
        type = x;
    }
void Trackbase::write_id(int x)
    {
        id = x;
    }
void Trackbase::write_status(TargetStatus s)
    {
        status = s;
    }
void Trackbase::write_x_y(double init_x , double init_y)
    {
        x = init_x , y = init_y;
    }
void Trackbase::write_angle(double init_angle)
    {
        angle = init_angle;
    }
void Trackbase::write_miscount(int miscount)
    {
        lost_count = miscount;
    }
void Trackbase::write_v(double x_v , double y_v)
    {
        v[0] = x_v;
        v[1] = y_v;
    }
void Trackbase::write_predic(double a , double b)
    {
        predic_x = a , predic_y = b;
    }
void Trackbase::circlulate(std::int64_t t_ns , std::int64_t t_prev_ns , std::int64_t time3 , double x_n , double y_n , TargetStatus st , double angle_n)
    {
        double dt = (t_ns - t_prev_ns) / 1e9;
        int dt_obs = (t_ns - time3) / 1e9;
        if (st == TargetStatus::TRACK)
        {
            write_v((x_n - x) / dt_obs , (y_n - y) / dt_obs);
        }
        predic_x = x + v[0] * dt;
        predic_x = y + v[1] * dt;
        if (st == TargetStatus::TRACK && status == TargetStatus::LOST)
        {
            write_x_y(x_n ,  y_n);
            write_miscount(0);
            v = std::vector<double> (2);
            predic_x , predic_y = x , y;
            status = TargetStatus::TRACK;
        }
        else if (st == TargetStatus::TRACK)
        {
            write_x_y(x_n ,  y_n);
            write_miscount(0);
        }
        else
        {
            write_miscount(++lost_count);
        }
    }
TargetSnapshot Trackbase::out()
    {
        TargetSnapshot result;
        result.id = id;
        result.angle = angle;
        result.predicted_x = predic_x;
        result.predicted_y = predic_y;
        result.status = status;
        result.type = type;
        result.velocity_x = v[0];
        result.velocity_y = v[1];
        result.missed_count = lost_count;
        result.x = x;
        result.y = y;
        result.angular_velocity = 0;
        result.angle = 0;
        result.predicted_angle = 0;
        return result;
    }
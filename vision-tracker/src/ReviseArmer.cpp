#include "Base/trackbase.hpp"
#include "Revise/ReviseArmer.hpp"


Revise::Revise(TargetSnapshot d , int i)
    {
        write_id(d.id);
        write_type(d.type);
        write_status (d.status);
        write_x_y(d.x , d.y);
        write_predic(d.predicted_x , d.predicted_y);
        write_angle(d.angle);
        write_v(d.velocity_x , d.velocity_y);
        angle_v = d.angular_velocity;
        predic_angle = d.predicted_angle;
        write_miscount(d.missed_count);
        count = i;
    }

void Revise::circlulate(std::int64_t t_ns , std::int64_t t_prev_ns , std::int64_t time3 , double x_n , double y_n , TargetStatus st , double angle_n)
    {
        double x1 = get_x_y()[0] , y1 = get_x_y()[1];
        std::vector<double>v1 = get_v();
        double dt = (t_ns - t_prev_ns) / 1e9;
        double dt_obs = (t_ns - time3) / 1e9;
        if (get_miscount() < 5)
        {    if (st == TargetStatus::TRACK && get_status() == TargetStatus::TRACK)
            {
                write_v((x_n - x1) / dt_obs , (y_n - y1) / dt_obs);
                v1 = get_v();
                angle_v = (angle_n - get_angle()) / dt_obs;
                write_x_y(x_n , y_n);
                write_angle(angle_n);
                predic_angle = get_angle() + angle_v * dt;
                write_predic (x_n +  v1[0]* dt , y_n + v1[1] * dt);
                write_miscount(0);
            }
            if (st == TargetStatus::LOST && get_status() == TargetStatus::TRACK)
            {
                write_angle(get_angle() + angle_v * dt);
                predic_angle = get_angle() + angle_v * dt;
                double x2 = get_predic()[0] , y2 = get_predic()[1];
                write_predic (x2 + v1[0]* dt , y2 + v1[1] * dt);
                write_miscount(get_miscount() + 1);
                if(get_miscount() == 5)//全部置零
                {
                    write_x_y(0,0);
                    write_v(0,0);
                    write_angle(0);
                    angle_v = 0;
                    predic_angle = 0;
                    write_predic(0 , 0);
                    write_status(TargetStatus::LOST);
                    count++;
                }
            }
        }
        else
        {
            if (st == TargetStatus::TRACK)
            {
                write_miscount(0);
                write_status(TargetStatus::TRACK);
                write_x_y(x_n , y_n);
                write_angle(angle_n);
                write_predic(x_n , y_n);
                predic_angle = angle_n;
                count = 0;
            }
            else
            {
                count++;
            }
        }
    }
TargetSnapshot Revise::out()
    {
        std::vector<double>vv = get_v();
        TargetSnapshot result;
        result.id = get_id();
        result.angle = get_angle();
        result.predicted_x = get_predic()[0] , result.predicted_y = get_predic()[1];
        result.status = get_status();
        result.type = get_type();
        result.velocity_x = vv[0] , result.velocity_y = vv[1];
        result.missed_count = get_miscount();
        result.x =get_x_y()[0] , result.y = get_x_y()[1];
        result.angular_velocity = angle_v;
        result.angle = get_angle();
        result.predicted_angle = predic_angle;
        return result;
    }
int Revise::check()
    {
        return count;
    }

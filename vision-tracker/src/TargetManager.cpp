#include<iostream>
#include "data_types.hpp"
#include "test_frames.hpp"
#include "Normal/NormalArmer.hpp"
#include "Revise/ReviseArmer.hpp"
#include "TargetManager/TargetManager.hpp"

using rm::InputFrame;
using rm::Result;
using rm::TargetSnapshot;
using rm::TargetStatus;
using rm::TargetType;

void TargetManager::add_list(TargetSnapshot x , long double y)
    {
        list.push_back(x);
        list_time.push_back(y);
    }

TargetSnapshot TargetManager::exchange(TargetSnapshot y , std::int64_t t_ns , std::int64_t t_prev_ns , std::int64_t time3 , double x_n , double y_n , TargetStatus st , double angle_n , int i , int i_num)
    {
        if (y.type == TargetType::ARMOR)
        {
            Normal nor(y , i);
            nor.circlulate(t_ns , t_prev_ns , time3 , x_n , y_n , st , angle_n);
            list_count[i_num] = nor.check();
            return(nor.out());
        }
        else
        {
            Revise rev(y , i);
            rev.circlulate(t_ns , t_prev_ns , time3 , x_n , y_n , st , angle_n);
            list_count[i_num] = rev.check();
            return(rev.out());
        }
        
    }

TargetSnapshot TargetManager::create(rm::Observation x)
    {
        TargetSnapshot rs;
        rs.id = x.id;
        rs.angle = x.angle;
        rs.x = x.x;
        rs.y = x.y;
        rs.type = x.type;
        rs.angular_velocity = 0;
        rs.predicted_x = rs.x;
        rs.predicted_y = rs.y;
        rs.predicted_angle = rs.angle;
        rs.status = TargetStatus::TRACK;
        rs.velocity_x = 0;
        rs.velocity_y = 0;
        rs.angular_velocity = 0;
        rs.missed_count = 0;
        return rs;
    }

TargetSnapshot TargetManager::recreate(rm::Observation x)
{
    TargetSnapshot rs;
    rs.id = x.id;
    rs.angle = x.angle;
    rs.x = x.x;
    rs.y = x.y;
    rs.type = x.type;
    rs.angular_velocity = 0;
    rs.predicted_x = rs.x;
    rs.predicted_y = rs.y;
    rs.predicted_angle = rs.angle;
    rs.status = TargetStatus::TRACK;
    rs.velocity_x = 0;
    rs.velocity_y = 0;
    rs.angular_velocity = 0;
    rs.missed_count = 0;
    return rs;
}

int TargetManager::find(int i , std::vector<TargetSnapshot> list11)
    {
        for (int t = 0 ; t < list11.size() ; t ++)
        {
            if (list11[t].id == i)
            {
                return t;
            }
        }
        return -1;
    }

int TargetManager::find2(int i , std::vector<rm::Observation> list11)
    {
        for (int t = 0 ; t < list11.size() ; t ++)
        {
            if (list11[t].id == i)
            {
                return t;
            }
        }
        return -1;
    }

int TargetManager::count1()
    {
        int count1 = 0 , count2 = 0 , count3 = 0 , count4 = 0 ;
        for (auto i : list)
        {
            if (i.type == TargetType::ARMOR)
            {
                count1++;
            }
        }
        return count1 ;
    }

int TargetManager::count2()
    {
        int count1 = 0;
        for (auto i : list)
        {
            if (i.type == TargetType::SPINNING)
            {
                count1++;
            }
        }
        return count1 ;
    }

int TargetManager::count3()
    {
        int count1 = 0;
        for (auto i : list)
        {
            if (i.status == TargetStatus::TRACK)
            {
                count1++;
            }
        }
        return count1 ;
    }

int TargetManager::count4()
    {
        int count1 = 0;
        for (auto i : list)
        {
            if (i.status == TargetStatus::LOST)
            {
                count1++;
            }
        }
        return count1 ;
    }

Result TargetManager::onFrame(const InputFrame &frame)
    {
        if(frame.frame_id > max_id && frame.timestamp_ns > now_time)
        {
            std::vector<int> id1ist;
            for (auto i : frame.observations)
            {
                if( i.id < 0 )
                {
                    return Result{false, frame.frame_id, list , 0, 0, 0, 0};
                }
                for (auto j : id1ist)
                {
                    if(i.id == j)
                    {
                        return Result{false, frame.frame_id, list , 0, 0, 0, 0};
                    }
                    else
                    {
                        id1ist.push_back(i.id);
                    }
                }
                if (find(i.id , list) > -1)
                {
                    return Result{false, frame.frame_id, list , 0, 0, 0, 0};
                }

            }
            last_time = now_time;
            now_time = frame.timestamp_ns;
            last_id = now_id;
            now_id = frame.frame_id;
            max_id = now_id;
            for (auto i : frame.observations)
            {
                if (list.size() == 0 || find(i.id , list) == -1)
                {
                    list.push_back(create(i));
                    list_time.push_back(now_time);
                    list_count.push_back(0);
                }
                else
                {
                    if(list[find(i.id , list)].type == i.type)
                    {
                        list[find(i.id , list)] = exchange(list[find(i.id , list)] , now_time , last_time , list_time[find(i.id , list)] , i.x , i.y , TargetStatus::TRACK , i.angle , list_count[find(i.id , list)] , find(i.id , list));
                        list_time[find(i.id , list)] = now_time ;
                    }
                    else
                    {
                        list[find(i.id , list)] = recreate(i);
                        list_time[find(i.id , list)] = now_time;
                        list_count[find(i.id , list)] = 0;
                    }
                }
            }
            for (int i = 0 ; i < list.size() ; i ++)
            {
                if (find2(list[i].id , frame.observations) == -1)
                {
                    list[i] = exchange(list[i] , now_time , last_time , list_time[i] , 0 , 0 , TargetStatus::LOST , 0 , list_count[i] , i);
                    if (list_count[i] >= 10)
                    {
                        list.erase(list.begin() + i);
                        list_time.erase(list_time.begin() + i);
                        list_count.erase(list_count.begin() + i);
                    }
                }
            }
            int c1 = count1() , c2 = count2() , c3 = count3() , c4 = count4();

            return Result{true, frame.frame_id, list , c3, c4, c1, c2};
             

        }
        else
        {
            return Result{false, frame.frame_id, list , 0, 0, 0, 0};
        }
    }

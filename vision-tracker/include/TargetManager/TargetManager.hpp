#pragma once

#include<iostream>
#include "data_types.hpp"
#include "test_frames.hpp"
#include "Normal/NormalArmer.hpp"
#include "Revise/ReviseArmer.hpp"

using rm::InputFrame;
using rm::Result;
using rm::TargetSnapshot;
using rm::TargetStatus;
using rm::TargetType;

class TargetManager
{
    private:
    std::vector<TargetSnapshot> list;

    std::vector<std::int64_t> list_time;

    std::int64_t last_time = -1;

    std::int64_t now_time = -1;

    std::vector<int> list_count;


    int last_id = -1;
    
    int now_id = -1;

    int max_id = -1;

    public:
    void add_list(TargetSnapshot x , long double y);

    TargetSnapshot exchange(TargetSnapshot y , std::int64_t t_ns , std::int64_t t_prev_ns , std::int64_t time3 , double x_n , double y_n , TargetStatus st , double angle_n , int i , int i_num);

    TargetSnapshot create(rm::Observation x);

    int find(int i , std::vector<TargetSnapshot> list11);

    int find2(int i , std::vector<rm::Observation> list11);

    Result onFrame(const InputFrame &frame);

    int count1();

    int count2();

    int count3();

    int count4();

    TargetSnapshot recreate(rm::Observation x);
};
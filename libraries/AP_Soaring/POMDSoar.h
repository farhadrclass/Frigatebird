// Copyright (c) Microsoft Corporation. All rights reserved. 
// Licensed under the GPLv3 license

#pragma once

#include <APM_Control/APM_Control.h>
#include "PomdpSolver.h"
class SoaringController;


//
// POMDSoarAlgorithm, the POMDP/Bayesian RL-based logic used by SoaringController to decide on the course of action
//
class POMDSoarAlgorithm
{
    friend class SoaringController;

private:
    float _roll_cmds[MAX_ACTIONS];
    int _n_actions = 2;
    int _prev_n_actions = 2;
    bool _pomdp_active = false;
    Location _pomdp_wp;
    Location _prev_pomdp_wp;
    float _sign = 1;
    float _pomdp_radius;
    uint64_t _prev_pomdp_update_time = 0;
    int _prev_pomdp_action = 0;
    uint64_t _pomdp_solve_time;
    uint64_t _pomp_loop_time;
    uint64_t _pomp_loop_min_time;
    uint64_t _pomp_loop_max_time;
    uint64_t _pomp_loop_av_time = 0;
    float _pomdp_roll_cmd = 0;
    Vector2f _pomdp_vecNE;
    int _m = 0;
    int _j = 0;
    int _n_action_samples;
    bool _new_actions_to_send = false;
    int _pomdp_mode = 0; // 0 = explore, 1 = max lift
    float _weights[4] = { 1, 1, 1, 1 };
    uint8_t _prev_run_timing_test;

    float _debug_out[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t _debug_out_mode;

    const AP_AutoTune::ATGains &_gains;
    AP_Float &_scaling_speed;
    PomdpSolver _solver;
    const SoaringController *_sc;

    AP_Int8 pomdp_delta_mode;
    AP_Int16 pomdp_n;
    AP_Int16 pomdp_k;
    AP_Float pomdp_hori;
    AP_Float pomdp_roll1;
    AP_Float pomdp_roll2;
    AP_Float pomdp_step_t;
    AP_Float pomdp_loop_load;
    AP_Int8 pomdp_n_actions;
    AP_Float pomdp_roll_rate;
    AP_Float I_moment;
    AP_Float k_aileron;
    AP_Float k_roll_damping;
    AP_Float c_lp;
    AP_Int8 pomdp_norm_pth;
    AP_Int8 pomdp_extend;
    AP_Int8 pomdp_plan_mode;
    AP_Float pomdp_pth;

    void init_actions(bool mode);

public:
    POMDSoarAlgorithm(const SoaringController *sc, AP_RollController &rollController, AP_Float &scaling_speed);
    void init_thermalling();
    float assess_thermalability(uint8_t exit_mode);
    bool are_computations_in_progress();
    void stop_computations();
    void update_internal_state();
    void update_internal_state_test();
    bool is_set_to_continue_past_thermal_locking_period();
    int get_curr_mode();
    uint64_t get_latest_pomdp_solve_time();
    float get_action();
    void send_test_out_msg(mavlink_channel_t chan);
    bool update_thermalling(const Location &current_loc);
    void run_tests();
};

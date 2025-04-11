#ifndef TIMER_HPP
#define TIMER_HPP

#include <random>
#include <chrono>

/*
    the unit is milisec
*/
class Timer{
public:
    Timer() {ResetCheckTime();}
    ~Timer() = default;

    bool IsTimeout(){return std::chrono::steady_clock::now() > checkTime + changeTime;}

    void ResetCheckTime(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(min, max);

        changeTime = std::chrono::milliseconds(dist(gen));
        checkTime = std::chrono::steady_clock::now();
    }

    void SetChangeTime(int min = 1000, int max = 2000){
        this->min = min;
        this->max = max;
    }

private:
    std::chrono::steady_clock::time_point checkTime;  // 記錄開始時間
    std::chrono::milliseconds changeTime;
    int min = 1000, max = 2000;
};


#endif
#include "Time.h"

namespace ToolEngine
{
    Time::Time()
    {
        last_time = std::chrono::high_resolution_clock::now();
        delta_time = 0.0f;
    }
    Time::~Time()
    {

    }

    void Time::tick()
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        delta_time = std::chrono::duration<float, std::ratio<1, 1>>(current_time - last_time).count();
        last_time = current_time;
    }

    float Time::getCurrentTime() const
    {
        return std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}
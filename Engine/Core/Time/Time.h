#pragma once

#include "Marco.h"

namespace Engine
{
    class Time final
    {
    public:
        static Time& getInstance()
        {
            static Time instance;
            return instance;
        }

        void tick();
        float getDeltaTime() const { return delta_time; }
        float getCurrentTime() const;

    private:
        Time();
        ~Time();
        std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
        float delta_time;
    };
}
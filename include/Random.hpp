#pragma once

#include <random>

using std::random_device;
using std::mt19937;

class Random {
    public:
        Random();

        unsigned char rand(unsigned char min, unsigned char max);
        unsigned int rand(unsigned int min, unsigned int max);
        int rand(int min, int max);

    private:
        random_device mRandomDevice;
        mt19937 mEngine;
};

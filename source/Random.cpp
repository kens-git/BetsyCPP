#include "Random.hpp"

using std::uniform_int_distribution;

Random::Random() : mEngine(mRandomDevice()) { }

unsigned char Random::rand(unsigned char min, unsigned char max) {
    uniform_int_distribution<unsigned char> dist(min, max);
    return dist(mEngine);
}

unsigned int Random::rand(unsigned int min, unsigned int max) {
    uniform_int_distribution<unsigned int> dist(min, max);
    return dist(mEngine);
}

int Random::rand(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(mEngine);
}

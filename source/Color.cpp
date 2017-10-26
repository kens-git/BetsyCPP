#include "Color.hpp"

Color::Color() {
    mRed = 255;
    mGreen = 255;
    mBlue = 255;
    mAlpha = 255;
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue) {
    mRed = red;
    mGreen = green;
    mBlue = blue;
    mAlpha = 255;
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    mRed = red;
    mGreen = green;
    mBlue = blue;
    mAlpha = alpha;
}

unsigned char Color::getRed() const {
    return mRed;
}

void Color::setRed(unsigned char red) {
    mRed = red;
}

unsigned char Color::getGreen() const {
    return mGreen;
}

void Color::setGreen(unsigned char green) {
    mGreen = green;
}


unsigned char Color::getBlue() const {
    return mBlue;
}

void Color::setBlue(unsigned char blue) {
    mBlue = blue;
}

unsigned char Color::getAlpha() const {
    return mAlpha;
}

void Color::setAlpha(unsigned char alpha) {
    mAlpha = alpha;
}

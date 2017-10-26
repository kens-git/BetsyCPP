#pragma once

class Color {
    public:
        Color();
        Color(unsigned char red, unsigned char green, unsigned char blue);
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

        unsigned char getRed() const;
        void setRed(unsigned char red);

        unsigned char getGreen() const;
        void setGreen(unsigned char green);

        unsigned char getBlue() const;
        void setBlue(unsigned char blue);

        unsigned char getAlpha() const;
        void setAlpha(unsigned char alpha);
	
    private:
        unsigned char mRed;
        unsigned char mGreen;
        unsigned char mBlue;
        unsigned char mAlpha;
};

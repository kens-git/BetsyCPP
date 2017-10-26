#pragma once


// TODO: figure out why the compiler doesn't recognize the forward declared string
// as the included standard string in DrawManager.cpp
#include <string>
using std::string;

#include <thread>
#include <mutex>
using std::thread;
using std::mutex;

class QPainter;
class QImage;

class QHostAddress;

#include "Color.hpp"

class DrawManager {
    public:
        DrawManager(const Color& backgroundColor);
        ~DrawManager();

        static const unsigned char WIDTH = 162;
        static const unsigned char HEIGHT = 108;

        void finish(); // copies image data from mScreen to mFrame, essentially, ends drawing and writes a frame

        void setBackground(Color backgroundColor); // will be drawn first every frame
        Color& getBackground();

        void line(unsigned char width, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2,
                  const Color& color);

        void rect(unsigned char width, unsigned char height, unsigned char x, unsigned char y, const Color& fillColor);
        void rect(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                  unsigned char strokeWidth, const Color& strokeColor);
        void rect(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                  unsigned char strokeWidth, const Color& strokeColor, const Color& fillColor);

        void ellipse(unsigned char width, unsigned char height, unsigned char x, unsigned char y, const Color& fillColor);
        void ellipse(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                     unsigned char strokeWidth, const Color& strokeColor);
        void ellipse(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                     unsigned char strokeWidth, const Color& strokeColor, const Color& fillColor);

        void exportImage(const string& filename);

        void setDrawing(bool isDrawing);
        bool getDrawing();

    private:
        void drawFrame(); // called by mDrawThread, copies and transmits image data to Betsy

        bool mIsDrawing;

        Color mBackgroundColor;

        QPainter* mPainter;
        QImage* mScreen; // where the image data is drawn to
        QImage* mFrame; // copied from mScreen, then sent to Betsy

        QHostAddress* mBetsyIP;
        unsigned int mBetsyPort;

        thread mDrawThread;
        mutex mDrawMutex;

        // TODO: give DrawManager brushes and pens as members, to save instantiation costs every time a draw method is called
};

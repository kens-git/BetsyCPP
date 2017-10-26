#include "DrawManager.hpp"
#include "Color.hpp"

#include <chrono>

using std::chrono::milliseconds;

using std::lock_guard;
using std::this_thread::sleep_for;
//#include <string>
//using std::string;

#include <QString>
#include <QtGui/QImage>
#include <QtGui/QImageWriter>
#include <QtGui/QPainter>
#include <QtGui/QPen>

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>

DrawManager::DrawManager(const Color& backgroundColor)
        : mIsDrawing(true), mDrawThread(&DrawManager::drawFrame, this) {
    mScreen = new QImage(WIDTH, HEIGHT, QImage::Format_RGBA8888);
    mFrame = new QImage(WIDTH, HEIGHT, QImage::Format_RGBA8888);
    mPainter = new QPainter(mScreen);

    mBetsyIP = new QHostAddress("192.168.111.69");
    mBetsyPort = 65506;

    mBackgroundColor = backgroundColor;
    // The background is only drawn after a call to finish(), so this
    // fills the screen with the background color before anything else is drawn
    rect(WIDTH, HEIGHT, 0, 0, 0, mBackgroundColor);
}

DrawManager::~DrawManager() {
    mPainter->end();

    mIsDrawing = false;
    mDrawThread.join();

    delete mFrame;
    mFrame = nullptr;

    delete mScreen;
    mScreen = nullptr;

    delete mPainter;
    mPainter = nullptr;
}

void DrawManager::setBackground(Color backgroundColor) {
    mBackgroundColor = backgroundColor;
}

Color& DrawManager::getBackground() {
    return mBackgroundColor;
}

void DrawManager::finish() {
    lock_guard<mutex> lock(mDrawMutex);
    *mFrame = mScreen->copy(0, 0, WIDTH, HEIGHT);
    mScreen->fill(QColor(mBackgroundColor.getRed(), mBackgroundColor.getGreen(),
                          mBackgroundColor.getBlue(), mBackgroundColor.getAlpha()));
}

void DrawManager::line(unsigned char width, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2,
                       const Color &color) {
    QPen pen(QColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
    pen.setWidth(width);
    mPainter->setPen(pen);
    mPainter->drawLine(x1, y1, x2, y2);
}

void DrawManager::rect(unsigned char width, unsigned char height, unsigned char x, unsigned char y, const Color& fillColor) {
    QColor color(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
    mPainter->fillRect(x, y, width, height, color);
}

void DrawManager::rect(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                       unsigned char strokeWidth, const Color& strokeColor) {
    QPen pen(QBrush(Qt::NoBrush), strokeWidth);
    pen.setColor(QColor(strokeColor.getRed(), strokeColor.getGreen(), strokeColor.getBlue(), strokeColor.getAlpha()));
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setWidth(strokeWidth);
    mPainter->setPen(pen);
    mPainter->drawRect(x, y, width, height);
}

void DrawManager::rect(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                       unsigned char strokeWidth, const Color& strokeColor, const Color& fillColor) {
    QPen pen(QBrush(Qt::NoBrush), strokeWidth);
    pen.setColor(QColor(strokeColor.getRed(), strokeColor.getGreen(), strokeColor.getBlue(), strokeColor.getAlpha()));
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setWidth(strokeWidth);
    mPainter->setPen(pen);

    mPainter->setBrush(QBrush(QColor(fillColor.getRed(), fillColor.getGreen(),
                                     fillColor.getBlue(), fillColor.getAlpha())));

    // TODO: figure out how to offset x and y to account for the stroke width
    // x + (strokeWidth / 2) would work, but only if strokeWidth is even
    // if strokeWidth is odd: x + (strokeWidth +/- 1 / 2)

    mPainter->drawRect(x, y, width, height);
}

void DrawManager::ellipse(unsigned char width, unsigned char height, unsigned char x, unsigned char y, const Color& fillColor) {
    QColor color(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
    mPainter->setBrush(QBrush(color, Qt::SolidPattern));
    mPainter->setPen(Qt::NoPen);
    mPainter->drawEllipse(x, y, width, height);
}

void DrawManager::ellipse(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                          unsigned char strokeWidth, const Color& strokeColor) {
    QPen pen(QBrush(Qt::NoBrush), strokeWidth);
    pen.setColor(QColor(strokeColor.getRed(), strokeColor.getGreen(), strokeColor.getBlue(), strokeColor.getAlpha()));
    mPainter->setPen(pen);

    mPainter->setBrush(QBrush(Qt::NoBrush));

    mPainter->drawEllipse(x, y, width, height);
}

void DrawManager::ellipse(unsigned char width, unsigned char height, unsigned char x, unsigned char y,
                          unsigned char strokeWidth, const Color&  strokeColor, const Color& fillColor) {
    QPen pen(QBrush(Qt::NoBrush), strokeWidth);
    pen.setColor(QColor(strokeColor.getRed(), strokeColor.getGreen(), strokeColor.getBlue(), strokeColor.getAlpha()));
    mPainter->setPen(pen);

    mPainter->setBrush(QBrush(QColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(),
                                     fillColor.getAlpha())));

    mPainter->drawEllipse(x, y, width, height);
}

void DrawManager::exportImage(const string& filename) {
    QImageWriter exporter;
    QString name = QString::fromStdString(filename);
    exporter.setFileName(name);

    exporter.write(*mScreen);
}

void DrawManager::drawFrame() {
    while (mIsDrawing) {
        if (mFrame != nullptr) {
            // width * height * 3 bytes per pixl (RGB values), plus 6 bytes for payload header
            char packetData[WIDTH * HEIGHT * 3 + 6];

            packetData[0] = 0xC9; // packet start
            packetData[1] = 0xDA; // packet type (data)

            // packet size (# of bytes)
            packetData[2] = 0xCD; // high-byte
            packetData[3] = 0x08; // low-byte

            // Required by the protocol but not used by us
            packetData[4] = 0x00; // packet number
            packetData[5] = 0x00; // # of packets

            unsigned int index = 6;

            QUdpSocket sock;

            lock_guard<mutex> lock(mDrawMutex);
            QImage tempFrame = *mFrame;

            for (unsigned int i = 0; i < HEIGHT; i++) {
                for (unsigned int j = 0; j < WIDTH ; j++) {
                    packetData[index] = tempFrame.pixelColor(j, i).red();
                    index++;
                    packetData[index] = tempFrame.pixelColor(j, i).green();
                    index++;
                    packetData[index] = tempFrame.pixelColor(j, i).blue();
                    index++;
                }
            }

            sock.writeDatagram(packetData, sizeof(packetData), *mBetsyIP, mBetsyPort);
        } else {
            sleep_for(milliseconds(1000));
        }

        sleep_for(milliseconds(33)); // 30 frames per second, roughly
    }
}

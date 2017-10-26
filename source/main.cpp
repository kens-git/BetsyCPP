#include "DrawManager.hpp"
#include "Color.hpp"
#include "Random.hpp"

int main() {
    DrawManager draw(Color(0, 0, 0));
    Random rand;

    bool notDone = true;

    while (notDone) {
        // draw.drawStuff()...

        draw.finish(); // ends the frame
    }

    return 0;
}

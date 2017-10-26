Drawing library for a (formerly) outdoor LED screen. Meant to abstract away the specifics of the networking protocol and the underlying Qt drawing functions.
Uses the main thread for drawing and a second thread to send the UDP packets.

Currently not particularly performant; randomizing every pixel (162 * 108) every frame gets about 2-3 frames/second on a raspberry pi model B.
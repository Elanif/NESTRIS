# NESTRIS

## NESTRIS is a nes tetris clone that aims to be a perfect recreation in every competitive aspect, additionally it aims to lower the input delay to the lowest amount possible.

### Some of its characteristics:
- c++: I went for c++ since it's the language I know best and since it's known for its optimization.
- delays: Playing on an original console with a crt yields 1/2 frame of lag from the crt, 2 frames lag built in the nes, plus controller lag. Playing on a normal emulator yields 4 frames of lag, plug monitor lag and controller lag. Retroarch's run ahead option reduces lag to 2 frames.
- fps: One of the reasons why I chose c++ over a web application is that I couldn't get a broswer game to have exactly 60.0988 fps at all times.

### TO-DO list
- Accuracy of the program must be checked for 1 frame discrepancies.
- Console manager should be changed to a proper (hidable) window.
- A real use interface on a separate window is needed for the settings, right now settings can only be changed through the settings.ini.
- Quality of life
- Pause (pausing invalidates a run)
- Sound: I tried to understand how the nes sounds work, and while I got the basics I couldn't understand how to emulate sounds from a cartridge.
- More game modes
- Crt filter
- Modding options?
- Fullscreen option: fullscreen could reduce input lag by 1 frame

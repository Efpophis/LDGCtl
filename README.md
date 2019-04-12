# LDG AT-1000 ProII Control Program

This is my attempt to reverse-engineer the LDG AT-1000ProII Meter protocol in order to eventually implement software control AND meter reading. It's very much a work in progress, so no judging.

To communicate with the tuner, I needed a USB to TTL virtual serial adapter. The TTL is important, the voltages are different than for RS-232. Fortunately, the USB adapter I acquired takes care of all the level shifting for me. If you want to use an RS-232 port, you need to make or buy a level shifter to convert from 12V RS-232 to 5V TTL. A suitable item can be found on Amazon if you search for "FTDI USB TTL Adapter."

The other thing you need is a 4 PIN mini-DIN connector. These are terribly hard to find, but mouser has one [here](https://www.mouser.com/ProductDetail/Kycon/KMDLAX-4P?qs=sGAEpiMZZMsPDM5321osT7ZY%252bEpo2V%252bd). Note that S-Video cables, while having the same connector, cannot be used because they bridge some of the pins together. In fact, you should NEVER use an S-Video cable with your tuner, becasue one of the pins on the tuner is +12V and you don't want that getting fed back into someplace it doesn't belong. Also, do NOT get the 4-pin connector from Amazon - they tend to melt.

LDG published a document called "meter protocol" for the LDG M-1000, which, due to it seeming to suddenly want to vanish from online, I have included in my repository. Much of my information for this project came from there.

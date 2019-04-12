# LDG AT-1000 ProII Control Program

This is my attempt to reverse-engineer the LDG AT-1000ProII Meter protocol in order to eventually implement software control AND meter reading. It's very much a work in progress, so no judging.

To communicate with the tuner, I needed a USB to TTL virtual serial adapter. The TTL is important, the voltages are different than for RS-232. Fortunately, the USB adapter I acquired takes care of all the level shifting for me. If you want to use an RS-232 port, you need to make or buy a level shifter to convert from 12V RS-232 to 5V TTL. A suitable item can be found on Amazon if you search for "FTDI USB TTL Adapter."

The other thing you need is a 4 PIN mini-DIN connector. These are terribly hard to find, but mouser has one [here](https://www.mouser.com/ProductDetail/Kycon/KMDLAX-4P?qs=sGAEpiMZZMsPDM5321osT7ZY%252bEpo2V%252bd). Note that S-Video cables, while having the same connector, cannot be used because they bridge some of the pins together. In fact, you should NEVER use an S-Video cable with your tuner, becasue one of the pins on the tuner is +12V and you don't want that getting fed back into someplace it doesn't belong. Also, do NOT get the 4-pin connector from Amazon - they tend to melt.

LDG published a document called "meter protocol" for the LDG M-1000, which, due to it seeming to suddenly want to vanish from online, I have included in my repository. Much of my information for this project came from there.

## Meter Telemetry

The data that comes from the LDG Tuner as intended for the M-1000 meter follows a certain format that looks kind of like this:

```
2 bytes representing forward power
2 bytes representing reflected power
2 bytes of something I can't discern, but it varies by band / frequency
2 byte End Of Message marker, always 0x3b3b or ";;"
```

Note that all values come across in Network Byte Order, so if you're on a little-endian machine like me, you have to use htons() to convert them before doing any additional math.

The forward and reflected power can be approximately converted to watts using the formula found in tuner.cpp. VSWR is then calculated based on the values of forward and reflected power.

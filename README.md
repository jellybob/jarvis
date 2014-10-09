# Jarvis

This repository is my playground for home automation stuff, mostly
based on Arduinos, duck tape, and hackery. You probably shouldn't
use it as an example of sensible development practices in the Arudino
world.

## Sub-projects

* Controller: The API that drives everything.
* IRLight: Arudino firmware to send messages to IR controlled lights.
* IRDump: Arudino firmeware to read command codes from infrared remotes.
* IRSend: Proof of concept sketch to send my TV a power signal.

## Implementation Notes

### Target Hardware

My initial implementation is designed to control Auraglow colour changing
LED light bulbs. Out of the box they just have a crappy IR remote, which
is pretty nasty. I'm replacing that bit with an Arduino based IR transmitter
initially, and then hopefully removing the receiver in the bulb and replacing
that entirely.

Communication between the controller and bulbs is via Ciesco SRF radios, largely
because thats what I have to hand.

The target hardware, at least initially, is somewhat esoteric. The IR transmitter
takes the form of an EMF 2014 badge, the [Tilda MKe](https://wiki.emfcamp.org/wiki/TiLDA_MKe),
which is an Arduino Due and bunch of extra bits. Most relevantly for this
an IR transmitter, and a Ciesco SRF radio.

This has required some hacking around of the IRremote library, both to
get Due support (thanks to https://github.com/enternoescape/Arduino-IRremote-Due)
and to send IR commands to the correct pin.

At some point I'll shrink things down a bit, and hopefully be able to biuild the whole
lot into the bulb itself.

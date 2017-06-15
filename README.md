# RPi-Fan-Control
Another Raspberry pi fan control daemon (made for a retropie box)

This is a very simple (no dependency) fan controller daemon for a raspberry pi.
Just download the source, run 'make' and then setup systemd by editing fan.service file to locate the daemon and setup parameters.

The fan is controlled with a mosfet from any GPIO pin (GPIO17 for me, it was easier to solder).

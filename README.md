# RPi-Fan-Control
Another Raspberry pi fan control daemon (made for a retropie box)

This is a very simple (no dependency, working with systemd) fan controller daemon for a raspberry pi 
* download the source
* run ```make``` to build the daemon  
* edit fan.service file to locate the daemon and setup parameters (see below for parameter details)
* copy fan.service to systemd files: ```sudo cp fan.service /lib/systemd/system/```
* and activate it: ```sudo systemctl enable fan.service```

The fan is controlled with a mosfet from any GPIO pin (GPIO17 for me, it was easier to solder). It will start when pi temperature reach a maximum threshold value, and turned off when the temperature decreases below another threshold.

## Schematic
![myimage-alt-tag](schematic.png)

Very simple! the 10k resistor is limiting the current from the GPIO pin, the 200k is a pull down resistor, to turned the mosfet off when the gate is disconnected from GPIO. Those 2 resistors make a voltage divider, so be careful when choosing their values as the voltage on the mosfet gate will be 3.3v * R2 / (R1 + R2), which has to be greater than mosfet gate threshold voltage (Vgss).
You can also omit the current limiting resistor (R1/10k), as I did, but do it only if you really know what you're doing!

Any n-channel mosfet should work, as long as Vgss is below 3.3v * R2 / (R1 + R2) and the maximum drain current can handle the current running through the fan. In my case I have 5V/140mA Fan and use a 2N7000 mosfet (Id = 200mA).

## Systemd configuration
You need to edit 'fan.service' in order to setup the GPIO pin used to control the fan and the temperatures threshold values.
Those parameters are the argument passed to the daemon when systemd launches it:

```
[Unit]
Description=Fan daemon

[Service]
Type=simple
ExecStart=/home/pi/fan/fan --daemon 17 53 56
Restart=always

[Install]
WantedBy=multi-user.target
```

What you need to change here is this line :
```
ExecStart=/home/pi/fan/fan --daemon 17 53 56
```
First, change the path to match the directory where the fan program is. The first argument (--daemon) is used to make the program  run an endless loop (a daemon), the second argument (17) is the GPIO pin, 53(°C) is the min threshold temperature and 56(°C) is the max threshold temperature.

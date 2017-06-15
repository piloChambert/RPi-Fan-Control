# RPi-Fan-Control
Another Raspberry pi fan control daemon (made for a retropie box)

This is a very simple (no dependency) fan controller daemon for a raspberry pi.
* download the source
* run 'make' to build the daemon  
* edit fan.service file to locate the daemon and setup parameters (see below for parameter details)
* copy fan.service to /lib/systemd/system/
* run sudo ```systemctl enable fan.service```

The fan is controlled with a mosfet from any GPIO pin (GPIO17 for me, it was easier to solder). It will start when pi temperature reach a maximum threshold value, and turned off when the temperature decrease below another threshold.

## Systemd configuration
You need to edit 'fan.service' in order to setup the GPIO pin used to control the fan and the temperatures threshold values.
Those parameters are the argument passed to the daemon when systemd launch it:

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
First, change the path to match the directory where the fan program is. The first argument (--daemon) is used to make the program to run an endless loop (a daemon), the second argument (17) is the GPIO pin, 53(°C) is the min threshold temperature and 56(°C) is the max threshold temperature.

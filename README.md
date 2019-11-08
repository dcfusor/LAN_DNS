# LAN_DNS
Extra name resolution for linux and LAN of things

Solved some issues I had with pure peer to peer networking and machines coming and going a lot.  For a decade or so I didn't have enough spare solar power on my off-grid homestead to have an always-on server, so...workaround.  
I still add this to just about everything on the LAN.

This is composed of (at least) two pieces of code.  One, called tellem, broadcasts a name-IP pair over UDP to the lan, 
using port 53831.  This is simple enough that it's also easy to implement on small arduino-class machines, which I use 
as part of my LAN of things.

The other half, tellme, listens on that port on all the bigger machines, and updates /etc/hosts with the info, so name
resolution is quick and sure - things tend to check hosts before other possibilities.  
The code notices when it hasn't received a broadcast from some other host for awhile, and removes its entry 
from /etc/hosts, so this handles machines coming and going gracefully.

This may not be needed these days, but bad memories of having to reboot pretty much an entire windows network 
to recognize a change in machine status die hard - and even the current linuxes sometimes take awhile to notice.

The code for linux boxes is perl. I'll add some example code in C for ESP8266 (which should also work in an ESP32) as well.

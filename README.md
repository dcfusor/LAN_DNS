# LAN_DNS
Extra name resolution for linux and LAN of things

Solved some issues I had with pure peer to peer networking and machines coming and going a lot.  For a decade or so I didn't have enough spare solar power on my off-grid homestead to have an always-on server, so...workaround.  
I still add this to just about everything on the LAN.

NOTE:  If you have other things messing around the /etc/hosts or have a custom hosts file, you don't want this - 
it will wipe that kind of thing out.

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

To use the perl code, copy tellem and tellme to /usr/bin and make them executable. Owned by root is fine, or whatever your
sense of system security likes should work. 
I copy the .service files to /etc/systemd/system - but systemd changes now and then on where it wants things like that.
systemctl enable tellem (and the same for tellme) will get them started at boot time.

For smaller machines, in my case, ESP8266, you don't have all the services and languages you'd have in a linux host, 
so you have to do a little more work.  In the case of the ESP series, it seems more or less impossible to actually 
change the machine name, which is derived from the last few digits in the MAC, deep in the bowels of the WiFi library.
(things like this DO change over time, I'm going with the way it was when I wrote that code).  So, a couple snippets are
needed, one to build a hostname, and one to do the tellem part.  
I've arranged to call the former somewhere in setup, and call the later around once per minute in loop().  
Too slow (over 2 minutes) and the other machines will assume you're gone, and too fast wastes resources.
I often add a little something to the name in the build part so that I know what is what, rather than just 
seeing this ESP_MACDIGITS kind of name when I "cat /etc/hosts" to see what's up.

I've included a very simple example of how to do this in an ESP266, including scheduling.


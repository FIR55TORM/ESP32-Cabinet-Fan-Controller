# Cabinet Fan Controller
or where ever you decided to put it, fan controller...

This project started with the need to cool a small Ikea cabinet full of computer hardware and it got me thinking, why not overengineer a solution to a simple problem?
So based on an ESP32 microcontroller and after some prototyping later, the fruits of my labour (hopefully ours at some point) are now here.

## Now this isn't an original idea
During my research, I came across the lovely project by @KlausMu and their project, found here: https://github.com/KlausMu/esp32-fan-controller is likely more fully featured and deserves credit. 

## Learning all the things
Because I'm new to the world of IoTs and MCUs, I really wanted to learn so I decided to write my own project but the electronics side of things was very useful in the project I mentioned above.
I decided to make a prototype on a breadboard, test it, then I took it a step further and made it into a PCB on EasyEda. I will in time include these here but it will likely be later while I get to grips with KiCad.
I will also include the Fusion 360 files for the 3D printable model and STL files. Be warned they are not bad but also not great too. Certainly there is room for improvement.

### A little warning about the PCB files... 
The PCB files I will eventually include here, are a use at your own risk type situation! I'm not an eletronics expert and consider myself a begineer. That said I would be happy to accept changes to the PCB provided there is proof of it working along with a reasonable explaination that myself and others can understand.

### Powering the thing
I currently power the ESP32 via a usb hub and the fans with a 12v 2amp adapter. I initially used a buck convertor to tap off a 12v dc supply into 5v but I didn't like the bulk it added and my electronics isn't good enought to confidently build my own into the PCB design so I left it until I get better at it.

## Current support
* DHT11 temp/humiditiy sensor
* ESP32 MCUs (I used an Adafruit Huzzah32 feather v2 with w.fl wifi connector)
* 128x64 display (I used an Adafruit featherwing oled 128x64 display)
* any 12v PC fans with PWM (4 pin)
* Tacho support 
* WiFi Manager
* web server (i'll update this with the actual library I used)
* Some basic html for the web server to, well, server. Used jQuery with Bootstrap 5. I intend to use webpack with this in future.

## Future features will be:
* MQTT Support
* Support for other temp/humidity sensors
* Moarrr fans??

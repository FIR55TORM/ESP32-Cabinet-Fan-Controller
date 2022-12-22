# Cabinet Fan Controller
or where ever you decided to put it, fan controller...

This project started with the need to cool a small Ikea cabinet full of computer hardware and it got me thinking, why not over-engineer a solution to a simple problem like cooling a cabinet full of hardware?

Based on an ESP32 microcontroller along with some prototyping later, the fruits of my labour (hopefully ours at some point) is now ready for use.

# Getting Started
After pulling the repo, you can simply open the project in VS Code and start playing. Make sure you have the PlatformIO IDE extension installed. Of course you will likely have to change a few settings in the `platformio.ini` like the type of board you're using if different.

## Building the web frontend
The `\cabinet-fan-project-frontend` directory contains the frontend project. You will need to make sure NodeJs is installed. 

To install all the node packages, switch to the frontend project's directory and run:

`npm install`

Once all packages are installed, then you should run:

`npm run build`

This will build the frontend project and spit out the files into the `/data` folder. Beware the `/data` folder is cleaned on every build of the frontend so you should not put any files you want to keep, inside. 

By this point you should ideally have built and flashed the ESP32. Using PlatformIO, build the filesystem image and then upload it also. This will flash the assets to your EPS32's flash chip via SPIFFS.

All the assets are gzipped using Vite and the vite compression plugin, we produce the smallest posible sized files to save space and increase loading speed at the browser end.

## Running the dev server locally
When testing or writing new frontend code, you will likely want to run the dev server. You can do this by running:

`npm run dev`

Please be aware that you may need to change the `DEV_PROXY_URL` constant variable found inside `vite.config.js`, to the IP address your ESP32 is assigned to. If it is wrong, you will not be able to communicate with the web server on the ESP32 while testing locally.

# Some Background...
## Now this isn't an original idea
During my research, I came across the lovely project by @KlausMu and their project, found here: https://github.com/KlausMu/esp32-fan-controller so it deserves credit especially for the electronics side of things. 

## Learning all the things
As I'm new to the world of IoT and MCUs, I really wanted to learn so I decided to write my own project.
I decided to make a prototype on a breadboard, test it, then I took it a step further and made it into a PCB on EasyEda. I will in time include these here but it will likely be later while I get to grips with KiCad in an effort to keeping things open-source.

I will include the Fusion 360 files for the 3D printable model and STL files. Be warned they are not bad but also not great too. Certainly there is room for improvement, certainly would like to make the models parametric.

### A little warning about the PCB files... 
The PCB files I will eventually include here, are a use at your own risk type situation! I'm not an electronics expert and consider myself a beginner. That said I would be happy to accept changes to the PCB provided there is proof of it working along with a reasonable explanation that myself and others can understand.

### Powering the thing
I currently power the ESP32 via a usb hub and the fans with a 12v 2amp adapter. I initially used a buck convertor to tap off a 12v dc supply into 5v but I didn't like the bulk it added and my electronics isn't good enough to confidently build my own 5v power delivery system into the PCB design for the MCU so I left it until I get better at it or until someone comes up with something better.

## Current support
* Written in C++ using platform.io via VS code
* DHT11 temp/humidity sensor
* ESP32 MCUs (I used an Adafruit Huzzah32 feather v2 with w.fl wifi connector)
* 128x64 display (I used an Adafruit featherwing oled 128x64 display)
* any 12v PC fans with PWM (4 pin)
* Tacho reading and setting support 
* Frontend Built using VUE-Cli and includes VueJs, Bootstrap 5, Pinia, Vite, and eCharts (TODO: need to write docs for that)
* WiFi Manager
* Web server w/ ESPAsyncWebServer
* MQTT Support. Full list of topics in the MqttTopics class
  
## Known Issues
* The DHT11 is problematic and looking to replace it but will leave the option to use it.
* The frontend will not update if changes are made via MQTT
  
## Future features will be:
* Support for Adafruit's AHT20 temp/humidity sensor + others
* Moarrr fans??

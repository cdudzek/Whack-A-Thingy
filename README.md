# Whack-A-Thingy
Whack-A-Thingy Multi-Gameboard

## Introduction
Whack-A-Thingy is a multi-game hardware and software platform built on an Arduino Mega 2560 controller.

This GitHub repo is a work in progress, as I am currently building the first game board as we speak. Photos, assembly guide, and gameplay will be added when available.

### Game Board
The gameboard consists of the following features:
* Arduino Mega 2560 controller
* RGB-Backlit LCD 16x2 display for menus, messages, and scores
* Two 30mm arcade buttons for menu controls (Select/Start)
* Eight 100mm colored LED-backlit arcade buttons for game targets
* Custom designed PCB to connect target buttons with Arduino controller
* Wooden base frame

### Games Included
* Whack-A-Thingy - Whack-A-Mole clone using up to 8 target buttons, with selectable speed/difficulty
* Simon Sez - Simon Says clone using up to 8 target buttons. The game runs until you get one wrong!
* Reaction Time - See how fast you can hit the button after it lights. Selectable up to 8 target buttons to increase the difficulty.
* Button Test - Check that all buttons can light up, can respond to hardware interrupts, and can respond to hardware polling

## Game Board Design
All hardware, software, and electronics used to create the board are (or soon will be) included in the repository.

The [Bill of Materials](https://github.com/cdudzek/Whack-A-Thingy/blob/master/Documents/Bill%20of%20Materials%20with%20Costs.xlsx) lists most of the electronics and hardware used in the build. The total cost of the build (not including some power tools and soldering equipment) was less than US$300. Many thanks to Adafruit for their awesome part selection at cheap prices!

### Arduino Mega and Source Code
The GameBoard folder contains all of the Arduino source files to build the game. 

The size of the code, memory footprint, and required I/Os for all of the game buttons required the use of an Arduino Mega controller (instead of an Uno-compatible). It is possible the game could be adpated to other controllers or platforms, but this is the only one I have tested with. 

#### Prerequisites
* The [Adafruit RGB LCD Library](https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library) for Arduino is required to communicate with the [I2C RGB LCD Shield for Arduinos](https://www.adafruit.com/product/716).

### Physical Board Design
The game board and components were designed in Solidworks. The CAD folder contains the original Solidworks models and assembly, as well as standard STEP files. The PDF file shows the components and assemblies.

### Electronics
The project design includes a custom-designed circuit board to interface from the Arduino to the big target buttons. The board was designed in Fritzing, and includes all of the necessary files to send the board out for fabrication. 

Each PCB controls 4 target buttons, so 2 are used for the 8 total game buttons. The price to have 3 boards fabricated with [OSHPark](https://oshpark.com/) was less than US$50.

I will be creating a Fritzing model for the custom PCBs, and plan to provide a full Fritzing assembly including the Arduino, custom PCBs, buttons, and all wiring. 

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE](LICENSE) file for details

## Acknowledgments
This project is 100% custom designed (hardware, software, and electronics), but I got the original idea from the [Whac-A-Mole](https://www.hackster.io/WT040/whac-a-mole-button-edition-a2d391) project on [Hackster.io](https://hackster.io).

I would like to give a shout out to the awesome folks at [Adafruit](https://www.adafruit.com/) for their awesome product selection (including the massive [100mm arcade buttons](https://www.adafruit.com/product/1185)!), tutorials, code libraries and examples, and everything else they do. They are an inspiration. (I am in no way affiliated with Adafruit, just a very happy repeat customer.)

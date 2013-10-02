VHSLEDMatrixBadge
=================
http://vancouver.hackspace.ca/wp/2013/09/30/vhs-led-matrix-badge/



A few months ago we were talking about the Defcon and HOPE convention badges are and how awesome it would be if we could make one for VHS. "It can't be that hard" we said. We will just put an Arduino on a board and put a LED matrix on it. It won't take longer than weekend to build, a week or so of programming and we'll be good to go. ~2 full months later just in time for SHHH 29 and we have a prototype!
* HOPE Badge http://amd.hope.net/2010/07/postmortem/
* DEFCON http://vancouver.hackspace.ca/wp/2013/09/30/vhs-led-matrix-badge/Badge%20http://grandideastudio.com/wp-content/uploads/history_of_defcon_electronic_badges_slides.pdf
* OSH2013 http://www.seeedstudio.com/blog/2013/08/02/open-hardware-summit-2013/

Features
=================

The badge has 2 LED Matrix (8x8) next to each other, in a 8x16 matrix. Each pixel in the matrix can be either RED or GREEN, or any variation between the two.
The badge has a tiny microphone built into the board, that is connected to one of the analog pins on the microcontroller. It can be used for an equalizer or other interesting projects.
The board has a 4 way joystick with a centre button in the bottom left and a single button in the bottom right. We hope people will make simple pixel based games
* Mario Brothers Pixel game http://www.youtube.com/watch?v=cB6vSHDDdhI
* Racecar, object avoidance https://www.youtube.com/watch?v=ydYfNDpr734

We added a Complex Programmable Logic Device (CPLD) to controlling the LED matrix and decoding the button presses. It also has the added advantage that people can learn about Verilog and VHDL. (Note: The CPLD will come pre-programmed so you don't need to know Verilog and VHDL to get started) There are three exposed digital pins that are connected directly to the microcontroller for adding your own peripherals.
JTAG programming and debugging of the CPLD, SPI, I2C, AVR debugWire, Arduino bootloader and a few other nice features.

Schematics
=================

https://github.com/vhs/vhs-pcbs/raw/master/led-matrix/LEDMatrixLearningBadge.pdf

Bill of Materials
=================

* 2x - GYXM-1588-AURG - LED Matrix 8x16, 2 color (Red, Green)
* 1x - ATmega168-PV - Arduino 168
* 1x - 5M40ZE64 - CPLD (Complex Programmable Logic Device)
* 1x - BL-5F - 3.7v, 950 mA (Nokia cell phone batteries)

Full bill of materials
http://htmlpreview.github.io/?https://github.com/vhs/vhs-pcbs/master/led-matrix/LEDMatrixLearningBadge-BOM.html

Source and board design files
=================

The board design files and example source code can be found on VHS's github (https://github.com/vhs/vhs-pcbs/tree/master/led-matrix). Pull requests, comments and suggestions are welcome!
The board design and source code will be released under Creative Commons Attribution-ShareAlike 3.0 Unported License. Allowing anyone to copy, edit, and create their own updated versions of these boards as long as they provide attribution and share their changes under the same licence.

Cost:
=================

One of the design goals of this project is to make it as cheap as possible. We wanted to keep it cheap so the cost of the board would not be a barrier to playing with one. Unfortunately we did not count on the chinese Mid-Autumn Festival, and September equinox holidays. This prevented us from getting the boards printed in China in time for SHHH. We ended up getting the first run of boards printed in Vancouver at a considerable premium.
$20 for VHS members.

Future:
=================

We are looking for feedback on this version of the boards so we can make revisions and possibly make a Kickstarter. The boards are open source, the firmware and example code will also be open source on Github.
We will be running a series of classes on the design process that went into the board, manufacturing, etc, and using it as a teaching platform for AVR-GCC, and Verilog/VHDL

Licence:
=================

Creative Commons LicenceVHS LED Matrix Badge by Richard Sim. Updated project files can be found here . This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
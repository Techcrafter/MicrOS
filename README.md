```
     /|    //| |                          //   ) ) //   ) )
    //|   // | |    ( )  ___      __     //   / / ((
   // |  //  | |   / / //   ) ) //  ) ) //   / /    \\
  //  | //   | |  / / //       //      //   / /       ) )
 //   |//    | | / / ((____   //      ((___/ / ((___ / /
```

# MicrOS
A multitasking operating system for Arduino and compatible boards.

The idea behind this operating system is to use the Arduino as a standalone computer. The only thing you'll need (beside the Arduino itself) is a way to send and receive text via serial (the serial monitor that comes with the Arduino IDE for example). There is also an easy way to add libraries for keyboards and LCD screens for example, to use those as input and/or output devices.
You will be able to program on the Arduino itself over the serial connection, run those programs and even save them to the EEPROM!

## Still under development
MicrOS is still under development and far away from being finished. If you like this project, please consider giving it a star!

## Compatibility
|Board|Compatibility   |Reason                   |
|-----|----------------|-------------------------|
|Nano |Not Compatible  |Not enough dynamic memory|
|UNO  |Not Compatible  |Not enough dynamic memory|
|MEGA |Fully Compatible|                         |
|     |                |                         |
|     |                |                         |

***This list is not completed yet and may change at any time during the development process!***

## How to install

***Remeber: MicrOS is not finished yet! You might get unexpected results.***

1. Install the Arduino IDE if you have not done this already.
2. [Download the latest (unstable) main commit](https://raw.githubusercontent.com/Techcrafter/MicrOS/main/MicrOS.ino) - right click, 'Save link as...' and save as 'MicrOS.ino'.
3. Open the downloaded 'MicrOS.ino' file in the Arduino IDE, select your board and the correct port in the 'Tools' section.
4. Open the serial monitor in the 'Tools' section and set it to 'Newline' line-ending and 9600 bauds.
5. Upload the sketch to your board.
6. MicrOS should now be usable in the serial monitor window.
7. ![MicrOS setup assistant](https://raw.githubusercontent.com/Techcrafter/MicrOS/main/docs/img/setup.png)
8. After you have completed the initial setup and restarted your board, you will see the command line.
9. ![MicrOS setup assistant](https://raw.githubusercontent.com/Techcrafter/MicrOS/main/docs/img/command-line.png)
10. You can find more information on how to use MicrOS below this.

## Instructions and documentations (not finished yet)

  * [HTML version of this document](https://techcrafter.github.io/MicrOS)
  * [Setup and configuration guide (custom IO, change serial baud rate, ...)](https://techcrafter.github.io/MicrOS/setup.html)
  * [Basic user guide (commands, syntax, ...)](https://techcrafter.github.io/MicrOS/user-guide.html)
  * [How to code in MicroASM](https://techcrafter.github.io/MicrOS/MicroASM.html)
  * [Developer documentation](https://techcrafter.github.io/MicrOS/dev-doc.html)
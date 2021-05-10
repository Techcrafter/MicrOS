```
     /|    //| |                          //   ) ) //   ) )
    //|   // | |    ( )  ___      __     //   / / ((
   // |  //  | |   / / //   ) ) //  ) ) //   / /    \\
  //  | //   | |  / / //       //      //   / /       ) )
 //   |//    | | / / ((____   //      ((___/ / ((___ / /
```

# Setup and configuration guide

## Change the serial baud rate:
Simply locate the following line and change it to your desired value.

![Change your serial baud rate here](https://raw.githubusercontent.com/Techcrafter/MicrOS/main/docs/img/serial-baud-rate.png)

## Configure other input or output devices:
 1. Add the libraries in the section for those.
 2. ![Add libraries and function code here](https://raw.githubusercontent.com/Techcrafter/MicrOS/main/docs/img/customIO.png)
 3. Configure the code for input and/or output in the functions called customInput() and customOutput().
 4. Upload the sketch to your Arduino.
 5. Run the command matching your setup:
   * - 'sysconf customIO none' to disable custom input and output
   * - 'sysconf customIO input' to enable only custom input
   * - 'sysconf customIO output' to enable only custom output
   * - 'sysconf customIO both' to enable custom input and output
 6. ***Your configuration will be saved to the EEPROM!***
 7. You should now be able to use your new input and/or output device!

***Warning: Try to avoid delays in your IO functions. This would slow down the entire system.***

[Back to the main page](https://techcrafter.github.io/MicrOS)
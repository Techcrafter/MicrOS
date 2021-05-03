/*     
 *     /|    //| |                          //   ) ) //   ) ) 
 *    //|   // | |    ( )  ___      __     //   / / ((        
 *   // |  //  | |   / / //   ) ) //  ) ) //   / /    \\      
 *  //  | //   | |  / / //       //      //   / /       ) )   
 * //   |//    | | / / ((____   //      ((___/ / ((___ / /    
 * 
 *            For Arduino and compatible boards
 * 
 *        If you need more informations, feel free to
 *        visit https://github.com/Techcrafter/MicrOS
 * 
 *        How to install:
 *        1) Open serial monitor and select 9600 baud rate.
 *           > The baud rate can be changed in the section below.
 *        2) Upload the sketch to your arduino.
 *        3) Wait until the setup shows up on your serial monitor.
 *        4) Follow the steps of the setup assistant.
 *        5) Enjoy! ;)
 * 
 *        Configure other input or output devices:
 *        1) Add the libraries in the section below.
 *        2) Configure the code for input and/or output in the
 *           functions called customInput() and customOutput() below.
 *        3) Upload the sketch to your arduino.
 *        4) Run the command matching your setup:
 *           - 'sysconf customIO none' to disable custom input and output
 *           - 'sysconf customIO input' to enable only custom input
 *           - 'sysconf customIO output' to enable only custom output
 *           - 'sysconf customIO both' to enable custom input and output
 *           Your configuration will be saved to the EEPROM!
 *         5) You should now be able to use your new input and/or output device!
 */

//You can change the serial baud rate here:
const int serialBaudRate = 9600;

//Custom libraries for input and output go here:
//#include <MyCustomInputLibrary.h>
//#include <MyCustomOutputLibrary.h>

String customInput()  //Custom input function
{
  //Add your input code here and simply return the string with your input.
  //If no input was made, make sure that the function returns nothing (return "";)
  // -> Make sure that your code runs without delays!
  //    Else, you would slow down the entire system a lot.
}

void customOutput(String str)  //Custom output function
{
  //Add your output code here and use the str variable as the string to output.
  //Each function call represents one line.
  // -> Make sure that your code runs without delays!
  //    Else, you would slow down the entire system a lot.
}

//---------------------------------------------------------------------------------------
//------ Do not edit below this line unless you want to change the OS code itself! ------
//---------------------------------------------------------------------------------------

#include <EEPROM.h>

const int osVersion = 101;
const String ident = "MicrOS";

const int serialTimeout = 500;
const int waitingPhase = 500;

bool customInputEnabled;
bool customOutputEnabled;

int bufInt;
char bufChar;
char data[100];
String bufStr;

String part1, part2, part3;

void setup() {
  //bootcode
  EEPROM.begin();
  if(readEEPROMToString(0) != ident)  //Check if MicrOS is already set up on this device
  {
    //if it's not, run the initial setup
    initialSetup();
  }
  else
  {
    Serial.begin(serialBaudRate);  //Begin serial
    Serial.setTimeout(serialTimeout);
    
    if(readEEPROMToInt(8) < 0 || readEEPROMToInt(8) > 3)
    {
      error("Invalid IO configuration!");
    }
    if(readEEPROMToInt(8) == 1 || readEEPROMToInt(8) == 3)
    {
      customInputEnabled = true;
    }
    if(readEEPROMToInt(8) == 2 || readEEPROMToInt(8) == 3)
    {
      customOutputEnabled = true;
    }
    else
    {
      delay(waitingPhase);
      while(!Serial);  //wait for serial connection to be established
      delay(waitingPhase);
    }
    
    outputLogo();
    output("");
    
    if(readEEPROMToInt(6) < osVersion)
    {
      output("It seems like you've updated MicrOS!");
      output("Enter 'update' to update your EEPROM  or turn power off...");
      while(input() != "update");
      output("Updating... (Please be patient!)");
      EEPROM[6] = osVersion;
      output("Done!");
      delay(2000);
    }
    else if(readEEPROMToInt(6) > osVersion)
    {
      output("It seems like you've downgraded MicrOS!");
      output("Continuing could cause EEPROM corruption!");
      output("Please update MicrOS or type 'ignore' to continue anyways...");
      while(input() != "ignore");
    }
  }

  output("System ready!");
  output("");
  output("MicrOS:mfs1/");
}

void loop() {
  //main runtime code
  commandInterpreter(input());
  delay(waitingPhase);
}

void commandInterpreter(String str)  //interpreter for commands
{
  if(str != "X-x-X")
  {
    memset(data, 0, sizeof(data));  //clear data before use
    str.toCharArray(data, str.length() + 1);
    if(data[str.length() - 1] != ' ')
    {
      data[str.length()] = ' ';
      str = String(data);
    }
    part1 = "";
    part2 = "";
    part3 = "";
    for(bufInt = 0; bufInt < str.length(); bufInt++)
    {
      if(str.substring(bufInt, bufInt + 1) == " ")
      {
        if(part1 == "")
        {
          part1 = str.substring(0, bufInt);
        }
        else if(part2 == "")
        {
          part2 = str.substring(part1.length() + 1, bufInt);
        }
        else if(part3 == "")
        {
          part3 = str.substring(part1.length() + part2.length() + 2, bufInt);
        }
        else
        {
          error("Too many arguments!");
        }
      }
    }

    bufInt = 0;  //use bufChar as exit code

    //-----------command part 1-----------
    if(part1 == "sysconf")
    {
      //-----------sysconf-----------
      if(part2 == "customIO")
      {
        //-----------sysconf/customIO-----------
        if(part3 == "none")
        {
          //-----------sysconf/customIO/none-----------
          writeIntToEEPROM(8, 0);
          bufInt = 2;
        }
        else if(part3 == "input")
        {
          //-----------sysconf/customIO/input-----------
          writeIntToEEPROM(8, 1);
          bufInt = 2;
        }
        else if(part3 == "output")
        {
          //-----------sysconf/customIO/output-----------
          writeIntToEEPROM(8, 2);
          bufInt = 2;
        }
        else if(part3 == "both")
        {
          //-----------sysconf/customIO/both-----------
          writeIntToEEPROM(8, 3);
          bufInt = 2;
        }
      }
      else if(part2 == "reset")
      {
        //-----------sysconf/reset-----------
        output("Resetting in 60 seconds!");
        output("Power off to cancle...");
        delay(60000);
        writeStringToEEPROM(0, "      ");
        output("Please restart the system now...");
        while(1);
      }
    }

    //command exit codes
    if(bufInt == 0)  //unknown
    {
      output("Unknown command!");
    }
    else if(bufInt == 1)  //normal exit
    {
      //no message here
    }
    else if(bufInt == 2)  //completed
    {
      output("The command ran successfully!");
    }
    else if(bufInt == 3)  //failed
    {
      output("The command failed!");
    }

    output("MicrOS:mfs1/");
  }
}

void programInterpreter()  //interpreter for programs
{
  
}

String input()  //input a string
{
  if(Serial.available() > 0)
  {
    bufInt = 0;
    memset(data, 0, sizeof(data));  //clear data before use
    while(Serial.available() > 0)
    {
      bufChar = Serial.read();
      if(bufChar != 10)
      {
        data[bufInt] = bufChar;
        bufInt++;
      }
      else
      {
        data[bufInt] = '\0';
        return String(data);
      }
    }
  }
  
  if(customInputEnabled)
  {
    return customInput();
  }

  return "X-x-X";
}

void output(String str)  //output a string
{
  if(str != "X-x-X")
  {
    Serial.println(str + '\n');
    if(customOutputEnabled)
    {
      customOutput(str);
    }
  }
}

void outputLogo()  //output the MicrOS logo
{
  output("    /|    //| |                          //   ) ) //   ) )");
  output("   //|   // | |    ( )  ___      __     //   / / ((");
  output("  // |  //  | |   / / //   ) ) //  ) ) //   / /    \\");
  output(" //  | //   | |  / / //       //      //   / /       ) )");
  output("//   |//    | | / / ((____   //      ((___/ / ((___ / /");
  output("version " + String(osVersion));
}

void error(String reason)  //display error message to user with a reason
{
  output("");
  output("");
  output("An error has occured!");
  output("The system has temporarily been halted to prevent further damage.");
  output("");
  output("Reason for this error:");
  output(reason);
  output("");
  output("If you enbufInter this issue multiple times, please open an issue on GitHub!");
  output("");
  output("Enter 'resume' to continue or turn power off...");
  while(input() != "resume");
}

void writeIntToEEPROM(int adress, int number)  //write an int to the EEPROM
{
  EEPROM.write(adress, number >> 8);
  EEPROM.write(adress + 1, number & 0xFF);
}

int readEEPROMToInt(int adress)  //read an int from the EEPROM
{
  byte byte1 = EEPROM.read(adress);
  byte byte2 = EEPROM.read(adress + 1);

  return (byte1 << 8) + byte2;
}

void writeStringToEEPROM(char adressStart, String str)  //write a string to the EEPROM
{
  for(bufInt = 0;bufInt < str.length();bufInt++)
  {
    EEPROM.write(adressStart + bufInt, str[bufInt]);
  }
  EEPROM.write(adressStart + str.length(), '\0');
}

String readEEPROMToString(char adressStart)  //read a string from the EEPROM
{
  bufChar = EEPROM.read(adressStart);
  memset(data, 0, sizeof(data));  //clear data before use
  while(bufChar != '\0' && bufInt<500)
  {
    bufChar=EEPROM.read(adressStart + bufInt);
    data[bufInt] = bufChar;
    bufInt++;
  }
  data[bufInt] = '\0';

  return String(data);
}

void initialSetup()  //the setup which will run on first boot
{
  Serial.begin(serialBaudRate);
  Serial.setTimeout(serialTimeout);
  while(!Serial);  //wait for serial connection to be established
  outputLogo();
  output("");
  output("Welcome to the MicrOS setup!");
  output("Your EEPROM (" + String(EEPROM.length()) + " bytes) needs to be formated with mfs1 type.");
  output("!!!ALL EEPROM data will be deleted!!!");
  output("Type 'format' to continue or turn power off...");
  while(input() != "format")
  {
    delay(waitingPhase);
  }
  output("Formatting EEPROM... (Please be patient!)");
  bufInt = 0;
  while(bufInt <= EEPROM.length())
  {
    EEPROM[bufInt] = 0;
    bufInt++;
  }
  writeStringToEEPROM(0, ident);
  writeIntToEEPROM(6, osVersion);
  output("Formatted successfully!");
  delay(2000);
  output("Setup completed!");
  delay(1000);
  output("Please restart the system now...");
  while(1);
}

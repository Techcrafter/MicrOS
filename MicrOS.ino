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
 *        1) Open serial monitor and select
 *           "Newline" line ending and 9600 baud rate.
 *           > The baud rate can be changed in the section below.
 *        2) Upload the sketch to your Arduino.
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
  //Add your input code here and simply return the string with your input (without line ending ('\n')).
  //If no input was made, make sure that the function returns nothing (return "";)
  // -> Try to avoid using delays!
  //    Using them slows down the entire system speed.
}

void customOutput(String str)  //Custom output function
{
  //Add your output code here and use the str variable as the string to output.
  //Each function call represents one line.
  //Newline characters ('\n') are used too!
  // -> Try to avoid using delays!
  //    Using them slows down the entire system speed.
}

//---------------------------------------------------------------------------------------
//------ Do not edit below this line unless you want to change the OS code itself! ------
//---------------------------------------------------------------------------------------

#include <EEPROM.h>

const int osVersion = 101;  //os version
const String ident = "MicrOS";  //os identifier

const bool debugMode = true;  //set this to true to enable debug commands

const int serialTimeout = 500;  //500 gives best results
const int waitingPhase = 100;  //100 gives best results

bool customInputEnabled;
bool customOutputEnabled;

bool bufBool;
int bufInt1;
int bufInt2;
char bufChar;
char data[100];
String bufStr;

String procStr;
int exitCode;

String part1, part2, part3;  //comand parts

int task[3];  //task states
String taskName[3];  //task names

int regA[3];
int regB[3];
int regC[3];
String out[3];

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
      while(!Serial)  //wait for serial connection to be established
      {
        delay(waitingPhase);
      }
      delay(waitingPhase);
    }
    
    outputLogo();
    output("");
    
    if(readEEPROMToInt(6) < osVersion)
    {
      output("It seems like you've updated MicrOS!\nEnter 'update' to update your EEPROM  or turn power off...");
      while(input() != "update")
      {
        delay(waitingPhase);
      }
      output("Updating... (Please be patient!)");
      writeIntToEEPROM(6, osVersion);
      output("Done!");
      delay(2000);
    }
    else if(readEEPROMToInt(6) > osVersion)
    {
      output("It seems like you've downgraded MicrOS!\nContinuing could cause EEPROM corruption!\nPlease update MicrOS or type 'ignore' to continue anyways...");
      while(input() != "ignore")
      {
        delay(waitingPhase);
      }
    }
  }

  output("System ready!\n\nMicrOS:sto0/");
}

void loop() {
  //main runtime code
  procStr = input();
  if(procStr != "X-x-X")
  {
    splitter();
    commandInterpreter();
  }
  delay(waitingPhase);
}

void splitter()
{
  memset(data, 0, sizeof(data));  //clear data before use
  procStr.toCharArray(data, procStr.length() + 1);
  if(data[procStr.length() - 1] != ' ')
  {
    data[procStr.length()] = ' ';
    procStr = String(data);
  }
  part1 = "";
  part2 = "";
  part3 = "";
  for(bufInt1 = 0; bufInt1 < procStr.length(); bufInt1++)
  {
    if(procStr.substring(bufInt1, bufInt1 + 1) == " ")
    {
      if(part1 == "")
      {
        part1 = procStr.substring(0, bufInt1);
      }
      else if(part2 == "")
      {
        part2 = procStr.substring(part1.length() + 1, bufInt1);
      }
      else if(part3 == "")
      {
        part3 = procStr.substring(part1.length() + part2.length() + 2, bufInt1);
      }
      else
      {
        part3 = procStr.substring(part1.length() + part2.length() + 2, procStr.length());
      }
    }
  }
}

void commandInterpreter()  //interpreter for commands
{
  exitCode = 0;  //set exit code to 0
  output("");  //leave one free line

  //-----------sysconf-----------
  if(part1 == "sysconf")
  {
    //-----------sysconf/customIO-----------
    if(part2 == "customIO")
    {
      exitCode = 2;
      //-----------sysconf/customIO/none-----------
      if(part3 == "none")
      {
        writeIntToEEPROM(8, 0);
      }
      //-----------sysconf/customIO/input-----------
      else if(part3 == "input")
      {
        writeIntToEEPROM(8, 1);
      }
      //-----------sysconf/customIO/output-----------
      else if(part3 == "output")
      {
        writeIntToEEPROM(8, 2);
      }
      //-----------sysconf/customIO/both-----------
      else if(part3 == "both")
      {
        writeIntToEEPROM(8, 3);
      }
      else
      {
        output("Usage: sysconf customIO [none | input | output | both]");
        exitCode = 0;
      }
    }
    //-----------sysconf/reset-----------
    else if(part2 == "reset")
    {
      output("Do you really want to reset?\nYou will lose all your data!\nType 'continue' to continue or turn power off...");
      while(input() != "continue")
      {
        delay(waitingPhase);
      }
      output("Resetting in 60 seconds...\nPower off to cancle!");
      delay(60000);
      writeStringToEEPROM(0, "      ");
      output("Reset done!\nPlease restart the system now...");
      while(1);
    }
    else
    {
      output("Usage: sysconf [customIO | reset]");
    }
  }
  //-----------echo-----------
  else if(part1 == "echo")
  {
    output(part2 + " " + part3);
  }
  //-----------help-----------
  else if(part1 == "help")
  {
    output("Visit 'https://github.com/Techcrafter/MicrOS' for help!");
  }
  //-----------info-----------
  else if(part1 == "sysinfo")
  {
    outputLogo();
    output("ERS: " + String(EEPROM.length()) + " bytes | ERFS: mfs1 | ERI: sto0 | SBR: " + String(serialBaudRate) + " | ST: " + String(serialTimeout) + " | WP: " + String (waitingPhase));
    if(debugMode)
    {
      output("Debug mode enabled!");
    }
  }
  //-----------dir-----------
  else if(part1 == "dir")
  {
    output("Contents of sto0/:");
    mfs1Read(1, "sto0/");
    output("\nAvailable space: " + mfs1Read(0, "sto0") + " bytes");
  }
  //-----------storage-----------
  else if(part1 == "storage")
  {
    //-----------storage/list-----------
    if(part2 == "list")
    {
      output("Storage listing:\n\n--- sto0 ---\nFilesystem: mfs (version 1)\nTotal space: " + String(EEPROM.length()) + " bytes\nAvailable space: " + mfs1Read(0, "sto0") + " bytes");
    }
    //-----------storage/repair-----------
    else if(part2 == "repair")
    {
      output("Repairing storage... (Please be patient!)");
      if(readEEPROMToInt(6) != osVersion)
      {
        writeIntToEEPROM(6, osVersion);
        output("Fixed invalid os version!");
      }
      bufInt1 = 10;
      while(bufInt1 < EEPROM.length())
      {
        if(EEPROM.read(bufInt1) == 123)
        {
          bufInt2 = bufInt1;
          while(EEPROM.read(bufInt2) != 125)
          {
            bufInt2++;
            if(bufInt2 == EEPROM.length())
            {
              EEPROM.write(EEPROM.length() - 1, 125);
              output("Fixed unexpected end of disk!\nSome data may be lost or corrupted!");
            }
          }
        }
        bufInt1++;
      }
      output("Done!");
    }
    else
    {
      output("Usage: storage [list | repair]");
    }
  }
  //-----------run-----------
  else if(part1 == "run")
  {
    if(part3 == "single")
    {
      output("Running in single tasking mode!\nOther processes get killed.");
      task[0] = 0; task[1] = 0; task[2] = 0;
    }

    bufStr = mfs1Read(2, part2);
    if(bufStr == "MaxTasks")
    {
      output("Task limit of 3 has already been reached!");
    }
    else if(bufStr == "NotFound")
    {
      output("Program not found!");
      output("Usage: run (filename)");
    }
    else
    {
      output("Task started with PID " + bufStr + "!");
    }
  }
  //-----------tasks-----------
  else if(part1 == "task")
  {
    if(part2 == "list")
    {
      output("PID | Task\n----------\nX   | (SYSTEM)");
      bufInt1 = 0;
      while(bufInt1 < 3)
      {
        if(task[bufInt1] != 0)
        {
          output(String(bufInt1) + "     " + taskName[bufInt1]);
        }
        bufInt1++;
      }
      output("\nLimit: 3 tasks");
    }
    else if(part2 == "kill")
    {
      if(part3.toInt() < 0 || part3.toInt() > 2)
      {
        output("Wrong syntax or invalid PID!\nUsage: task kill (PID)");
      }
      else if(task[part3.toInt()] != 0)
      {
        output(String(task[part3.toInt()]));
        task[part3.toInt()] = 0;
        output("Task killed successfully!");
      }
      else
      {
        output("There is no task with this PID!");
      }
    }
    else
    {
      output("Usage: task [list | kill]");
    }
  }
  //-----------debug-----------
  else if(part1 == "debug")
  {
    if(debugMode)
    {
      //-----------debug/error-----------
      if(part2 == "error")
      {
        error("Manually triggered error!");
      }
      else
      {
        output("Usage: debug [error]");
      }
    }
    else
    {
      output("Debug mode is disabled!");
    }
  }
  else
  {
    exitCode = 1;
  }
  
  //command exit codes
  if(exitCode == 0)  //normal exit
  {
    //no message here
  }
  else if(exitCode == 1)  //unknown
  {
    output("Unknown command!");
  }
  else if(exitCode == 2)  //completed
  {
    output("The command ran successfully!");
  }
  else if(exitCode == 3)  //failed
  {
    output("The command failed!");
  }

  output("\nMicrOS:sto0/");
}

void programInterpreter()  //interpreter for programs
{
  
}

String mfs1Read(int action, String parameter)
{
  //0 - check free space
  if(action == 0 && parameter == "sto0")
  {
    bufInt1 = 10;
    bufInt2 = 0;
    bufBool = false;
    while(bufInt1 < EEPROM.length())
    {
      if(!bufBool)
      {
        if(EEPROM.read(bufInt1) == 123)
        {
          bufBool = true;
        }
        else
        {
          bufInt2++;
        }
      }
      else if(EEPROM.read(bufInt1) == 125)
      {
        bufBool = false;
      }
      bufInt1++;
    }

    return String(bufInt2);
  }
  //1 - list all files in /
  else if(action == 1 && parameter == "sto0/")
  {
    output("(SYSTEM CONFIGURATION) - 10 bytes");
    bufInt1 = 10;
    while(bufInt1 < EEPROM.length())
    {
      if(EEPROM.read(bufInt1) == 123)
      {
        bufInt2 = bufInt1;
        while(EEPROM.read(bufInt2) != 125)
        {
          bufInt2++;
          if(bufInt2 == EEPROM.length())
          {
            while(1)
            {
              error("Disk sto0 is corrupted!");
            }
          }
        }
        output(readEEPROMToString(bufInt1+1) + " - " + String(bufInt2 - bufInt1) + " bytes");
      }
      bufInt1++;
    }

    return "Done!";
  }
  //2 - execute/open file/program with the filename defined by the parameter string
  else if(action == 2)
  {
    bufInt1 = 10;
    while(bufInt1 < EEPROM.length())
    {
      if(EEPROM.read(bufInt1) == 123)
      {
        if(readEEPROMToString(bufInt1+1) == parameter)
        {
          bufInt2 = 0;
          while(bufInt2 < 3)
          {
            if(task[bufInt2] == 0)
            {
              task[bufInt2] = bufInt1 + parameter.length() + 1;
              taskName[bufInt2] = parameter;
              
              regA[bufInt2] = 0;
              regB[bufInt2] = 0;
              regC[bufInt2] = 0;
              out[bufInt2] = "";
              
              return String(bufInt2);
            }
            bufInt2++;
          }
          
          return "MaxTasks";
        }
      }
      bufInt1++;
    }

    return "NotFound";
  }
  else
  {
    error("Invalid mfs1 read instruction!");
  }
}

String input()  //input a string
{
  if(Serial.available() > 0)
  {
    bufInt1 = 0;
    memset(data, 0, sizeof(data));  //clear data before use
    while(Serial.available() > 0)
    {
      bufChar = Serial.read();
      if(bufChar != 10)
      {
        data[bufInt1] = bufChar;
        bufInt1++;
      }
      else
      {
        data[bufInt1] = '\0';
        return String(data);
      }
    }
  }
  
  if(customInputEnabled)
  {
    bufStr = customInput();
    if(bufStr != "")
    {
      return bufStr;
    }
  }

  return "X-x-X";
}

void output(String str)  //output a string
{
  if(str != "X-x-X")
  {
    Serial.println(str);
    if(customOutputEnabled)
    {
      customOutput(str);
    }
  }
}

void outputLogo()  //output the MicrOS logo
{
  output("    /|    //| |                          //   ) ) //   ) )\n   //|   // | |    ( )  ___      __     //   / / ((\n  // |  //  | |   / / //   ) ) //  ) ) //   / /    \\\n //  | //   | |  / / //       //      //   / /       ) )\n//   |//    | | / / ((____   //      ((___/ / ((___ / /\nversion " + String(osVersion));
}

void error(String reason)  //display error message to user with a reason
{
  output("\nAn error has occured!\nThe system has been temporarily halted to prevent further damage.\n\nReason for this error:\n" + reason + "\n\nIf you encounter this issue multiple times, please open an issue on GitHub!\n\nEnter 'resume' to continue or turn power off...");
  while(input() != "resume")
  {
    delay(waitingPhase);
  }
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
  for(bufInt1 = 0;bufInt1 < str.length();bufInt1++)
  {
    EEPROM.write(adressStart + bufInt1, str[bufInt1]);
  }
  EEPROM.write(adressStart + str.length(), '\0');
}

String readEEPROMToString(char adressStart)  //read a string from the EEPROM
{
  bufChar = EEPROM.read(adressStart);
  memset(data, 0, sizeof(data));  //clear data before use
  while(bufChar != '\0' && bufInt1<500)
  {
    bufChar=EEPROM.read(adressStart + bufInt1);
    data[bufInt1] = bufChar;
    bufInt1++;
  }
  data[bufInt1] = '\0';

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
  output("Your EEPROM (" + String(EEPROM.length()) + " bytes) needs to be formated with mfs1 filesystem.");
  output("!!!ALL EEPROM data will be deleted!!!");
  output("Type 'format' to continue or turn power off...");
  while(input() != "format")
  {
    delay(waitingPhase);
  }
  output("Formatting EEPROM... (Please be patient!)");
  bufInt1 = 0;
  while(bufInt1 <= EEPROM.length())
  {
    EEPROM.write(bufInt1, 0);
    bufInt1++;
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

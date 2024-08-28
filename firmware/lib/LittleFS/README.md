# LittleFS

This is a wrapper for the LittleFS File System for the Teensy family of microprocessors and provides support for RAM Disks, NOR and NAND Flash chips, and FRAM chips.  For the NOR, NAND Flash support is provided for SPI and QSPI in the case of the Teensy 4.1.  For FRAM only SPI is supported.

In addition, LittleFS is linked with the SD library so the user can create, delete, read/write files to any of the supported memory type.  See usuage section. 

## Supported Chips

### NOR Flash

####
MFG | PART # | Size
------------ | ------------- |------------ 
Winbond | W25Q16JV*IQ/W25Q16FV | 16Mb
... | W25Q32JV*IQ/W25Q32FV | 32Mb
... | W25Q64JV*IQ/W25Q64FV | 64Mb
... | W25Q128JV*IQ/W25Q128FV | 128Mb
... | W25Q256JV*IQ | 256Mb
... | Winbond W25Q512JV*IQ | 512Mb
... | W25Q64JV*IM (DTR) | 64Mb
... | W25Q128JV*IM (DTR) | 128Mb
... | W25Q256JV*IM (DTR) | 256Mb
... | W25Q512JV*IM (DTR) | 512Mb
Adesto/Atmel | AT25SF041 | 4Mb
Spansion | S25FL208K | 8Mb

### NAND Flash

####
MFG | PART # | Size
------------ | ------------- |------------ 
Winbond  | W25N01G | 1Gb
... | W25N02G | 2Gb
... | W25M02 | 2Gb


### FRAM

####
MFG | PART # | Size
------------ | ------------- |------------ 
Cypress | CY15B108QN-40SXI | 8Mb
... | FM25V10-G | 1Mb
... | FM25V10-G rev1 | 1Mb
... | CY15B104Q-SXI | 4Mb
... | CY15B102Q-SXI | 2Mb
ROHM | MR45V100A | 1Mb
Fujitsu | MB85RS2MTAPNF | 2Mb


## USAGE

### Program Memory

NOTE:  This option is only available on the Teensy 4.0, Teensy 4.1 and Teensy Micromod boards.

Creates the filesystem using program memory.  The begin function expects a size in bytes, for the amount of the program memory you wish to use. It must be at least 65536 and smaller than the actual unused program space.  All interrupts are disabled during writing and erasing, so use of this storage does come with pretty substantial impact on interrupt latency for other libraries.  Uploading new code by Teensy Loader completely wipes the unused program space, erasing all stored files. But the filesystem persists across reboots and power cycling.

To create a disk in program memory the following constructor is used:
```
LittleFS_Program myfs;
```

In setup space is allocated int program memory by specifing space in the begin statement:
```cpp
 if (!myfs.begin(1024 * 1024 * 6)) {
    Serial.printf("Error starting %s\n", "Program flash DISK");
  }
```

### RAM Disk

A RAM disk can be setup to use space on the PSRAM chip in the case of the Teensy 4.1 or to use DMAMEM or RAM space on the Teensy itself.  If using DMAMEM or RAM the amount of available space to create these disks to going to be dependent on the Teensy model you are using so be careful.  Not to worry though it will let you know if you are out of space on compile.

To create a RAM disk  you need to first allocate the space based on the type of memory area, for the Teensy 4.1 for example you can use these settings:
1. ```EXTMEM char buf[8 * 1024 * 1024]; ```  This allocates all 8Mb of the PSRAM for the RAM disk.
2. ```DMAMEM char buf[490000]; ``` This allocates 490K of the DMAMEM
3. ```char buf[390000]; ``` This allocates 390K of RAM

Afer allocating space you specify that you want to create a RAM Disk:

```cpp
LittleFS_RAM myfs;
```

In setup you have to specify to begin using the memory area:
```cpp
 if (!myfs.begin(buf, sizeof(buf))) {
    Serial.printf("Error starting %s\n", "RAM DISK);
  } 
```
  
At this point you can access or create files in the same manner as you would with an SD Card using the SD Library bundled with Teensyduino.  See the examples section for creating and writing a file.

### QSPI

QSPI is only supported on the Teensy 4.1.  These are the Flash chips that you would solder onto the bottom side of the Teesny 4.1.  To access Flash NOR or NAND chips QSPI is similar to the RAM disk:
1. For a NAND Flash chip use the following construtor: ```LittleFS_QPINAND myfs;```
2. For a NOR flash ```LittleFS_QSPIFlash myfs; ```

And then in setup all you need for the NAND or NOR QSPI is:
```cpp
 if (!myfs.begin() {
    Serial.printf("Error starting %s\n", "QSPI");
  } 
```

### SPI
For SPI, three constructors are availble - NOR Flash, NAND Flash and FRAM
1. ```LittleFS_SPIFlash myfs;```
2. ```LittleFS_SPINAND myfs;```
3. ```LittleFS_SPIFram myfs;```

For SPI the ```begin``` statement requires the user to specify the Chip Select pin and optionally which SPI port to use:

```myfs.begin(CSpin, SPIport);```

By default the SPI port is SPI, use SPI1, SPI2 etc for other ports.

## Examples

Several examples are provided.  A simple example is as follows for a datalogger for a SPI NAND

```cpp
/*
  LittleFS  datalogger
 
 This example shows how to log data from three analog sensors
 to an storage device such as a FLASH.
 
 This example code is in the public domain.
 */

#include <LittleFS.h>

LittleFS_SPINAND  myfs;   //Specifies to use an SPI NOR Flash attached to SPI

const int chipSelect = 4;

void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect.
  }


  Serial.print("Initializing SPI FLASH...");

  // see if the Flash is present and can be initialized:
 if (!myfs.begin(chipSelect, SPI)) {
    Serial.printf("Error starting %s\n", "SPI FLASH");
    while (1) {
      // Flash error, so don't do anything more - stay stuck here
    }
  }
  Serial.println("Flash initialized.");
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file.
  File dataFile = myfs.open("datalog.txt", FILE_WRITE);
  
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  } else {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  }
  delay(100); // run at a reasonable not-too-fast speed
}
```
This is an example taked from the SD library.  Basically instead of specifying the SD library you specify to use the littleFS library on the first line as shown in the example.  Then using our methods for specifing which memory to use all we did was substitute "myfs" for where "SD" was specified before.

## [Aditional Methods](https://github.com/mjs513/LittleFS/blob/main/README1.md)

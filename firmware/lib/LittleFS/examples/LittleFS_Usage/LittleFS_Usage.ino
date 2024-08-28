/*
  LittleFS usage from the LittleFS library
  
  Starting with Teensyduino 1.54, support for LittleFS has been added.

  LittleFS is a wrapper for the LittleFS File System for the Teensy family of microprocessors and provides support for RAM Disks, NOR and NAND Flash chips, and FRAM chips. For the NOR, NAND Flash support is provided for SPI and QSPI in the case of the Teensy 4.1. For FRAM only SPI is supported. It is also linked to SDFat so many of the same commands can be used as for an SD Card.
  
  This example shows the use of some of the commands provided in LittleFS using a SPI Flash chip such as the W25Q128. 
  
  See the readme for the LittleFS library for more information: https://github.com/PaulStoffregen/LittleFS
  
*/

#include <LittleFS.h>


// Some variables for later use
uint64_t fTot, totSize1;

// To use SPI flash we need to create a instance of the library telling it to use SPI flash.
/* Other options include:
  LittleFS_QSPIFlash myfs;
  LittleFS_Program myfs;
  LittleFS_SPIFlash myfs;
  LittleFS_SPIFram myfs;
  LittleFS_SPINAND myfs;
  LittleFS_QPINAND myfs;
  LittleFS_RAM myfs;
*/
LittleFS_SPIFlash myfs;

// Since we are using SPI we need to tell the library what the chip select pin
#define chipSelect 6  // use for access flash on audio or prop shield

// Specifies that the file, file1 and file3 are File types, same as you would do for creating files
// on a SD Card
File file, file1, file2;


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial port to connect.
  }
  Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);

  Serial.print("Initializing LittleFS ...");

  // see if the Flash is present and can be initialized:
  // Note:  SPI is default so if you are using SPI and not SPI for instance
  //        you can just specify myfs.begin(chipSelect). 
  if (!myfs.begin(chipSelect, SPI)) {
    Serial.printf("Error starting %s\n", "SPI FLASH");
    while (1) {
      // Error, so don't do anything more - stay stuck here
    }
  }
  myfs.quickFormat();
  Serial.println("LittleFS initialized.");
  
  
  // To get the current space used and Filesystem size
  Serial.println("\n---------------");
  Serial.printf("Bytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
  waitforInput();

  // Now lets create a file and write some data.  Note: basically the same usage for 
  // creating and writing to a file using SD library.
  Serial.println("\n---------------");
  Serial.println("Now lets create a file with some data in it");
  Serial.println("---------------");
  char someData[128];
  memset( someData, 'z', 128 );
  file = myfs.open("bigfile.txt", FILE_WRITE);
  file.write(someData, sizeof(someData));

  for (uint16_t j = 0; j < 100; j++)
    file.write(someData, sizeof(someData));
  file.close();
  
  // We can also get the size of the file just created.  Note we have to open and 
  // thes close the file unless we do file size before we close it in the previous step
  file = myfs.open("bigfile.txt", FILE_WRITE);
  Serial.printf("File Size of bigfile.txt (bytes): %u\n", file.size());
  file.close();

  // Now that we initialized the FS and created a file lets print the directory.
  // Note:  Since we are going to be doing print directory and getting disk usuage
  // lets make it a function which can be copied and used in your own sketches.
  listFiles();
  waitforInput();
  
  // Now lets rename the file
  Serial.println("\n---------------");
  Serial.println("Rename bigfile to file10");
  myfs.rename("bigfile.txt", "file10.txt");
  listFiles();
  waitforInput();
  
  // To delete the file
  Serial.println("\n---------------");
  Serial.println("Delete file10.txt");
  myfs.remove("file10.txt");
  listFiles();
  waitforInput();

  Serial.println("\n---------------");
  Serial.println("Create a directory and a subfile");
  myfs.mkdir("structureData1");

  file = myfs.open("structureData1/temp_test.txt", FILE_WRITE);
  file.println("SOME DATA TO TEST");
  file.close();
  listFiles();
  waitforInput();

  Serial.println("\n---------------");
  Serial.println("Rename directory");
  myfs.rename("structureData1", "structuredData");
  listFiles();
  waitforInput();

  Serial.println("\n---------------");
  Serial.println("Lets remove them now...");
  //Note have to remove directories files first
  myfs.remove("structuredData/temp_test.txt");
  myfs.rmdir("structuredData");
  listFiles();
  waitforInput();

  Serial.println("\n---------------");
  Serial.println("Now lets create a file and read the data back...");
  
  // LittleFS also supports truncate function similar to SDFat. As shown in this
  // example, you can truncate files.
  //
  Serial.println();
  Serial.println("Writing to datalog.bin using LittleFS functions");
  file1 = myfs.open("datalog.bin", FILE_WRITE);
  unsigned int len = file1.size();
  Serial.print("datalog.bin started with ");
  Serial.print(len);
  Serial.println(" bytes");
  if (len > 0) {
    // reduce the file to zero if it already had data
    file1.truncate();
  }
  file1.print("Just some test data written to the file (by SdFat functions)");
  file1.write((uint8_t) 0);
  file1.close();

  // You can also use regular SD type functions, even to access the same file.  Just
  // remember to close the file before opening as a regular SD File.
  //
  Serial.println();
  Serial.println("Reading to datalog.bin using LittleFS functions");
  file2 = myfs.open("datalog.bin");
  if (file2) {
    char mybuffer[100];
    int index = 0;
    while (file2.available()) {
      char c = file2.read();
      mybuffer[index] = c;
      if (c == 0) break;  // end of string
      index = index + 1;
      if (index == 99) break; // buffer full
    }
    mybuffer[index] = 0;
    Serial.print("  Read from file: ");
    Serial.println(mybuffer);
  } else {
    Serial.println("unable to open datalog.bin :(");
  }
  file2.close();


  Serial.println("\nBasic Usage Example Finished");
}

void loop() {}

void listFiles()
{
  Serial.println("---------------");
  printDirectory(myfs);
  Serial.printf("Bytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
}

void printDirectory(FS &fs) {
  Serial.println("Directory\n---------");
  printDirectory(fs.open("/"), 0);
  Serial.println();
}

void printDirectory(File dir, int numSpaces) {
   while(true) {
     File entry = dir.openNextFile();
     if (! entry) {
       //Serial.println("** no more files **");
       break;
     }
     printSpaces(numSpaces);
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numSpaces+2);
     } else {
       // files have sizes, directories do not
       printSpaces(36 - numSpaces - strlen(entry.name()));
       Serial.print("  ");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void printSpaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(" ");
  }
}

void waitforInput()
{
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;
}

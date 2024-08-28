/*
  Sustained write speed test

  This program repeated writes 512K of random data to a flash memory chip,
  while measuring the write speed.  A blank chip will write faster than
  one filled with old data, where sectors must be erased.

  The main purpose of this example is to verify sustained write performance
  is fast enough for USB MTP.  Microsoft Windows requires at least
  87370 bytes/sec speed to avoid timeout and cancel of MTP SendObject.
    https://forum.pjrc.com/threads/68139?p=295294&viewfull=1#post295294

  This example code is in the public domain.
*/

#include <LittleFS.h>
#include <Entropy.h>

LittleFS_SPIFlash myfs;

#define chipSelect 6  // use pin 6 for access flash on audio or prop shield

void setup() {
  Entropy.Initialize();
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("LittleFS Sustained Write Speed Test");
  if (!myfs.begin(chipSelect, SPI)) {
    Serial.printf("Error starting %s\n", "SPI FLASH");
    while (1) ; // stop here
  }
  Serial.printf("Volume size %d MByte\n", myfs.totalSize() / 1048576);
}

uint64_t total_bytes_written = 0;

void loop() {
  unsigned long buf[1024];
  File myfile = myfs.open("WriteSpeedTest.bin", FILE_WRITE_BEGIN);
  if (myfile) {
    const int num_write = 128;
    Serial.printf("Writing %d byte file... ", num_write * 4096);
    randomSeed(Entropy.random());
    elapsedMillis t=0;
    for (int n=0; n < num_write; n++) {
      for (int i=0; i<1024; i++) buf[i] = random();
      myfile.write(buf, 4096);
    }
    myfile.close();
    int ms = t;
    total_bytes_written = total_bytes_written + num_write * 4096;
    Serial.printf(" %d ms, bandwidth = %d bytes/sec", ms, num_write * 4096 * 1000 / ms);
    myfs.remove("WriteSpeedTest.bin");
  }
  Serial.println();
  delay(2000);
  /* Do not write forever, possibly reducing the chip's lifespan */
  if (total_bytes_written >= myfs.totalSize() * 100) {
    Serial.println("End test, entire flash has been written 100 times");
    while (1) ; // stop here
  }
}

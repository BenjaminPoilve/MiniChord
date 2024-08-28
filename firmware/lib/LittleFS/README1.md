### Additional Methods

### Formatting

```myfs.formatUnused()``` formatUnused() can be used ahead of logging on slower media to assure best write times.  It only formats unused space on the media.  Can come in handy on large NAND chips.

```myfs.quickFormat()``` performs a quick format of the media specified

```myfs.lowLevelFormat(char, Serial Port)``` performs a low level format.  Uses the specified character, e.g, "." to show progress and is sent to the specified Serial port.

### File Operations

```file.peek()``` Return the next available byte without consuming it. (SDFat class reference)

```file.available()``` The number of bytes available from the current position to EOF 

```file.flush()``` Ensures that any bytes written to the file are physically saved

```file.truncate(bytes)``` Truncate a file to a specified length

```file.seek(position, mode)``` Sets the position in a file based on mode. There are 3 modes available:

1. SeekSet - Sets a file's position.
2. SeekCur - Set the files position to current position + pos
3. SeekEnd - Set the files position to end-of-file + pos

```file.position()``` The current position in a file

```file.size()``` The total number of bytes in a file 

### Directory Operations

```myfs.mkdir(name)``` Make a subdirectory in the volume working directory with specified name, e.g., "structureData1", must use quotes.
```myfs.remove(name)``` Remove a file from the volume working directory, e.g, "structureData1/temp_test.txt", which removes temp_test.txt in sub-directory structuredData1.
```myfs.rename(from name, to name)```  Rename a file or subdirectory. Example: ```myfs.rename("file10", "file10.txt");```
```myfs.rmdir(name)```   Remove a subdirectory from the working directory, example ```myfs.rmdir("test3")```



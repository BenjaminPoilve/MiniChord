
char szInputs[] = "0123456789RdDwcghkFqvplmusSBbyYxfan+-?";
uint32_t lastTime;
void checkInput( int step ) { // prompt for input without user input with step != 0
	uint32_t nowTime = micros();

	char retVal = 0, temp;
	char *pTemp;
	if ( step != 0 ) {
		nowTime -= lastTime;
		Serial.printf( "[%6.2f M](%6.5f M elap) Awaiting input %s loops left %d >", millis() / 60000.0, nowTime / 60000000.0, szInputs, loopLimit );
	}
	else {
		if ( !Serial.available() ) return;
		nowTime -= lastTime;
		Serial.printf( "[%6.2f M](%6.2f elap) Awaiting input %s loops left %d >", millis() / 60000.0, nowTime / 60000000.0, szInputs, loopLimit );
		//Serial.printf( "[%6.2f] Awaiting input %s loops left %d >", millis() / 60000.0, szInputs, loopLimit );
		while ( Serial.available() ) {
			temp = Serial.read( );
			if ( (pTemp = strchr(szInputs, temp)) ) {
				retVal = pTemp[0];
				parseCmd( retVal );
			}
		}
	}
	while ( !Serial.available() );
	while ( Serial.available() ) {
		temp = Serial.read();
		if ( (pTemp = strchr(szInputs, temp)) ) {
			retVal = pTemp[0];
			parseCmd( retVal );
		}
	}
	Serial.print( '\n' );
	if ( '?' == retVal ) checkInput( 1 ); // recurse on '?' to allow command show and response
	return;
}
void parseCmd( char chIn ) { // pass chIn == '?' for help
	uint32_t timeMe;
	char szNone[] = "";
	switch (chIn ) {
	case '?':
		Serial.printf( "\n%s\n", " 1-9 '#' passes continue Loop before Pause\n\
 'c, or 0': Continuous Loop, or stop Loop in progress\n\
 'h, or k': start Hundred or Thousand Loops\n\
 'd' Directory of LittleFS Media\n\
 'D' Walk all Dir Files verify Read Size\n\
 'l' Show count of loop()'s, Bytes Read,Written\n\
 'B, or b': Make Big file half of free space, or remove all Big files\n\
 'S, or s': Make 2MB file , or remove all 2MB files\n\
 \t>> Media Format : 'q' and 'F' remove ALL FS data\n\
 'q' Quick Format Disk \n\
 'F' Low Level Format Disk \n\
 'w' WIPE ALL Directories and Files\n\
 'm' Make ROOT dirs (needed after q/F format or 'w')\n\
 'f' LittleFS::formatUnused( ALL ) : DATA PRESERVED, empty blocks preformatted \n\
 'a' Auto formatUnused() during Loop - TOGGLE\n\
 'y' reclaim 1 block :: myfs.formatUnused( 1 )\n\
 'Y' reclaim 15 blocks :: myfs.formatUnused( 15 )\n\
 \t>> Test Features \n\
 'g' run SpeedBench() Media speed benchmark\n\
 'x' Directory filecount verify - TOGGLE\n\
 'v' Verbose All Dir Prints - TOGGLE\n\
 'p' Pause after all Dir prints - TOGGLE\n\
 'n' No verify on Write- TOGGLE\n\
 'u' Update Filecount\n\
 'R' Restart Teensy - Except 'RAM' - data persists\n\
 '+, or -': more, or less add .vs. delete in Loop\n\
 '?' Help list : A Loop will Create, Extend, or Delete files to verify Integrity" );
		break;
	case 'R':
		Serial.print(" RESTART Teensy ...");
		delay(100);
		SCB_AIRCR = 0x05FA0004;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		loopLimit = chIn - '0';
		if ( chIn != '0' )	// Preserve elapsed time on Error or STOP command
			lastTime = micros();
		break;
	case 'b':
		bigFile( 0 ); // delete
		chIn = 0;
		break;
	case 'B':
		lastTime = micros();
		bigFile( 1 ); // CREATE
		chIn = 0;
		break;
	case 's':
		bigFile2MB( 0 ); // CREATE
		chIn = 0;
		break;
	case 'S':
		lastTime = micros();
		bigFile2MB( 1 ); // CREATE
		chIn = 0;
		break;
	case 'c':
		loopLimit = -1;
		break;
	case 'g':
		lastTime = micros();
		speedBench();
		chIn = 0;
		break;
	case 'd':
		Serial.print( " d\n" );
		lastTime = micros();
		printDirectory();
		Serial.print( '\n' );
		parseCmd( 'l' );
		checkInput( 1 );
		chIn = 0;
		break;
	case 'D':
		Serial.println("\nWalk all Files verify Read Size:\t");
		lastTime = micros();
		Serial.printf( "\t%u Errors found\n" , DirectoryVerify( myfs.open("/") ) );
		Serial.printf("\nBytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
		parseCmd( 'l' );
		checkInput( 1 );
		chIn = 0;
		break;
	case 'w':
		Serial.println("\nWipe All Files and DIRS:");
		deleteAllDirectory(myfs.open("/"), szNone );
		errsLFS = 0; // No Errors on new Format
		warnLFS = 0; // No warning on new Format
		chIn = 0;
		parseCmd( 'u' );
		break;
	case 'h':
		loopLimit = 100;
		lastTime = micros();
		break;
	case 'k':
		loopLimit = 1000;
		lastTime = micros();
		break;
	case 'F': // Low Level format
		Serial.print( "\nFormatting Low Level:\n\t" );
		lastTime = micros();
		timeMe = micros();
		myfs.lowLevelFormat('.');
		timeMe = micros() - timeMe;
		Serial.printf( "\n Done Formatting Low Level in %lu us.\n", timeMe );
		errsLFS = 0; // No Errors on new Format
		warnLFS = 0; // No warning on new Format
		bCheckFormat  = false;
		parseCmd( 'u' );
		break;
	case 'q': // quick format
		lastTime = micros();
		myfs.quickFormat();
		errsLFS = 0; // No Errors on new Format
		parseCmd( 'u' );
		break;
	case 'v': // verbose dir
		showDir = !showDir;
		showDir ? Serial.print(" Verbose on: ") : Serial.print(" Verbose off: ");
		chIn = 0;
		break;
	case 'p': // pause on dirs
		pauseDir = !pauseDir;
		pauseDir ? Serial.print(" Pause on: ") : Serial.print(" Pause off: ");
		chIn = 0;
		break;
	case 'x': // dir filecount Verify
		bDirVerify = !bDirVerify;
		bDirVerify ? Serial.print(" FileCnt on: ") : Serial.print(" FileCnt off: ");
		lastTime = micros();
		dirVerify();
		chIn = 0;
		break;
	case 'n': // No Verify on write
		bWriteVerify = !bWriteVerify;
		bWriteVerify ? Serial.print(" Write Verify on: ") : Serial.print(" Write Verify off: ");
		chIn = 0;
		break;
	case 'a': // Auto myfs.formatUnused() during iterations
		bAutoFormat = !bAutoFormat;
		bAutoFormat ? Serial.print(" \nAuto formatUnused() On: ") : Serial.print(" \nAuto formatUnused() Off: ");
		chIn = 0;
		break;
	case 'y': // Reclaim 1 unused format
		lastTime = micros();
		Serial.printf( "\n myfs.formatUnused( 1 ) ...\n" );
		timeMe = micros();
		res = myfs.formatUnused( 1, res );
		timeMe = micros() - timeMe;
		Serial.printf( "\n\t formatUnused :: Done Formatting Low Level in %lu us (last %lu).\n", timeMe, res );
		chIn = 0;
		break;
	case 'Y': // Reclaim 15 unused format
		lastTime = micros();
		Serial.printf( "\n myfs.formatUnused( 15 ) ...\n" );
		timeMe = micros();
		res = myfs.formatUnused( 15, res );
		timeMe = micros() - timeMe;
		Serial.printf( "\n\t formatUnused :: Done Formatting Low Level in %lu us (last %lu).\n", timeMe, res );
		chIn = 0;
		break;
	case 'f': // Reclaim all unused format
		lastTime = micros();
		Serial.printf( "\n myfs.formatUnused( 0 ) ...\n" );
		timeMe = micros();
		myfs.formatUnused( 0, 0 );
		timeMe = micros() - timeMe;
		Serial.printf( "\n\t formatUnused :: Done Formatting Low Level in %lu us.\n", timeMe );
		chIn = 0;
		break;
	case 'l': // Show Loop Count
		Serial.printf("\n\t Loop Count: %u (#fileCycle=%u), Bytes read %llu, written %llu, #Files=%u\n", LoopCnt, lCnt, rdCnt, wrCnt, filecount );
		if ( 0 != errsLFS )
			Serial.printf("\t ERROR COUNT =%u\n", errsLFS );
		if ( 0 != warnLFS )
			Serial.printf("\t Free Space Warn COUNT =%u\n", warnLFS );
		dirVerify();
		chIn = 0;
		break;
	case 'm':
		Serial.printf("m \n\t Making Root Dirs\n" );
		makeRootDirs();
		parseCmd( 'd' );
		chIn = 0;
		break;
	case 'u': // Show Loop Count
		filecount = printDirectoryFilecount( myfs.open("/") );
		Serial.printf("\n\t Updated filecount %u\n", filecount );
		chIn = 0;
		break;
	case '+': // increase add cycles
		DELSTART++;
		Serial.printf("+\n Deletes start after %u cycles ", DELSTART);
		chIn = 0;
		break;
	case '-': // decrease add cycles
		DELSTART--;
		if ( DELSTART < 1 ) DELSTART = 1;
		Serial.printf("-\n Deletes start after %u cycles ", DELSTART);
		chIn = 0;
		break;
	default:
		Serial.println( chIn ); // never see without unhandled char in szInputs[]
		break;
	}
	if ( 0 != chIn ) Serial.print( chIn );
}

uint32_t loopAutoFormat( uint32_t cnt, uint32_t myres ) {
	uint32_t retres;
	retres = myfs.formatUnused( cnt, myres );
	Serial.printf("\t fmtU @ %lu - %lu \n", myres, retres );
	return retres;
}

uint32_t fTot, totSize;
void printDirectory() {
	fTot = 0, totSize = 0;
	Serial.printf("printDirectory %s\n--------------\n", szDiskMem);
	printDirectory(myfs.open("/"), 0);
	Serial.printf(" %Total %u files of Size %u Bytes\n", fTot, totSize);
	Serial.printf("Bytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
	if ( myfs.usedSize() == myfs.totalSize() ) {
		Serial.printf("\n\n\tWARNING: DISK FULL >>>>>  Bytes Used: %llu, Bytes Total:%llu\n\n", myfs.usedSize(), myfs.totalSize());
		warnLFS++;
	}
}

void deleteAllDirectory(File dir, char *fullPath ) {
	char myPath[ 256 ] = "";
	while (true) {
		File entry =  dir.openNextFile();
		if (! entry) {
			// no more files
			break;
		}
		if (entry.isDirectory()) {
			strcpy( myPath, fullPath);
			strcat( myPath, entry.name());
			strcat( myPath, "/");
			deleteAllDirectory(entry, myPath);
			entry.close();
			if ( !myfs.remove( myPath ) )
				Serial.print( "  Fail remove DIR>\t");
			else
				Serial.print( "  Removed DIR>\t");
			Serial.println( myPath );

		} else {
			strcpy( myPath, fullPath);
			strcat( myPath, entry.name());
			entry.close();
			if ( !myfs.remove( myPath ) )
				Serial.print( "\tFail remove>\t");
			else
				Serial.print( "\tRemoved>\t");
			Serial.println( myPath );
		}
	}
}

int printDirectoryFilecount(File dir) {
	unsigned int filecnt = 0;
	while (true) {
		File entry =  dir.openNextFile();
		if (! entry) {
			// no more files
			break;
		}
		if (entry.isDirectory()) {
			filecnt += printDirectoryFilecount(entry);
		} else {
			filecnt++;
		}
		entry.close();
	}
	return filecnt;
}

void printDirectory(File dir, int numTabs) {
	uint64_t fSize = 0;
	uint32_t dCnt = 0, fCnt = 0;
	if ( 0 == dir ) {
		Serial.printf( "\t>>>\t>>>>> No Dir\n" );
		return;
	}
	while (true) {
		File entry =  dir.openNextFile();
		if (! entry) {
			// no more files
			Serial.printf("\n %u dirs with %u files of Size %llu Bytes\n", dCnt, fCnt, fSize);
			fTot += fCnt;
			totSize += fSize;
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			Serial.print('\t');
		}

		if (entry.isDirectory()) {
			Serial.print("DIR\t");
			dCnt++;
		} else {
			Serial.print("FILE\t");
			fCnt++;
			fSize += entry.size();
		}
		Serial.print(entry.name());
		if (entry.isDirectory()) {
			Serial.println(" / ");
			printDirectory(entry, numTabs + 1);
		} else {
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
		//Serial.flush();
	}
}

int DirectoryVerify(File dir) {
	int errCnt = 0;
	while (true) {
		File entry =  dir.openNextFile();
		if (! entry) {
			break;
		}
		if (entry.isDirectory()) {
			Serial.print("\tD");
			Serial.print(entry.name()[0]);
			Serial.print(" ");
			errCnt += DirectoryVerify(entry);
			Serial.print("\n");
		} else {
			uint64_t fileSize, sizeCnt = 0;
			char mm;
			Serial.print(entry.name()[0]);
			fileSize = entry.size();
			while ( entry.available() ) {
				if (fileSize < sizeCnt ) break;
				entry.read( &mm , 1 );
				sizeCnt++;
			}
			if (fileSize != sizeCnt ) {
				Serial.printf("\n File Size Error:: %s found %llu Bytes for Size %llu \n", entry.name(), sizeCnt, fileSize);
				errCnt++;
				errsLFS++;
			}
		}
		entry.close();
	}
	return errCnt;
}

uint32_t cCnt = 0;
uint32_t fileCycle(const char *dir) {
	static char szFile[] = "_file.txt";
	char szPath[150];
	int ii;
	lCnt++;
	byte nNum = lCnt % MAXNUM;
	char chNow = 'A' + lCnt % MAXNUM;
	lfs_ssize_t resW = 1;
	uint32_t timeMeAll = micros();

	if ( dir[1] == 0 )	// catch root
		sprintf( szPath, "/%c%s", chNow, szFile );
	else
		sprintf( szPath, "%s/%c%s", dir, chNow, szFile );
	if ( cCnt >= DELSTART && myfs.exists(szPath) ) { // DELETE ALL KNOWN FILES
		if ( nNum == 1 ) {
			Serial.print( "\n == == ==   DELETE PASS START  == == == = \n");
			if ( showDir ) {
				printDirectory();
				Serial.print( " == == ==   DELETE PASS START  == == == = \n");
			}
			delayMicroseconds(DELDELAY);
		}
	}
	Serial.printf( ":: %s ", szPath );
	if ( cCnt >= DELSTART && myfs.exists(szPath) ) { // DELETE ALL KNOWN FILES
		readVerify( szPath, chNow );
		myfs.remove(szPath);
		filecount--;
		Serial.printf(" ----DEL----");
		Serial.printf(" -- %c", chNow);
		if ( showDir ) {
			Serial.print("\n");
			printDirectory(myfs.open(dir), 1);
		}
		if ( pauseDir ) checkInput( 1 );
		Serial.println();
	}
	else {
		if ( bWriteVerify && ( myfs.totalSize() - myfs.usedSize() ) < MAXFILL ) {
			warnLFS++;
			Serial.printf( "\tXXX\tXXX\tXXX\tXXX\tSIZE WARNING { MAXFILL } \n" );
			cCnt = DELSTART;
			return cCnt;	// EARLY EXIT
		}
		if ( nNum == 0 ) {
			nNum = 10;
			cCnt++;
			if ( cCnt >= DELSTART + 2 ) cCnt = 0;
		}
		file3 = myfs.open(szPath, FILE_WRITE);
		if ( 0 == file3 ) {
			Serial.printf( "\tXXX\tXXX\tXXX\tXXX\tFail File open {mkdir?}\n" );
			delayMicroseconds(300000);
			checkInput( 1 );	// PAUSE on CmdLine
		}
		else {
			delayMicroseconds(ADDDELAY);
			char mm = chNow + lowOffset;
			uint64_t jj = file3.size() + 1;
			uint32_t timeMe = micros();
			int32_t nn = nNum * SUBADD + BIGADD;
			for ( ii = 0; ii < nn && resW >= 0; ii++ ) {
				if ( 0 == ((ii + jj) / lowShift) % 2 )
					resW = file3.write( &mm , 1 );
				else
					resW = file3.write( &chNow , 1 );
				wrCnt++;
				// if ( lCnt%100 == 50 ) mm='x'; // GENERATE ERROR to detect on DELETE read verify
			}
			file3.close();
			timeMe = micros() - timeMe;
			timeMeAll = micros() - timeMeAll;
			Serial.printf(" +++ size %llu: Add %d @KB/sec %5.2f ",  jj - 1, nn, ii / (timeMeAll / 1000.0) );
			if (resW < 0) {
				Serial.printf( "\n\twrite fail ERR# %i 0x%X \n", resW, resW );
				parseCmd( '0' );
				errsLFS++;
				checkInput( 1 );	// PAUSE on CmdLine
			}
			else if ( jj == 1 ) filecount++; // File Added
			Serial.print("\t");
			if ( bWriteVerify )
				readVerify( szPath, chNow );
			Serial.print('\n');
			if ( showDir ) {
				Serial.print('\n');
				printDirectory(myfs.open(dir), 1);
			}
		}
		if ( pauseDir ) checkInput( 1 );
		//Serial.print("\n");
		delayMicroseconds(ADDDELAY);
	}
	checkInput( 0 ); // user stop request?
	if ( bDirVerify ) dirVerify();
	return cCnt;
}

void dirVerify() {
	uint32_t timeMe = micros();
	Serial.printf("\tdirV...");
	if ( filecount != printDirectoryFilecount( myfs.open("/") ) ) {
		Serial.printf( "\tFilecount mismatch %u != %u\n", filecount, printDirectoryFilecount( myfs.open("/") ) );
		parseCmd( '0' );
		errsLFS++;
		checkInput( 1 );	// PAUSE on CmdLine
	}
	timeMe = micros() - timeMe;
	Serial.printf("%lu_us\t", timeMe);
}

void readVerify( char szPath[], char chNow ) {
	uint32_t timeMe = micros();
	file3 = myfs.open(szPath);
	if ( 0 == file3 ) {
		Serial.printf( "\tV\t Fail File open %s\n", szPath );
		parseCmd( '0' );
		errsLFS++;
		checkInput( 1 );
	}
	char mm;
	char chNow2 = chNow + lowOffset;
	uint32_t ii = 0;
	while ( file3.available() ) {
		file3.read( &mm , 1 );
		rdCnt++;
		//Serial.print( mm ); // show chars as read
		ii++;
		if ( 0 == (ii / lowShift) % 2 ) {
			if ( chNow2 != mm ) {
				Serial.printf( "<Bad Byte!  %c! = %c [0x%X] @%u\n", chNow2, mm, mm, ii );
				parseCmd( '0' );
				errsLFS++;
				checkInput( 1 );
				break;
			}
		}
		else {
			if ( chNow != mm ) {
				Serial.printf( "<Bad Byte!  %c! = %c [0x%X] @%u\n", chNow, mm, mm, ii );
				parseCmd( '0' );
				errsLFS++;
				checkInput( 1 );
				break;
			}
		}
	}
	Serial.printf( "  Verify %u Bytes ", ii );
	if (ii != file3.size()) {
		Serial.printf( "\n\tRead Count fail! :: read %u != f.size %llu", ii, file3.size() );
		parseCmd( '0' );
		errsLFS++;
		checkInput( 1 );	// PAUSE on CmdLine
	}
	file3.close();
	timeMe = micros() - timeMe;
	Serial.printf( " @KB/sec %5.2f", ii / (timeMe / 1000.0) );
}

bool bigVerify( char szPath[], char chNow ) {
	uint32_t timeMe = micros();
	file3 = myfs.open(szPath);
	uint64_t fSize;
	if ( 0 == file3 ) {
		return false;
	}
	char mm;
	uint32_t ii = 0;
	uint32_t kk = file3.size() / 50;
	fSize = file3.size();
	Serial.printf( "\tVerify %s bytes %lu : ", szPath, fSize );
	while ( file3.available() ) {
		file3.read( &mm , 1 );
		rdCnt++;
		ii++;
		if ( !(ii % kk) ) Serial.print('.');
		if ( chNow != mm ) {
			Serial.printf( "<Bad Byte!  %c! = %c [0x%X] @%u\n", chNow, mm, mm, ii );
			parseCmd( '0' );
			errsLFS++;
			checkInput( 1 );
			break;
		}
		if ( ii > fSize ) { // catch over length return makes bad loop !!!
			Serial.printf( "\n\tFile LEN Corrupt!  FS returning over %u bytes\n", fSize );
			parseCmd( '0' );
			errsLFS++;
			checkInput( 1 );
			break;
		}
	}
	if (ii != file3.size()) {
		Serial.printf( "\n\tRead Count fail! :: read %u != f.size %llu\n", ii, file3.size() );
		parseCmd( '0' );
		errsLFS++;
		checkInput( 1 );	// PAUSE on CmdLine
	}
	else
		Serial.printf( "\tGOOD! >>  bytes %lu", ii );
	file3.close();
	timeMe = micros() - timeMe;
	Serial.printf( "\n\tBig read&compare KBytes per second %5.2f \n", ii / (timeMe / 1000.0) );
	if ( 0 == ii ) return false;
	return true;
}


void bigFile( int doThis ) {
	char myFile[] = "/0_bigfile.txt";
	char fileID = '0' - 1;

	if ( 0 == doThis ) {	// delete File
		Serial.printf( "\nDelete with read verify all #bigfile's\n");
		do {
			fileID++;
			myFile[1] = fileID;
			if ( myfs.exists(myFile) && bigVerify( myFile, fileID) ) {
				filecount--;
				myfs.remove(myFile);
			}
			else break; // no more of these
		} while ( 1 );
	}
	else {	// FILL DISK
		lfs_ssize_t resW = 1;
		char someData[2048];
		uint32_t xx, toWrite;
		toWrite = (myfs.totalSize()) - myfs.usedSize();
		if ( toWrite < 65535 ) {
			Serial.print( "Disk too full! DO :: b or q or F");
			return;
		}
		toWrite -= SLACK_SPACE;
		toWrite /= 2;
		xx = toWrite;
		Serial.printf( "\nStart Big write of %u Bytes", xx);
		uint32_t timeMe = micros();
		file3 = nullptr;
		do {
			if ( file3 ) file3.close();
			fileID++;
			myFile[1] = fileID;
			file3 = myfs.open(myFile, FILE_WRITE);
		} while ( fileID < '9' && file3.size() > 0);
		if ( fileID == '9' ) {
			Serial.print( "Disk has 9 halves 0-8! DO :: b or q or F");
			return;
		}
		memset( someData, fileID, 2048 );
		int hh = 0;
		while ( toWrite > 2048 && resW > 0 ) {
			resW = file3.write( someData , 2048 );
			hh++;
			if ( !(hh % 40) ) Serial.print('.');
			toWrite -= 2048;
		}
		xx -= toWrite;
		file3.close();
		timeMe = micros() - timeMe;
		file3 = myfs.open(myFile, FILE_WRITE);
		if ( file3.size() > 0 ) {
			filecount++;
			Serial.printf( "\nBig write %s took %5.2f Sec for %lu Bytes : file3.size()=%llu", myFile , timeMe / 1000000.0, xx, file3.size() );
		}
		if ( file3 != 0 ) file3.close();
		Serial.printf( "\n\tBig write KBytes per second %5.2f \n", xx / (timeMe / 1000.0) );
		Serial.printf("\nBytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
		if ( myfs.usedSize() == myfs.totalSize() ) {
			Serial.printf("\n\n\tWARNING: DISK FULL >>>>>  Bytes Used: %llu, Bytes Total:%llu\n\n", myfs.usedSize(), myfs.totalSize());
			warnLFS++;
		}
		if ( resW < 0 ) {
			Serial.printf( "\nBig write ERR# %i 0x%X \n", resW, resW );
			errsLFS++;
			myfs.remove(myFile);
		}
	}
}

void bigFile2MB( int doThis ) {
	char myFile[] = "/0_2MBfile.txt";
	char fileID = '0' - 1;

	if ( 0 == doThis ) {	// delete File
		Serial.printf( "\nDelete with read verify all #bigfile's\n");
		do {
			fileID++;
			myFile[1] = fileID;
			if ( myfs.exists(myFile) && bigVerify( myFile, fileID) ) {
				filecount--;
				myfs.remove(myFile);
			}
			else break; // no more of these
		} while ( 1 );
	}
	else {	// FILL DISK
		lfs_ssize_t resW = 1;
		char someData[2048];
		uint32_t xx, toWrite;
		toWrite = 2048 * 1000;
		if ( toWrite > (65535 + (myfs.totalSize() - myfs.usedSize()) ) ) {
			Serial.print( "Disk too full! DO :: q or F");
			return;
		}
		xx = toWrite;
		Serial.printf( "\nStart Big write of %u Bytes", xx);
		uint32_t timeMe = micros();
		file3 = nullptr;
		do {
			if ( file3 ) file3.close();
			fileID++;
			myFile[1] = fileID;
			file3 = myfs.open(myFile, FILE_WRITE);
		} while ( fileID < '9' && file3.size() > 0);
		if ( fileID == '9' ) {
			Serial.print( "Disk has 9 files 0-8! DO :: b or q or F");
			return;
		}
		memset( someData, fileID, 2048 );
		int hh = 0;
		while ( toWrite >= 2048 && resW > 0 ) {
			resW = file3.write( someData , 2048 );
			hh++;
			if ( !(hh % 40) ) Serial.print('.');
			toWrite -= 2048;
		}
		xx -= toWrite;
		file3.close();
		timeMe = micros() - timeMe;
		file3 = myfs.open(myFile, FILE_WRITE);
		if ( file3.size() > 0 ) {
			filecount++;
			Serial.printf( "\nBig write %s took %5.2f Sec for %lu Bytes : file3.size()=%llu", myFile , timeMe / 1000000.0, xx, file3.size() );
		}
		if ( file3 != 0 ) file3.close();
		Serial.printf( "\n\tBig write KBytes per second %5.2f \n", xx / (timeMe / 1000.0) );
		Serial.printf("\nBytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
		if ( myfs.usedSize() == myfs.totalSize() ) {
			Serial.printf("\n\n\tWARNING: DISK FULL >>>>>  Bytes Used: %llu, Bytes Total:%llu\n\n", myfs.usedSize(), myfs.totalSize());
			warnLFS++;
		}
		if ( resW < 0 ) {
			Serial.printf( "\nBig write ERR# %i 0x%X \n", resW, resW );
			errsLFS++;
			myfs.remove(myFile);
		}
	}
}

void makeRootDirs() {
	char szDir[16];
	for ( uint32_t ii = 1; ii <= NUMDIRS; ii++ ) {
		sprintf( szDir, "/%lu_dir", ii );
		myfs.mkdir( szDir );
	}
}

// for void speedBench()
#include <sdios.h>
ArduinoOutStream cout(Serial);
File file;

// File size in bytes.
const uint32_t FILE_SIZE = 16 * 1024;

// Set SKIP_FIRST_LATENCY true if the first read/write to the SD can
// be avoid by writing a file header or reading the first record.
const bool SKIP_FIRST_LATENCY = true;

// Size of read/write.
const size_t BUF_SIZE = 2048;

// Write pass count.
const uint8_t WRITE_COUNT = 5;

// Read pass count.
const uint8_t READ_COUNT = 5;

//Block size for qspi
#define MYBLKSIZE 2048 // 2048

// Insure 4-byte alignment.
uint32_t buf32[(BUF_SIZE + 3) / 4];
uint8_t* bufA32 = (uint8_t*)buf32;

//Number of random reads
#define randomReads 1

void speedBench() {
	File file;
	float s;
	uint32_t t;
	uint32_t maxLatency;
	uint32_t minLatency;
	uint32_t totalLatency;
	bool skipLatency;

	myfs.remove("bench.dat");
	//for(uint8_t cnt=0; cnt < 10; cnt++) {

	// fill buf with known data
	if (BUF_SIZE > 1) {
		for (size_t i = 0; i < (BUF_SIZE - 2); i++) {
			bufA32[i] = 'A' + (i % 26);
		}
		bufA32[BUF_SIZE - 2] = '\r';
	}
	bufA32[BUF_SIZE - 1] = '\n';

	Serial.printf("%s Disk Stats:", szDiskMem );
	Serial.printf("Bytes Used: %llu, Bytes Total:%llu\n", myfs.usedSize(), myfs.totalSize());
	Serial.printf("%s Benchmark:\n", szDiskMem );
	cout << F("FILE_SIZE = ") << FILE_SIZE << endl;
	cout << F("BUF_SIZE = ") << BUF_SIZE << F(" bytes\n");
	cout << F("Starting write test, please wait.") << endl << endl;

	// do write test
	uint32_t n = FILE_SIZE / BUF_SIZE;
	cout << F("write speed and latency") << endl;
	cout << F("speed,max,min,avg") << endl;
	cout << F("KB/Sec,usec,usec,usec") << endl;

	// open or create file - truncate existing file.
	file = myfs.open("bench.dat", FILE_WRITE);

	for (uint8_t nTest = 0; nTest < WRITE_COUNT; nTest++) {
		file.seek(0);

		maxLatency = 0;
		minLatency = 9999999;
		totalLatency = 0;
		skipLatency = SKIP_FIRST_LATENCY;
		t = millis();

		for (uint32_t i = 0; i < n; i++) {
			uint32_t m = micros();
			if (file.write(bufA32, BUF_SIZE) != BUF_SIZE) {
				Serial.println("write failed");
			}
			m = micros() - m;
			totalLatency += m;
			if (skipLatency) {
				// Wait until first write to SD, not just a copy to the cache.
				skipLatency = file.position() < 512;
			} else {
				if (maxLatency < m) {
					maxLatency = m;
				}
				if (minLatency > m) {
					minLatency = m;
				}
			}
		}

		t = millis() - t;
		s = file.size();
		cout << s / t << ',' << maxLatency << ',' << minLatency;
		cout << ',' << totalLatency / n << endl;
	}
	cout << endl << F("Starting sequential read test, please wait.") << endl;
	cout << endl << F("read speed and latency") << endl;
	cout << F("speed,max,min,avg") << endl;
	cout << F("KB/Sec,usec,usec,usec") << endl;

	// do read test
	for (uint8_t nTest = 0; nTest < READ_COUNT; nTest++) {
		file.seek(0);
		maxLatency = 0;
		minLatency = 9999999;
		totalLatency = 0;
		skipLatency = SKIP_FIRST_LATENCY;
		t = micros();
		for (uint32_t i = 0; i < n; i++) {
			bufA32[BUF_SIZE - 1] = 0;
			uint32_t m = micros();
			int32_t nr = file.read(bufA32, BUF_SIZE);
			if (nr != BUF_SIZE) {
				Serial.println("read failed");
			}
			m = micros() - m;
			totalLatency += m;
			if (bufA32[BUF_SIZE - 1] != '\n') {
				Serial.println("data check error");
			}
			if (skipLatency) {
				skipLatency = false;
			} else {
				if (maxLatency < m) {
					maxLatency = m;
				}
				if (minLatency > m) {
					minLatency = m;
				}
			}
		}

		s = file.size();

		t = micros() - t;
		cout << s * 1000 / t << ',' << maxLatency << ',' << minLatency;
		cout << ',' << totalLatency / n << endl;
	}

	cout << endl << F("Done") << endl;

	cout << endl << F("Starting random read test, please wait.") << endl;

	Serial.printf("Number of random reads: %d\n", randomReads);
	Serial.printf("Number of blocks: %d\n", n);

	cout << endl << F("read speed and latency") << endl;
	cout << F("speed,max,min,avg") << endl;

	// do read test
	for (uint8_t nTest = 0; nTest < READ_COUNT; nTest++) {
		file.seek(0);
		maxLatency = 0;
		minLatency = 0;
		totalLatency = 0;
		skipLatency = SKIP_FIRST_LATENCY;
		t = micros();
		for (uint32_t i = 0; i < randomReads; i++) {
			bufA32[BUF_SIZE - 1] = 0;
			uint32_t m = micros();
			uint32_t block_pos = random(0, (n - 1));
			uint32_t random_pos = block_pos * MYBLKSIZE;
			cout << "Position (bytes), Block: " << random_pos << ", ";
			cout << block_pos << endl;
			uint32_t startCNT = ARM_DWT_CYCCNT;
			file.seek(random_pos);
			int32_t nr = file.read(bufA32, BUF_SIZE);
			uint32_t endCNT = ARM_DWT_CYCCNT;
			cout << F("Read Time (ARM Cycle Delta): ") << endCNT - startCNT << endl;
			if (nr != BUF_SIZE) {
				Serial.println("read failed");
			}
			m = micros() - m;
			totalLatency += m;
			if (bufA32[BUF_SIZE - 1] != '\n') {
				Serial.println("data check error");
			}
			if (skipLatency) {
				skipLatency = false;
			} else {
				if (maxLatency < m) {
					maxLatency = m;
				}
				if (minLatency > m) {
					minLatency = m;
				}
			}
		}

		s = file.size();


		t = micros() - t;
		cout << F("KB/Sec,usec,usec,usec") << endl;
		cout << s * 1000 / t << ',' << maxLatency << ',' << minLatency;
		cout << ',' << totalLatency / n << endl;
	}
	cout << endl << F("Done") << endl;

	file.close();
	myfs.remove("bench.dat");

}

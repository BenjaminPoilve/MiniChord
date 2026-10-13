# Firmware 

Note to reader: a lot of this firmware has been developed in 45-minute time increment after a day's work, which in part explains that the code quality is not great. Refactoring and making the code cleaner will be a future focus. 

If you are an experimented C/C++ dev, and you whish to help on this project, I might be able to send you pre-production sample of the minichord in exchange for a bit of help. Don't hesitate to reach out to minichord@benjaminpoilve.com .

### Structure 

This folder contains a PlatformIO project for Teensy 4.0. 

In particular the it contains :

- the `src`folder containing the `main.cpp` file, as well as the file defining the USB name of the device, `name.c`
- the `lib` folder containing the librairies for the chips used, the debouncing and the potentiometer logic 
- the `include` folder containing all the Teensy audio architecture definition, as well as the hardware definitions and the `sysex_handler.h`file. 

The next interesting folder is related to the "generator". To be able to simply modify parameters related to sound synthesis and have a coherent firmware and control software, those parameters are defined in a [parameters.json file](https://github.com/BenjaminPoilve/MiniChord/blob/main/firmware/generator/parameters.json). By using the generation script, both the interface and necessary firmware file are generated. Note that the interface will be included in the minichord website by using the `build_site.sh` script in the documentation folder. 

The resulting fimware itself is present at the root of the project : [firmware.hex](https://github.com/BenjaminPoilve/MiniChord/blob/main/firmware/firmware.hex).

### Usage 

If you want to set-up a PlatformIO environnement for Teensy, please follow the tutorial for PlatformIO Teensy usage [available here](https://forum.pjrc.com/index.php?threads/tutorial-how-to-use-platformio-visual-code-studio-for-teensy.66674/)

Once the project is set-up, you can modify, compile and upload the code directly from VSCode. 

To use the generator, set-up a venv in the generator directory using `python3 -m venv .venv`. Then enter the venv by using `source venv/bin/activate`. 

Once in the venv, install the necessary packages by using `pip3 install -r requirements.txt`. 

Generation can then simply be done by running the `generate.py`script: `python3 generate.py`.

### USB descriptor customisation

The device uses the stock `USB_MIDI16_AUDIO_SERIAL` USB type of the Teensy core, with three changes: it advertises 2 MIDI cables instead of 16, it reports `minichord` as manufacturer and product name, and it offers no USB speaker. The minichord's sound still goes to the host for recording, but a phone or computer no longer sends its own sound to the minichord, where nothing would play it.

The USB audio recording tap in `include/audio_definition.h` is guarded by `#ifdef AUDIO_INTERFACE`, so building against a USB type without audio interfaces still works without further edits.

All three are done from within the project, so no file of the `framework-arduinoteensy` package has to be touched:

- the names are set in [src/name.c](src/name.c), which overrides the (weakly defined) `usb_string_manufacturer_name` and `usb_string_product_name` descriptors of the core;
- the cable count is set in [include/usb_desc_override.h](include/usb_desc_override.h), which `platformio.ini` force-includes in front of every translation unit with `-include`. Because the core's `usb_desc.h` uses `#pragma once`, reading it from that header first means the core sources later get a no-op include and keep our value of `MIDI_NUM_CABLES`;
- the speaker is removed by [tools/usb_no_speaker.py](tools/usb_no_speaker.py), a PlatformIO pre-script that compiles a copy of the core's `usb_desc.c`, without the speaker's descriptors, in place of the original. If the core's file ever stops reading as the script expects, the build stops.

Earlier versions of this project instead asked for a manual edit of `usb_desc.h` in `~/.platformio/packages/framework-arduinoteensy/cores/teensy4`. That is no longer needed, and if you patched that file previously you can restore it to its original content.

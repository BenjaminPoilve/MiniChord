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

### Caveat 

To get the midi interface to display the right number of cables, the `usb_desc.h` file in the `~/.platformio/packages/framework-arduinoteensy/cores/teensy4` folder needs to be modified to the following:

```
#elif defined(USB_MIDI16_AUDIO_SERIAL)
  #define VENDOR_ID		0x16C0
  #define PRODUCT_ID		0x048A
  #define BCD_DEVICE		0x0212
  #define MANUFACTURER_NAME	{'m','i','n','i','c','h','o','r','d'}
  #define MANUFACTURER_NAME_LEN	9
  #define PRODUCT_NAME		{'m','i','n','i','c','h','o','r','d'}
  #define PRODUCT_NAME_LEN	9
  #define EP0_SIZE		64
  #define NUM_ENDPOINTS         8
  #define NUM_INTERFACE		6
  #define CDC_IAD_DESCRIPTOR	1
  #define CDC_STATUS_INTERFACE	0
  #define CDC_DATA_INTERFACE	1	// Serial
  #define CDC_ACM_ENDPOINT	2
  #define CDC_RX_ENDPOINT       3
  #define CDC_TX_ENDPOINT       3
  #define CDC_ACM_SIZE          16
  #define CDC_RX_SIZE_480       512
  #define CDC_TX_SIZE_480       512
  #define CDC_RX_SIZE_12        64
  #define CDC_TX_SIZE_12        64
  #define MIDI_INTERFACE        2	// MIDI
  #define MIDI_NUM_CABLES       2
  #define MIDI_TX_ENDPOINT      4
  #define MIDI_TX_SIZE_12       64
  #define MIDI_TX_SIZE_480      512
  #define MIDI_RX_ENDPOINT      4
  #define MIDI_RX_SIZE_12       64
  #define MIDI_RX_SIZE_480      512
  #define AUDIO_INTERFACE	3	// Audio (uses 3 consecutive interfaces)
  #define AUDIO_TX_ENDPOINT     5
  #define AUDIO_TX_SIZE         180
  #define AUDIO_RX_ENDPOINT     5
  #define AUDIO_RX_SIZE         180
  #define AUDIO_SYNC_ENDPOINT	6
  #define ENDPOINT2_CONFIG	ENDPOINT_RECEIVE_UNUSED + ENDPOINT_TRANSMIT_INTERRUPT
  #define ENDPOINT3_CONFIG	ENDPOINT_RECEIVE_BULK + ENDPOINT_TRANSMIT_BULK
  #define ENDPOINT4_CONFIG	ENDPOINT_RECEIVE_BULK + ENDPOINT_TRANSMIT_BULK
  #define ENDPOINT5_CONFIG	ENDPOINT_RECEIVE_ISOCHRONOUS + ENDPOINT_TRANSMIT_ISOCHRONOUS
  #define ENDPOINT6_CONFIG	ENDPOINT_RECEIVE_UNUSED + ENDPOINT_TRANSMIT_ISOCHRONOUS
```

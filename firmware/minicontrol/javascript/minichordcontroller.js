// MiniChord Controller - Handles MIDI communication
class MiniChordController {
    constructor() {
      this.device = false;
      this.parameter_size = 256;
      // The knob memories and the volumes are put to the middle once a connection, on the first dump,
      // so presets saved from here keep them there; not on every dump, or a preset change, a pop or a
      // refresh would snap the knobs' alternate functions back to the middle while playing.
      this.knobs_centred = false;
      this.color_hue_sysex_adress = 20;
      this.base_adress_rythm = 220;
      this.potentiometer_memory_adress = [4, 5, 6];
      this.modulation_adress = [14, 10, 12,16];
      this.volume_memory_adress = [2, 3];
      this.active_bank_number = -1;
      this.min_firmware_accepted = 0.02;
      this.firmware_adress = 7;
      this.float_multiplier = 100.0;
      this.MIDI_request_option = {
        sysex: true,
        software: false
      };
      this.onConnectionChange = null;
      this.onDataReceived = null;
      this.json_reference="../json/minichord.json";
    }
  
    // Initialize MIDI connection
    async initialize() {
      try {
        console.log(">> Requesting MIDI access");
        const midiAccess = await navigator.requestMIDIAccess(this.MIDI_request_option);
        console.log(">> MIDI access granted");
        this.handleMIDIAccess(midiAccess);
        midiAccess.onstatechange = (event) => this.handleStateChange(event);
        return true;
      } catch (error) {
        console.log(">> ERROR: MIDI access failed");
        console.error(error);
        return false;
      }
    }
  
    // Handle MIDI access
    handleMIDIAccess(midiAccess) {
      console.log(">> Available outputs:");
      for (const entry of midiAccess.outputs) {
        const output = entry[1];
        if (output.name.includes("minichord") && output.name.includes("1") || output.name == "minichord") {
          console.log(
            `>>>> minichord sysex control port [type:'${output.type}'] id: '${output.id}' manufacturer: '${output.manufacturer}' name: '${output.name}' version: '${output.version}'`
          );
          this.device = output;
          this.knobs_centred = false;
          const sysex_message = [0xF0, 0, 0, 0, 0, 0xF7];
          this.device.send(sysex_message);
        } else {
          console.log(
            `>>>> Other port [type:'${output.type}'] id: '${output.id}' manufacturer: '${output.manufacturer}' name: '${output.name}' version: '${output.version}'`
          );
        }
      }
      console.log(">> Available inputs:");
      for (const entry of midiAccess.inputs) {
        const input = entry[1];
        if (input.name.includes("minichord") && input.name.includes("1") || input.name == "minichord") {
          input.onmidimessage = (message) => this.processCurrentData(message);
          console.log(
            `>>>> minichord sysex control port [type:'${input.type}'] id: '${input.id}' manufacturer: '${input.manufacturer}' name: '${input.name}' version: '${input.version}'`
          );
        } else {
          console.log(
            `>>>> Other port [type:'${input.type}'] id: '${input.id}' manufacturer: '${input.manufacturer}' name: '${input.name}' version: '${input.version}'`
          );
        }
      }
      if (this.device == false) {
        console.log(">> ERROR: no minichord device found");
        if (this.onConnectionChange) {
          this.onConnectionChange(false, "Make sure the minichord is connected to the computer and turned on");
        }
      } else {
        console.log(">> minichord succesfully connected");
      }
    }
  
    // Handle MIDI state changes
    handleStateChange(event) {
      console.log(">> MIDI state change received");
      console.log(event);
      if (event.port.state == "disconnected" && this.device != false && (event.port.name == "minichord Port 1" || event.port.name == "minichord")) {
        console.log(">> minichord was disconnected");
        this.device = false;
        if (this.onConnectionChange) {
          this.onConnectionChange(false, "> minichord disconnected, please reconnect");
        }
      }
      if (event.port.state == "connected" && this.device == false && (event.port.name == "minichord Port 1" || event.port.name == "minichord")) {
        console.log(">> a new device was connected");
        this.handleMIDIAccess(event.target);
      }
    }
  
    // Process incoming MIDI data
    processCurrentData(midiMessage) {
      const data = midiMessage.data.slice(1);
      if (data.length != this.parameter_size * 2 + 1) {
        console.log(">> Non-sysex message received, ignoring");
      } else {
        const processedData = {
          parameters: [],
          rhythmData: [],
          bankNumber: data[2 * 1],
          firmwareVersion: 0
        };
        
        for (var i = 2; i < this.parameter_size; i++) {
          const sysex_value = data[2 * i] + 128 * data[2 * i + 1];
          if (i == this.firmware_adress) {
            processedData.firmwareVersion = sysex_value;
            if (processedData.firmwareVersion < this.min_firmware_accepted) {
              alert("Please update the minichord firmware");
            }
          } else if (i < this.base_adress_rythm + 16 && i > this.base_adress_rythm - 1) {
            const j = i - this.base_adress_rythm;
            const rhythmBits = [];
            for (var k = 0; k < 7; k++) {
              rhythmBits[k] = !!(sysex_value & (1 << k));
            }
            processedData.rhythmData[j] = rhythmBits;
          } else {
            processedData.parameters[i] = sysex_value;
          }
        }
        
        this.active_bank_number = processedData.bankNumber;
        
        // Override potentiometer and volume values, once a connection
        if (!this.knobs_centred) {
          this.knobs_centred = true;
          for (const i of this.potentiometer_memory_adress) {
            this.sendParameter(i, 512);
            processedData.parameters[i] = 512;
          }

          for (const i of this.volume_memory_adress) {
            this.sendParameter(i, 0.5 * 100);
            processedData.parameters[i] = 0.5 * 100;
          }
        }
        
        if (this.onDataReceived) {
          this.onDataReceived(processedData);
        }
      }
    }
  
    // Send parameter to device
    sendParameter(address, value) {
      if (!this.device) return false;
      const first_byte = parseInt(value % 128);
      const second_byte = parseInt(value / 128);
      const first_byte_address = parseInt(address % 128);
      const second_byte_address = parseInt(address / 128);
      const sysex_message = [0xF0, first_byte_address, second_byte_address, first_byte, second_byte, 0xF7];
      this.device.send(sysex_message);
      return true;
    }
  
    // Reset memory
    resetMemory() {
      if (!this.device) return false;
      const sysex_message = [0xF0, 0, 0, 1, 0, 0xF7];
      this.device.send(sysex_message);
      return true;
    }
  
    // Save current settings
    saveCurrentSettings(bankNumber) {
      if (!this.device) return false;
      const sysex_message = [0xF0, 0, 0, 2, bankNumber, 0xF7];
      this.device.send(sysex_message);
      return true;
    }
  
    // Reset current bank
    resetCurrentBank() {
      if (!this.device || this.active_bank_number == -1) return false;
      const sysex_message = [0xF0, 0, 0, 3, this.active_bank_number, 0xF7];
      this.device.send(sysex_message);
      return true;
    }
  
    // Check if device is connected
    isConnected() {
      return this.device !== false;
    }
  
    // Get device info
    getDeviceInfo() {
      return {
        connected: this.isConnected(),
        activeBankNumber: this.active_bank_number,
        parameterSize: this.parameter_size,
        colorHueAddress: this.color_hue_sysex_adress,
        baseAddressRhythm: this.base_adress_rythm,
        floatMultiplier: this.float_multiplier
      };
    }

    
  }
  
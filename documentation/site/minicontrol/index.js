minichord_device = false;
var parameter_size = 256;
var color_hue_sysex_adress = 20;
var base_adress_rythm = 220;
var potentiometer_memory_adress = [4, 5, 6];

//-->>UTILITIES
function map_value(value, in_min, in_max, out_min, out_max) {
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + Number(out_min);
}

//-->>INTERFACE HANDLER
//Slider Handler
function handlechange(event) {
  if (minichord_device) {
    curve_type = event.getAttribute("curve")
    if (curve_type == "exponential") {
      range_value = Math.exp((Math.log(event.max) / event.max) * event.value);
    } else {
      range_value = event.value
    }
    if (event.getAttribute("data_type") == "int") {
      displayed_value = map_value(range_value, event.min, event.max, event.getAttribute("target_min"), event.getAttribute("target_max")).toFixed(0)
    } else if (event.getAttribute("data_type") == "float") {
      displayed_value = map_value(range_value, event.min, event.max, event.getAttribute("target_min"), event.getAttribute("target_max")).toFixed(2)
      range_value = Math.round(range_value * 100);
    }
    adress = event.getAttribute("adress_field")
    var value_zone = document.getElementById("value_zone" + event.getAttribute("adress_field"));
    value_zone.innerHTML = displayed_value
    send_parameter(minichord_device, adress, Math.round(range_value));
    if (adress == color_hue_sysex_adress) { //handling the color scheme
      var hue = Math.round(range_value);
      console.log('hsl(' + hue + ',100%,50%)');
      //document.body.style.color = 'hsl(' + hue + ',100%,50%)';
      var elements = document.getElementsByClassName('slider');
      for (let i = 0; i < elements.length; i++) {
        elements[i].style.setProperty('--slider_color', 'hsl(' + hue + ',100%,50%)')
      }

    }
  } else {
    event.value = event.getAttribute("value");
    document.getElementById("information_zone").focus();
  }
}

function checkbox_array() {
  var container
  for (var i = base_adress_rythm; i < base_adress_rythm + 16; i++) {
    container = document.getElementById(i);
    container.innerHTML = "";
    container.style.display = 'block';
  }
  for (var j = 0; j < 7; j++) {
    var line = document.createElement("div");
    container.appendChild(line);

    for (var i = 0; i < 16; i++) {
      var checkBox = document.createElement("input");
      checkBox.type = "checkbox";
      checkBox.id = "checkbox" + j + i;

      line.appendChild(checkBox);
    }
  }
  var button = document.createElement("input");
  button.type = "button";
  button.value = "Send";
  button.onclick = send_array_data;
  container.appendChild(button);
}
checkbox_array();
function send_array_data(event) {
  for (var i = 0; i < 16; i++) {
    var output_value = 0
    for (var j = 0; j < 7; j++) {
      var checkbox = document.getElementById("checkbox" + j + i);
      if (null != checkbox) {
        output_value = output_value | checkbox.checked << j;
      }

    }
    console.log(output_value);
    send_parameter(minichord_device, base_adress_rythm + i, output_value);

  }
}
//-->>MIDI HANDLER
MIDI_request_option = {
  sysex: true,
  software: false
};
//Initial setup 
try {
  navigator.requestMIDIAccess(MIDI_request_option)
    .then(onMIDISuccess, onMIDIFailure);
  //connection
  function onMIDISuccess(midiAccess) {
    document.getElementById("step1").classList.remove("unsatisfied");
    for (const entry of midiAccess.outputs) {
      const output = entry[1];
      if (output.name == "Minichord") {
        console.log(
          `Output port [type:'${output.type}'] id: '${output.id}' manufacturer: '${output.manufacturer}' name: '${output.name}' version: '${output.version}'`,
        );
        minichord_device = output
        const sysex_message = [0xF0, 0, 0, 0, 0, 0xF7]; //Query for the current settings of the Minichord
        minichord_device.send(sysex_message); // sends the message.
      }
    }
    //handle for receiving the data
    for (const entry of midiAccess.inputs) {
      const input = entry[1];
      if (input.name == "Minichord") {
        input.onmidimessage = process_current_data;
      }
    }
    if (minichord_device == false) {
      document.getElementById("information_text").innerHTML = "Make sure the minichord is connected to the computer and turned on";
      document.getElementById("information_zone").focus();
    }
    midiAccess.onstatechange = (event) => {
      // In case the device is disconnectd during use
      if (event.port.state == "disconnected") {
        minichord_device = false;
        document.getElementById("information_text").innerHTML = "minichord disconnected, please reconnect and reload page";
        document.getElementById("information_zone").focus();
        var body = document.getElementById('body');
        window.scrollTo(0, 0);
        body.classList.add("control_full");
        var elements = document.getElementsByClassName('active');
        while (elements.length > 0) {
          elements.item(0).classList.add("inactive");
          elements[0].classList.remove("active");
        }
      }
    };
    document.getElementById("step2").classList.remove("unsatisfied");

  }
} catch (error) {
  console.error(error);
  document.getElementById("information_text").innerHTML = "Please use a compatible browser, for example Chrome";
  document.getElementById("information_zone").focus();
}

//Handle refusal to grant access
function onMIDIFailure() {
  document.getElementById("information_text").innerHTML = "Please reload and provide the authorisation to access the Minichord";
  document.getElementById("information_zone").focus();
}
function process_current_data(midiMessage) {
  data = midiMessage.data.slice(1);
  if (data.length != parameter_size * 2 + 1) {
    console.log("incomplete message received, ignoring");
  } else {
    //the two first parameter are for control and bank number, ignored
    //the rest are applied to the sliders
    for (var i = 2; i < parameter_size; i++) {
      if (i < base_adress_rythm + 16 & i > base_adress_rythm - 1) {
        sysex_value = data[2 * i] + 128 * data[2 * i + 1]
        console.log(sysex_value);
        var j = i - base_adress_rythm;
        for (var k = 0; k < 7; k++) {
          var checkbox = document.getElementById("checkbox" + k + j);
          if (sysex_value & 1 << k) {
            checkbox.checked = true;
          } else {
            checkbox.checked = false;
          }
        }

      } else {
        var result = document.querySelectorAll('[adress_field="' + i + '"]');
        if (result.length > 0) {
          sysex_value = data[2 * i] + 128 * data[2 * i + 1]
          var slider_value
          if (result[0].getAttribute("curve") == "exponential") {
            slider_value = Math.round((result[0].max * Math.log(sysex_value) / Math.log(result[0].max)));
          } else {
            slider_value = sysex_value
          }
          if (result[0].getAttribute("data_type") == "float") {
            result[0].value = slider_value / 100.0;
            var value_zone = document.getElementById("value_zone" + i);
            console.log(value_zone);
            value_zone.innerHTML = sysex_value / 100.0
          } else {
            result[0].value = slider_value;
            var value_zone = document.getElementById("value_zone" + i);
            console.log(value_zone);
            value_zone.innerHTML = sysex_value
          }
        }
      }
    }
    //We override the potentiometer value that might be stored (so we can rightfully test it)
    for( const i of potentiometer_memory_adress){
      console.log(i);
      send_parameter(minichord_device, i, 512);
    }
    //We apply the bank number to dropdown and header
    let element = document.getElementById("bank_number_selection");
    bank_number = data[2 * 1]
    element.value = bank_number;
    var from_one_bank_number = bank_number + 1;
    //document.getElementById("bank_number_zone").innerText = "Current bank :" + from_one_bank_number;
    //we apply the current hue to the background of the page 
    var result = document.querySelectorAll('[adress_field="' + color_hue_sysex_adress + '"]');
    if (result.length > 0) {
      console.log(result);
      var hue = result[0].valueAsNumber;
      console.log('hsl(' + hue + ',100%,100%)');
      //document.body.style.color = 'hsl(' + hue + ',100%,50%)';
      var elements = document.getElementsByClassName('slider');
      for (let i = 0; i < elements.length; i++) {
        elements[i].style.setProperty('--slider_color', 'hsl(' + hue + ',100%,50%)')
      }
    }
    document.getElementById("step3").classList.remove("unsatisfied");
    document.getElementById("information_text").innerHTML = "minichord connected";
    var body = document.getElementById('body');
    body.classList.remove("control_full");
    var elements = document.getElementsByClassName('inactive');
    for (const i of potentiometer_memory_adress) {
      send_parameter(minichord_device, i, 512); //we initialize the potentiometers, we want them at the middle of our setting
    }
    while (elements.length > 0) {
      elements.item(0).classList.add("active");
      elements[0].classList.remove("inactive");
    }
  }
}


//-->>COMMUNICATION HANDLERS
function send_parameter(device, adress, value) {
  first_byte = parseInt(value % 128);
  second_byte = parseInt(value / 128)
  first_byte_adress = parseInt(adress % 128);
  second_byte_adress = parseInt(adress / 128)
  const sysex_message = [0xF0, first_byte_adress, second_byte_adress, first_byte, second_byte, 0xF7];
  device.send(sysex_message); // sends the message.
  console.log("sent");

}
function reset_memory() {
  if (minichord_device) {
    const sysex_message = [0xF0, 0, 0, 1, 0, 0xF7]; //send 1 to wipe memory
    minichord_device.send(sysex_message); // sends the message.
  } else {
    document.getElementById("information_zone").focus();
  }
}
function save_current_settings() {
  if (minichord_device) {
    var e = document.getElementById("bank_number_selection");
    var bank_number = e.value;
    console.log(bank_number);
    const sysex_message = [0xF0, 0, 0, 2, bank_number, 0xF7]; //send 2 to save at bank
    minichord_device.send(sysex_message); // sends the message.
  } else {
    document.getElementById("information_zone").focus();
  }
}
function reset_selected_bank() {
  if (minichord_device) {
    var e = document.getElementById("bank_number_selection");
    var bank_number = e.value;
    console.log(bank_number);
    const sysex_message = [0xF0, 0, 0, 3, bank_number, 0xF7]; //send 3 to reset current bank
    minichord_device.send(sysex_message); // sends the message.
  } else {
    document.getElementById("information_zone").focus();
  }
}

//-->>SETTINGS OFFLINE SHARE
function generate_settings() {
  if (minichord_device == false) {
    document.getElementById("information_zone").focus();
  }else{
    var sysex_array = Array(parameter_size).fill(0);
    var sliders = document.querySelectorAll(".slider")
    for (let i = 0; i < sliders.length; i++) {
      curve_type = sliders[i].getAttribute("curve")
      range_value = 0;
      adress = sliders[i].getAttribute("adress_field")
      if (curve_type == "exponential") {
        range_value = Math.round(Math.exp((Math.log(sliders[i].max) / sliders[i].max) * sliders[i].value));
      } else {
        range_value = sliders[i].value
      }

      if (sliders[i].getAttribute("data_type") == "float") {
        range_value = Math.round(range_value * 100);
      }
      sysex_array[adress] = range_value
    }
    //now the rythm pattern
    for (var i = 0; i < 16; i++) {
      var output_value = 0
      for (var j = 0; j < 7; j++) {
        var checkbox = document.getElementById("checkbox" + j + i);
        if (null != checkbox) {
          output_value = output_value | checkbox.checked << j;
        }

      }
      sysex_array[ base_adress_rythm + i] = output_value
    }
    output_string = "int16_t sysex_parameters[" + parameter_size + "]={";
    output_base64="";
    for (let i = 0; i < (parameter_size - 1); i++) {
      output_string += String(sysex_array[i])
      output_string += ","
      output_base64+=sysex_array[i];
      output_base64+=";"
    }
    output_string += String(sysex_array[parameter_size - 1]);
    output_string += "};"
    encoded=btoa(output_base64)
    console.log(encoded)
    navigator.clipboard.writeText(encoded);
    alert("Preset code copied to clipboard");
    console.log(atob(encoded))
    document.getElementById("output_zone").innerHTML = output_string;
  }
}

function load_settings() {
  if (minichord_device == false) {
    document.getElementById("information_zone").focus();
  }else{
    let preset_code = prompt('Paste preset code');
    if(preset_code!=null){
      parameters=atob(preset_code).split(";");
      if(parameters.length!=parameter_size){
        alert("malformed preset code");
      }else{
        adress_index=2; //skip the command and bank number
        for (adress_index; adress_index<parameter_size;adress_index++) {
          send_parameter(minichord_device,adress_index, parameters[adress_index]);
        } 
        send_parameter(minichord_device,0, 0);//we ask the minichord to update the interface
      }
    }
  }
}
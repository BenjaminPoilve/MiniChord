
// Global controller instance
const miniChordController = new MiniChordController();

// Legacy global variables for backward compatibility
let minichord_device = false;
let active_bank_number = -1;

//-->>UTILITIES
function map_value(value, in_min, in_max, out_min, out_max) {
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + Number(out_min);
}

//-->>INTERFACE HANDLER
//Slider Handler
// A row of degree checkboxes writes bits of one parameter. Assemble the mask
// from every box in the row and send it as a single value.
function handledegree(box) {
  const address = box.getAttribute("adress_field");
  const boxes = document.querySelectorAll('.degree_box[adress_field="' + address + '"]');
  let mask = 0;
  boxes.forEach(b => { if (b.checked) mask |= (1 << parseInt(b.getAttribute("bit"))); });
  const value_zone = document.getElementById("value_zone" + address);
  if (value_zone) {
    let count = 0;
    boxes.forEach(b => { if (b.checked) count++; });
    value_zone.innerHTML = count + " notes";
  }
  miniChordController.sendParameter(address, mask);
}

function handlechange(event) {
  if (miniChordController.isConnected()) {
    const curve_type = event.getAttribute("curve");
    let range_value;
    if (curve_type == "exponential") {
      range_value = Math.exp((Math.log(event.max) / event.max) * event.value);
    } else {
      range_value = event.value;
    }
    let displayed_value;
    if (event.getAttribute("data_type") == "int") {
      displayed_value = map_value(range_value, event.min, event.max, event.getAttribute("target_min"), event.getAttribute("target_max")).toFixed(0);
    } else if (event.getAttribute("data_type") == "float") {
      displayed_value = map_value(range_value, event.min, event.max, event.getAttribute("target_min"), event.getAttribute("target_max")).toFixed(2);
      range_value = Math.round(range_value * miniChordController.float_multiplier);
    }
    const address = event.getAttribute("adress_field");
    var value_zone = document.getElementById("value_zone" + event.getAttribute("adress_field"));
    if (value_zone) {
      value_zone.innerHTML = displayed_value;
    }
    miniChordController.sendParameter(address, Math.round(range_value));
    if (control_leads[address] != null) retarget_value_slider(control_leads[address]);
    if (address == miniChordController.color_hue_sysex_adress) { //handling the color scheme
      var hue = Math.round(range_value);
      var elements = document.getElementsByClassName('slider');
      for (let i = 0; i < elements.length; i++) {
        elements[i].style.setProperty('--slider_color', 'hsl(' + hue + ',100%,50%)');
      }
    }
  } else {
    event.value = event.getAttribute("value");
    document.getElementById("information_zone").focus();
  }
}
//function that builds the checkboxes for rythm pattern
function checkbox_array() {
  var container;
  for (var i = 0; i < 16; i++) {
    target_adress = miniChordController.base_adress_rythm + i;
    container = document.getElementById(target_adress);
    if (i < 7) {
      container.innerHTML = "";
      container.className = "line data_line content_line inactive";
      var padding_name = document.createElement("div");
      padding_name.className = "bloc B4 M0 S0";
      padding_name.innerHTML = "";
      container.appendChild(padding_name);
      var padding_category = document.createElement("div");
      padding_category.className = "bloc B3 M2 S3";
      padding_category.innerHTML = "";
      container.appendChild(padding_category);
      var padding_ID = document.createElement("div");
      padding_ID.className = "bloc B1 M1 S1";
      padding_ID.innerHTML = "***";
      container.appendChild(padding_ID);
      var name_zone = document.createElement("div");
      name_zone.className = "bloc B4 M3 S4";
      if (i < 4) {
        name_zone.innerHTML = "voice " + (i + 1);
      } else {
        name_zone.innerHTML = "voice " + (i - 2) + '"';
      }
      container.appendChild(name_zone);
      var line = document.createElement("div");
      line.className = "checkbox_div bloc B5 M4 S7";
      container.appendChild(line);

      for (var j = 0; j < 16; j++) {
        var checkBox = document.createElement("input");
        checkBox.onclick = send_array_data;
        checkBox.type = "checkbox";
        checkBox.id = "checkbox" + i + j;
        line.appendChild(checkBox);
      }
    } else {
      container.innerHTML = "";
      container.style.display = "block";
    }
  }
}
checkbox_array();
function send_array_data() {
  for (var i = 0; i < 16; i++) {
    var output_value = 0;
    for (var j = 0; j < 7; j++) {
      var checkbox = document.getElementById("checkbox" + j + i);
      if (null != checkbox) {
        output_value = output_value | (checkbox.checked << j);
      }
    }
    console.log(output_value);
    miniChordController.sendParameter(miniChordController.base_adress_rythm + i, output_value);
  }
}
//-->>UI INTEGRATION
// UI callbacks for controller events
miniChordController.onConnectionChange = function(connected, message) {
  if (connected) {
    document.getElementById("step3").classList.remove("unsatisfied");
    document.getElementById("information_text").innerHTML = "";
    document.getElementById("status_zone").className = "";
    document.getElementById("status_zone").classList.add("connected");
    var body = document.getElementById('body');
    body.classList.remove("control_full");
  } else {
    document.getElementById("information_text").innerHTML = message;
    document.getElementById("information_zone").focus();
    if (message.includes("disconnected")) {
      document.getElementById("status_zone").className = "";
      document.getElementById("status_zone").classList.add("disconnected");
      var body = document.getElementById('body');
      window.scrollTo(0, 0);
      body.classList.add("control_full");
      var elements = document.getElementsByClassName('active');
      while (elements.length > 0) {
        elements.item(0).classList.add("inactive");
        elements[0].classList.remove("active");
      }
    }
  }
  // Update legacy variable for backward compatibility
  minichord_device = miniChordController.isConnected();
};

miniChordController.onDataReceived = function(data) {
  // Update sliders
  for (let i = 2; i < miniChordController.parameter_size; i++) {
    if (data.parameters[i] !== undefined) {
      set_slider_to_value(i, data.parameters[i]);
    }
  }
  
  // Update rhythm checkboxes
  for (let j = 0; j < data.rhythmData.length; j++) {
    const rhythmBits = data.rhythmData[j];
    if (rhythmBits) {
      for (let k = 0; k < 7; k++) {
        const checkbox = document.getElementById("checkbox" + k + j);
        if (checkbox) {
          checkbox.checked = rhythmBits[k];
        }
      }
    }
  }
  
  // Update bank selection
  const element = document.getElementById("bank_number_selection");
  if (element) {
    element.value = data.bankNumber;
  }
  active_bank_number = data.bankNumber;
  
  // Apply color theme
  const result = document.querySelectorAll('[adress_field="' + miniChordController.color_hue_sysex_adress + '"]');
  if (result.length > 0) {
    const hue = result[0].valueAsNumber;
    const elements = document.getElementsByClassName('slider');
    for (let i = 0; i < elements.length; i++) {
      elements[i].style.setProperty('--slider_color', 'hsl(' + hue + ',100%,50%)');
    }
  }
  
  // Update UI state
  document.getElementById("step3").classList.remove("unsatisfied");
  document.getElementById("information_text").innerHTML = "";
  document.getElementById("status_zone").className = "";
  document.getElementById("status_zone").classList.add("connected");
  
  var body = document.getElementById('body');
  body.classList.remove("control_full");
  var elements = document.querySelectorAll('.inactive');
  
  for (let i = 0; i < elements.length; i++) {
    if (elements[i].getAttribute("version") <= data.firmwareVersion) {
      elements[i].classList.add("active");
      elements[i].classList.remove("inactive");
    }
  }
  
  // Update connection status
  miniChordController.onConnectionChange(true, "");
};

// Initialize the controller
async function initializeMidiController() {
  try {
    const success = await miniChordController.initialize();
    if (success) {
      document.getElementById("step1").classList.remove("unsatisfied");
      document.getElementById("step2").classList.remove("unsatisfied");
    } else {
      document.getElementById("information_text").innerHTML = "> please use a compatible browser";
      document.getElementById("information_zone").focus();
    }
  } catch (error) {
    console.log(">> ERROR: MIDI initialization failed");
    console.error(error);
    document.getElementById("information_text").innerHTML = "> please reload and provide the authorisation to access the minichord";
    document.getElementById("information_zone").focus();
  }
}

// Start initialization
initializeMidiController();

//-->>DOUBLE TAP VALUES FOLLOW THEIR TARGETS
// A double tap value is a raw number whatever its target, so on its own it is a 0-4095 slider.
// Once its control names a target, the value's slider takes on that target's own range, type and
// curve, so the value is set the way the setting itself is.
// parameters.json says which control each value follows ("follows_target").
let parameter_meta = {};         // address -> the parameter's entry in parameters.json
let value_follows = {};          // value address -> its control's address
let control_leads = {};          // control address -> the value address that follows it
fetch('./json/parameters.json').then(r => r.json()).then(d => {
  ['global_parameter', 'harp_parameter', 'chord_parameter'].forEach(section => (d[section] || []).forEach(p => {
    parameter_meta[p.sysex_adress] = p;
    if (p.follows_target != null) { value_follows[p.sysex_adress] = p.follows_target; control_leads[p.follows_target] = p.sysex_adress; }
  }));
  Object.keys(value_follows).forEach(v => retarget_value_slider(v));
}).catch(e => console.log("parameters.json not loaded; double tap values stay plain sliders", e));

function retarget_value_slider(value_address) {
  const slider = document.querySelector('input[adress_field="' + value_address + '"]');
  const control = document.querySelector('input[adress_field="' + value_follows[value_address] + '"]');
  if (!slider || !control) return;
  if (!slider.hasAttribute("plain_max")) {       // remember the plain slider, to go back to with no target
    ["min", "max", "step", "target_min", "target_max", "data_type", "curve"].forEach(a => slider.setAttribute("plain_" + a, slider.getAttribute(a)));
  }
  const target = parameter_meta[parseInt(control.value)];
  const plain = !target || target.follows_target != null || target.sysex_adress == value_address;
  const attr = a => plain ? slider.getAttribute("plain_" + a) : null;
  const float = !plain && target.data_type == "float";
  slider.min = plain ? attr("min") : target.min_value;
  slider.max = plain ? attr("max") : target.max_value;
  slider.step = plain ? attr("step") : (float ? "0.01" : "1");
  slider.setAttribute("target_min", plain ? attr("target_min") : target.min_value);
  slider.setAttribute("target_max", plain ? attr("target_max") : target.max_value);
  slider.setAttribute("data_type", plain ? attr("data_type") : target.data_type);
  slider.setAttribute("curve", plain ? attr("curve") : target.curve);
}

function set_slider_to_value(slider_num, sysex_value) {
  var result = document.querySelectorAll('[adress_field="' + slider_num + '"]');
  if (result.length > 0) {
    var slider_value;
    if (result[0].getAttribute("curve") == "exponential") {
      slider_value = Math.round((result[0].max * Math.log(sysex_value) / Math.log(result[0].max)));
    } else {
      slider_value = sysex_value;
    }
    if (result[0].getAttribute("data_type") == "float") {
      result[0].value = slider_value / miniChordController.float_multiplier;
      var value_zone = document.getElementById("value_zone" + slider_num);
      if (value_zone) {
        value_zone.innerHTML = sysex_value / miniChordController.float_multiplier;
      }
    } else if (result[0].getAttribute("data_type") == "degrees") {
      const boxes = document.querySelectorAll('.degree_box[adress_field="' + slider_num + '"]');
      let count = 0;
      boxes.forEach(b => {
        const on = (sysex_value & (1 << parseInt(b.getAttribute("bit")))) != 0;
        b.checked = on;
        if (on) count++;
      });
      var value_zone = document.getElementById("value_zone" + slider_num);
      if (value_zone) {
        value_zone.innerHTML = count + " notes";
      }
    } else {
      result[0].value = slider_value;
      var value_zone = document.getElementById("value_zone" + slider_num);
      if (value_zone) {
        value_zone.innerHTML = sysex_value;
      }
    }
    if (control_leads[slider_num] != null) retarget_value_slider(control_leads[slider_num]);
    if (value_follows[slider_num] != null) retarget_value_slider(slider_num);
  }
}


//-->>COMMUNICATION HANDLERS 
function reset_memory() {
  if (miniChordController.isConnected()) {
    return miniChordController.resetMemory();
  } else {
    document.getElementById("information_zone").focus();
    return false;
  }
}

function save_current_settings() {
  if (miniChordController.isConnected()) {
    var e = document.getElementById("bank_number_selection");
    var bank_number = e.value;
    console.log(bank_number);
    return miniChordController.saveCurrentSettings(bank_number);
  } else {
    document.getElementById("information_zone").focus();
    return false;
  }
}

function reset_current_bank() {
  if (miniChordController.isConnected()) {
    console.log(miniChordController.active_bank_number);
    return miniChordController.resetCurrentBank();
  } else {
    document.getElementById("information_zone").focus();
    return false;
  }
}

//-->>SETTINGS OFFLINE SHARE
function generate_settings() {
  if (!miniChordController.isConnected()) {
    document.getElementById("information_zone").focus();
  }else{
    var sysex_array = Array(miniChordController.parameter_size).fill(0);
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
      sysex_array[miniChordController.base_adress_rythm + i] = output_value
    }
    output_base64="";
    output_string="{";
    for (let i = 0; i < (miniChordController.parameter_size - 1); i++) {
      output_string += String(sysex_array[i])
      output_string += ","
      output_base64+=sysex_array[i];
      output_base64+=";"
    }
    output_string += String(sysex_array[miniChordController.parameter_size - 1]);
    output_string += "},"
    encoded=btoa(output_base64)
    console.log(encoded)
    navigator.clipboard.writeText(encoded);
    alert("Preset code copied to clipboard");
    console.log(atob(encoded))
    document.getElementById("output_zone").innerHTML = output_string;
  }
}

function load_settings() {
  if (!miniChordController.isConnected()) {
    document.getElementById("information_zone").focus();
  }else{
    let preset_code = prompt('Paste preset code');
    if(preset_code!=null){
      parameters=atob(preset_code).split(";");
      if(parameters.length!=miniChordController.parameter_size){
        alert("malformed preset code");
      }else{
        adress_index=2; //skip the command and bank number
        for (adress_index; adress_index<miniChordController.parameter_size;adress_index++) {
          miniChordController.sendParameter(adress_index, parameters[adress_index]);
        } 
        miniChordController.sendParameter(0, 0);//we ask the minichord to update the interface
      }
    }
  }
}

// Add hover highlighting for parameter names
function initializeSliderHoverEffects() {
  const sliders = document.querySelectorAll('.slider');
  
  sliders.forEach(slider => {
    // Find the corresponding dfn element in the same line
    const parentLine = slider.closest('.content_line');
    if (parentLine) {
      const dfnElement = parentLine.querySelector('dfn');
      
      if (dfnElement) {
        slider.addEventListener('mouseenter', () => {
          dfnElement.classList.add('highlighted');
        });
        
        slider.addEventListener('mouseleave', () => {
          dfnElement.classList.remove('highlighted');
        });
      }
    }
  });
}

// Initialize hover effects when the page loads
document.addEventListener('DOMContentLoaded', initializeSliderHoverEffects);

//-->>RANDOM PRESET GENERATOR
// Function to fetch and parse parameters.json and optionally load a random preset
async function loadParameterRanges() {
  try {
    const [parametersResponse, presetsResponse] = await Promise.all([
      fetch('./json/parameters.json'),
      fetch('./json/shared_presets.json')
    ]);
    
    const parametersData = await parametersResponse.json();
    const presetsData = await presetsResponse.json();
    
    const parameterRanges = {};
    
    // Select a random preset
    const randomPreset = presetsData.shared_presets[Math.floor(Math.random() * presetsData.shared_presets.length)];
    console.log(`Using random preset as base: "${randomPreset.name}" by ${randomPreset.author}`);
    
    // Decode the preset values
    const decodedPreset = atob(randomPreset.value).split(';').map(v => parseFloat(v));
    
    // Process all parameter categories
    ['global_parameter', 'harp_parameter', 'chord_parameter'].forEach(category => {
      parametersData[category].forEach(param => {
        const presetValue = decodedPreset[param.sysex_adress];
        let defaultValue = param.default_value;
        
        // Use preset value if available, otherwise fall back to original default
        if (presetValue !== undefined && presetValue !== null && !isNaN(presetValue)) {
          if (param.data_type === "float") {
            defaultValue = presetValue / 100; // Convert back from stored format
          } else {
            defaultValue = presetValue;
          }
        }
        
        parameterRanges[param.sysex_adress] = {
          min: param.min_value,
          max: param.max_value,
          type: param.data_type,
          default: defaultValue,
          original_default: param.default_value // Keep original default for reference
        };
      });
    });
    
    return parameterRanges;
  } catch (error) {
    console.error('Error loading parameter ranges or presets:', error);
    // Fallback to original behavior if preset loading fails
    try {
      const response = await fetch('./json/parameters.json');
      const parametersData = await response.json();
      
      const parameterRanges = {};
      
      ['global_parameter', 'harp_parameter', 'chord_parameter'].forEach(category => {
        parametersData[category].forEach(param => {
          parameterRanges[param.sysex_adress] = {
            min: param.min_value,
            max: param.max_value,
            type: param.data_type,
            default: param.default_value,
            original_default: param.default_value // Keep original default for reference

          };
        });
      });
      
      return parameterRanges;
    } catch (fallbackError) {
      console.error('Error loading parameter ranges:', fallbackError);
      return {};
    }
  }
}

// Generate random preset function
// This is based on TerminalWaltz's code. Provided under GNU General Public License v3.0
// License available here : https://github.com/terminalwaltz/minichord-random-generator/blob/main/LICENSE 
// Normal distribution function using Box-Muller transform
function normalRandom(mean, sigma) {
  let u = 0, v = 0;
  while(u === 0) u = Math.random(); // Converting [0,1) to (0,1)
  while(v === 0) v = Math.random();
  const z = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
  return z * sigma + mean;
}

async function generateRandomPreset() {
  if (!miniChordController.isConnected()) {
    document.getElementById("information_zone").focus();
    return;
  }
  
  const parameterRanges = await loadParameterRanges();

  const weirdness_factor = 0.10;

  // Initialize preset array
  const preset = Array(miniChordController.parameter_size).fill(0);

  const fixedValues = [32,33,34,35,41,97,106,107,108,197];
  
  // Generate values for each parameter
  Object.entries(parameterRanges).forEach(([index, params]) => {
    const idx = parseInt(index);
    
    if (idx < 19 || fixedValues.includes(idx)) {
      // For parameters below 19, keep the default values
      preset[idx] = params.original_default;
    } else {
      // For parameters above 21, generate random values with normal distribution
      const minVal = params.min;
      const maxVal = params.max;
      const center = params.default;;
      const range = maxVal - minVal;
      const sigma = range*weirdness_factor; // Use weirdness_factor to define sigma
      
      let value = normalRandom(center, sigma);
      
      // Clamp to valid range
      if(value<0){
        value = -value/4.0; // Ensure no negative values, but do not give excessive weight to the negative side
      }
      value = Math.max(minVal, Math.min(maxVal, value));
      console.log(`Parameter ${idx}: Generated value ${value} (min: ${minVal}, max: ${maxVal})`);
      
      // Handle data type
      if (params.type === "int") {
        preset[idx] = Math.round(value);
      } else { // float
        preset[idx] = Math.round(value * 100) / 100;
      }
    }
  });
  
  // Send the preset to the device
  for (let i = 2; i < miniChordController.parameter_size; i++) {
    if (preset[i] !== undefined) {
      let valueToSend = preset[i];
      
      // Float values are already in the correct range (e.g., 0-100), 
      // just multiply by the float_multiplier for transmission
      if (parameterRanges[i] && parameterRanges[i].type === "float") {
        valueToSend = Math.round(preset[i] * miniChordController.float_multiplier);
      }
      
      miniChordController.sendParameter(i, Math.round(valueToSend));
    }
  }
  
  // Update the interface
  miniChordController.sendParameter(0, 0);
  
  console.log("Random preset generated and applied!");
}

// Add event listener to the randomise button
document.addEventListener('DOMContentLoaded', function() {
  const randomiseBtn = document.getElementById('randomise_btn');
  if (randomiseBtn) {
    randomiseBtn.addEventListener('click', generateRandomPreset);
  }
});

// DARK MODE
function toggleTheme() {
  const body = document.body;
  body.classList.toggle('dark-mode');
  // Save the theme preference to localStorage
  const isDarkMode = body.classList.contains('dark-mode');
  localStorage.setItem('theme', isDarkMode ? 'dark' : 'light');
}

// Load the saved theme on page load
document.addEventListener('DOMContentLoaded', () => {
  const savedTheme = localStorage.getItem('theme');
  if (savedTheme === 'dark') {
    document.body.classList.add('dark-mode');
  }
});
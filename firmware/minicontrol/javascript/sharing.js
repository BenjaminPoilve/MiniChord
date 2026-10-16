// Sharing page functionality for MiniChord presets

// Global controller instance
const miniChordController = new MiniChordController();

// Global state
let currentBankNumber = -1;
let notificationTimeout = null;

// Preset management functions
class PresetManager {
    constructor() {
        this.presets = [];
        this.parameters = null;
        this.loadParameters();
        this.loadPresets();
    }

    async loadParameters() {
        try {
            const response = await fetch('json/parameters.json');
            if (response.ok) {
                this.parameters = await response.json();
            } else {
                console.error('Could not load parameters from JSON');
            }
        } catch (error) {
            console.error('Error loading parameters:', error);
        }
    }

    async loadPresets() {
        try {
            // Load from JSON file
            const response = await fetch('json/shared_presets.json');
            if (response.ok) {
                const data = await response.json();
                this.presets = data.shared_presets || [];
            } else {
                console.error('Could not load presets from JSON');
                this.presets = [];
            }
        } catch (error) {
            console.error('Error loading presets:', error);
            this.presets = [];
        }
        this.renderPresets();
    }

    renderPresets() {
        const container = document.getElementById('presetContainer');
        container.innerHTML = '';

        if (this.presets.length === 0) {
            container.innerHTML = '<p style="text-align: center; color: #666;">No presets available</p>';
            return;
        }

        this.presets.forEach((preset, index) => {
            const card = this.createPresetCard(preset, index);
            container.appendChild(card);
        });
    }

    createPresetCard(preset, index) {
        const card = document.createElement('div');
        card.className = 'preset-card';
        card.setAttribute('data-preset-index', index);
        const presetData = this.decodePresetData(preset.value);
        const hue = Math.round(presetData[miniChordController.color_hue_sysex_adress]);
        card.style.backgroundColor = 'hsl(' + hue + ',80%,90%)';
        
        // Get modulation parameter values
        const modMainControl = presetData[miniChordController.modulation_adress[0]];
        const altChordControl = presetData[miniChordController.modulation_adress[1]];
        const altHarpControl = presetData[miniChordController.modulation_adress[2]];
        const altModControl = presetData[miniChordController.modulation_adress[3]];

        // Create parameter display using parameters.json
        const displayParameters = this.getParameterNames([
            { address: modMainControl, control: "Modulation main" },
            { address:altChordControl, control: "Chord secondary" },
            { address: altHarpControl, control: "Harp secondary" },
            { address: altModControl, control: "Modulation secondary" }
        ]);

        // Create parameter list
        const parameterElements = displayParameters.map(param => 
            `<div class="parameter-item">
                <span class="parameter-name">${param.select}:</span>
                <span class="parameter-value">${param.value}</span>
            </div>`
        ).join('');

        card.innerHTML = `
            <div class="title-card">
                <h3>${preset.name}</h3>
                <button class="expand-btn">+</button>
            </div>
            <div class="preset-subtitle">by ${preset.author || 'Unknown'}</div>
            <div class="preset-actions">
                <button class="preset-btn try-btn" onclick="presetManager.tryPreset(${index})">Try</button>
                <button class="preset-btn save-btn" onclick="presetManager.savePreset(${index})">Save to current bank</button>
            </div>
            <div class="preset-details">
                <div class="preset-description">${preset.description || 'No description available'}</div>
                <div class="preset-parameters">
                    <h5>Potentiometer Parameters:</h5>
                    ${parameterElements}
                </div>
                <div style="margin-top: 1rem;">
                    <button class="preset-btn" onclick="presetManager.openInMinicontrol(${index})">Open in minicontrol</button>
                </div>
            </div>
        `;

        // Add click handler for expansion
        const expandBtn = card.querySelector('.expand-btn');
        expandBtn.addEventListener('click', (e) => {
            e.stopPropagation();
            this.toggleCard(card);
        });

        return card;
    }

    getParameterNames(parameterList) {
        if (!this.parameters) {
            // Fallback to simple display if parameters.json not loaded
            return parameterList.map((param, index) => ({
                select: param.control.toString(),
                value: param.address.toString()
            }));
        }

        return parameterList.map(param => {
            // Find parameter definition by sysex address
            let paramDef = null;
            
            // Search in all parameter categories
            const allParams = [
                ...(this.parameters.global_parameter || []),
                ...(this.parameters.harp_parameter || []),
                ...(this.parameters.chord_parameter || [])
            ];
            
            paramDef = allParams.find(p => p.sysex_adress === param.address);
            
            if (paramDef) {              
                return {
                    select: param.control.toString(),
                    value: paramDef.group.toString()+" "+paramDef.name.toString()
                };
            } else {
                // Fallback for unknown parameters
                return {
                    select: param.control.toString(),
                    value: param.address.toString()
                };
            }
        });
    }

    toggleCard(card) {
        // Close all other cards first
        const allCards = document.querySelectorAll('.preset-card');
        allCards.forEach(otherCard => {
            if (otherCard !== card) {
                otherCard.classList.remove('expanded');
                const otherExpandBtn = otherCard.querySelector('.expand-btn');
                if (otherExpandBtn) {
                    otherExpandBtn.textContent = '+';
                }
            }
        });
        
        // Toggle the clicked card
        card.classList.toggle('expanded');
        
        // Update button text
        const expandBtn = card.querySelector('.expand-btn');
        if (expandBtn) {
            expandBtn.textContent = card.classList.contains('expanded') ? '−' : '+';
        }
    }

    async tryPreset(presetIndex) {
        const preset = this.presets[presetIndex];
        if (!preset) return;

        if (!miniChordController.isConnected()) {
            alert('Please connect your minichord first');
            return;
        }

        try {
            // Decode the preset data
            const presetData = this.decodePresetData(preset.value);
            
            // Send all parameters to the device, page 1 at its defaults if the code has only page 0
            miniChordController.applyPreset(presetData);
            
            console.log(`Trying preset: ${preset.name}`);
            this.showNotification(`Applied preset: ${preset.name}`, 'success');
        } catch (error) {
            console.error('Error applying preset:', error);
            this.showNotification('Error applying preset', 'error');
        }
    }

    async savePreset(presetIndex) {
        const preset = this.presets[presetIndex];
        if (!preset) return;

        if (!miniChordController.isConnected()) {
            alert('Please connect your MiniChord device first');
            return;
        }

        try {
            // First apply the preset
            await this.tryPreset(presetIndex);
            
            // Give a small delay to ensure the preset is applied
            setTimeout(() => {
                // Save to current bank
                const bankNumber = currentBankNumber >= 0 ? currentBankNumber : 0;
                miniChordController.saveCurrentSettings(bankNumber);
                
                console.log(`Saved preset: ${preset.name} to bank ${bankNumber + 1}`);
                this.showNotification(`Saved "${preset.name}" to bank ${bankNumber + 1}`, 'success');
            }, 100);
        } catch (error) {
            console.error('Error saving preset:', error);
            this.showNotification('Error saving preset', 'error');
        }
    }

    async openInMinicontrol(presetIndex) {
        const preset = this.presets[presetIndex];
        if (!preset) return;

        try {
            // Apply the preset first
            await this.tryPreset(presetIndex);
            
            // Give a small delay to ensure the preset is applied, then redirect
            setTimeout(() => {
                window.location.href = './';
            }, 200);
        } catch (error) {
            console.error('Error opening preset in minicontrol:', error);
            this.showNotification('Error applying preset', 'error');
        }
    }

    decodePresetData(encodedData) {
        try {
            // Decode base64 and parse the data
            const decodedString = atob(encodedData);
            const parameters = decodedString.split(';');
            
            // Convert string values to numbers
            return parameters.map(param => {
                const num = parseInt(param);
                return isNaN(num) ? 0 : num;
            });
        } catch (error) {
            console.error('Error decoding preset data:', error);
            return new Array(miniChordController.parameter_size).fill(0);
        }
    }

    showNotification(message, type = 'info') {
        const statusElement = document.getElementById('status_zone');
        
        // Clear any existing notification timeout
        if (notificationTimeout) {
            clearTimeout(notificationTimeout);
        }
        
        // Show notification message in status
        statusElement.textContent = message;
        statusElement.className = `connection-status ${type === 'success' ? 'connected' : 'disconnected'}`;
        
        // Restore original status after 3 seconds
        notificationTimeout = setTimeout(() => {
            notificationTimeout = null;
            updateConnectionStatus(miniChordController.isConnected());
        }, 3000);
    }
}

// Initialize the preset manager
const presetManager = new PresetManager();

// MIDI Controller integration
miniChordController.onConnectionChange = function(connected) {
    updateConnectionStatus(connected);
};

miniChordController.onDataReceived = function(data) {
    // Update current bank number
    currentBankNumber = data.bankNumber;
    updateConnectionStatus(true);
};

function updateConnectionStatus(connected) {
    // Don't update if we're currently showing a notification
    if (notificationTimeout) {
        return;
    }
    
    const statusElement = document.getElementById('status_zone');
    if (connected) {
        const bankText = currentBankNumber >= 0 ? ` | Bank ${currentBankNumber + 1}` : '';
        statusElement.textContent = `● minichord connected${bankText}`;
        statusElement.className = 'connection-status connected';
    } else {
        statusElement.textContent = '● minichord disconnected';
        statusElement.className = 'connection-status disconnected';
    }
}

// Initialize MIDI controller when page loads
async function initializePage() {
    try {
        await miniChordController.initialize();
    } catch (error) {
        console.error('Failed to initialize MIDI controller:', error);
    }
}

// Start initialization when page loads
document.addEventListener('DOMContentLoaded', initializePage);
/*
 * Quantum Encryption Device (QED) - Core Implementation
 * 
 * Hardware-Dependent Cryptographic System Based on Physical Resonance
 * Copyright (C) 2025 Americo Simoes. All rights reserved.
 * 
 * This software is proprietary and confidential. Unauthorized copying,
 * distribution, or modification is strictly prohibited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include "../include/quantum_encryption.h"

// Global device instance for thread safety
static qed_device_t *global_device = NULL;

// Error string lookup table
static const char* error_strings[] = {
    "Success",                          // QED_SUCCESS
    "Memory allocation error",          // QED_ERROR_MEMORY
    "File I/O error",                  // QED_ERROR_FILE_IO
    "Hardware detection error",        // QED_ERROR_HARDWARE
    "Encryption error",                // QED_ERROR_ENCRYPTION
    "Decryption error",                // QED_ERROR_DECRYPTION
    "Signature mismatch",              // QED_ERROR_SIGNATURE_MISMATCH
    "Key not found",                   // QED_ERROR_KEY_NOT_FOUND
    "Invalid input",                   // QED_ERROR_INVALID_INPUT
    "Key limit reached"                // QED_ERROR_KEY_LIMIT_REACHED
};

const char* qed_get_error_string(qed_result_t result) {
    if (result >= 0 || result < -9) {
        return "Unknown error";
    }
    return error_strings[-result];
}

void qed_secure_zero(void *ptr, size_t len) {
    volatile uint8_t *p = (volatile uint8_t *)ptr;
    while (len--) {
        *p++ = 0;
    }
}

qed_result_t qed_detect_cpu_frequency(double *frequency) {
    FILE *fp;
    char line[256];
    double freq = 0.0;
    
    if (!frequency) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Try to read CPU frequency from /proc/cpuinfo
    fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        // Fallback to default frequency
        *frequency = 2.4e9; // 2.4 GHz default
        return QED_SUCCESS;
    }
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu MHz", 7) == 0) {
            char *colon = strchr(line, ':');
            if (colon) {
                freq = strtod(colon + 1, NULL);
                if (freq > 0) {
                    *frequency = freq * 1e6; // Convert MHz to Hz
                    fclose(fp);
                    return QED_SUCCESS;
                }
            }
        }
    }
    
    fclose(fp);
    
    // If we couldn't read the frequency, use a default
    *frequency = 2.4e9; // 2.4 GHz default
    return QED_SUCCESS;
}

qed_result_t qed_detect_ram_signature(qed_hardware_sig_t *hw_sig) {
    struct sysinfo info;
    char signature_input[256];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    if (!hw_sig) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    if (sysinfo(&info) != 0) {
        return QED_ERROR_HARDWARE;
    }
    
    hw_sig->ram_total = info.totalram;
    hw_sig->ram_available = info.freeram;
    
    // Create signature string
    snprintf(signature_input, sizeof(signature_input), "%lu%lu", 
             hw_sig->ram_total, hw_sig->ram_available);
    
    // Generate SHA256 hash
    if (!SHA256((unsigned char*)signature_input, strlen(signature_input), hash)) {
        return QED_ERROR_HARDWARE;
    }
    
    // Take first 4 bytes as signature and convert to uint32_t
    hw_sig->ram_signature = ((uint32_t)hash[0] << 24) |
                           ((uint32_t)hash[1] << 16) |
                           ((uint32_t)hash[2] << 8) |
                           ((uint32_t)hash[3]);
    hw_sig->ram_signature %= 1000000; // Modulo for consistency with Python version
    
    return QED_SUCCESS;
}

qed_result_t qed_measure_quantum_noise(qed_hardware_sig_t *hw_sig) {
    char seed_input[512];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    int i;
    
    if (!hw_sig) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Create seed from CPU frequency and RAM signature
    snprintf(seed_input, sizeof(seed_input), "%.6f%u", 
             hw_sig->cpu_frequency, hw_sig->ram_signature);
    
    // Generate SHA256 hash
    if (!SHA256((unsigned char*)seed_input, strlen(seed_input), hash)) {
        return QED_ERROR_HARDWARE;
    }
    
    // Convert hash bytes to string (similar to Python implementation)
    for (i = 0; i < SHA256_DIGEST_LENGTH && i < QED_QUANTUM_NOISE_LENGTH; i++) {
        int byte_val = hash[i] % 256;
        snprintf(hw_sig->quantum_noise + strlen(hw_sig->quantum_noise),
                QED_QUANTUM_NOISE_LENGTH - strlen(hw_sig->quantum_noise),
                "%d", byte_val);
        if (strlen(hw_sig->quantum_noise) >= QED_QUANTUM_NOISE_LENGTH) {
            break;
        }
    }
    
    // Truncate to exactly QED_QUANTUM_NOISE_LENGTH characters
    hw_sig->quantum_noise[QED_QUANTUM_NOISE_LENGTH] = '\0';
    
    return QED_SUCCESS;
}

qed_result_t qed_generate_hardware_resonance(const qed_hardware_sig_t *hw_sig, 
                                           uint8_t *resonance_data, size_t length) {
    double fixed_t;
    double xi, resonance, quantum_bit;
    int resonance_value;
    size_t i;
    
    if (!hw_sig || !resonance_data || length == 0) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Fixed time based on RAM signature for consistency
    fixed_t = hw_sig->ram_signature / 1e9;
    
    for (i = 0; i < length; i++) {
        xi = (double)i / length;
        
        // Generate resonance using the formula from the Python version
        resonance = sin(2.0 * M_PI * QED_RESONANCE_BASE * xi / 1e6);
        resonance *= (1.0 + QED_MASS_INCREASE * 
                     sin(2.0 * M_PI * hw_sig->cpu_frequency * fixed_t / 1e9));
        
        // Add quantum noise
        quantum_bit = (double)(hw_sig->quantum_noise[i % strlen(hw_sig->quantum_noise)] - '0');
        
        // Calculate final resonance value
        resonance_value = (int)(resonance * 128 + 128 + quantum_bit) % 256;
        resonance_data[i] = (uint8_t)resonance_value;
    }
    
    return QED_SUCCESS;
}

qed_result_t qed_generate_quantum_key(qed_device_t *device, const char *key_id, 
                                      uint8_t *key_out, size_t key_length) {
    size_t i;
    uint8_t *raw_key = NULL;
    unsigned char hash_input[1024];
    unsigned char final_hash[SHA256_DIGEST_LENGTH];
    size_t hash_input_len = 0;
    
    if (!device || !key_id || !key_out || key_length == 0) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    if (!device->initialized) {
        return QED_ERROR_HARDWARE;
    }
    
    // Check if key already exists
    for (i = 0; i < device->key_count; i++) {
        if (device->quantum_keys[i].in_use && 
            strcmp(device->quantum_keys[i].key_id, key_id) == 0) {
            // Key exists, return it
            memcpy(key_out, device->quantum_keys[i].key_data, 
                   key_length > QED_KEY_LENGTH ? QED_KEY_LENGTH : key_length);
            return QED_SUCCESS;
        }
    }
    
    // Check if we can add more keys
    if (device->key_count >= QED_MAX_KEYS) {
        return QED_ERROR_KEY_LIMIT_REACHED;
    }
    
    // Generate new key
    raw_key = malloc(key_length * 2);
    if (!raw_key) {
        return QED_ERROR_MEMORY;
    }
    
    // Generate hardware resonance
    qed_result_t result = qed_generate_hardware_resonance(&device->hardware_sig, 
                                                         raw_key, key_length * 2);
    if (result != QED_SUCCESS) {
        free(raw_key);
        return result;
    }
    
    // Combine raw key with quantum noise
    memcpy(hash_input, raw_key, key_length * 2);
    hash_input_len = key_length * 2;
    
    // Append quantum noise
    size_t noise_len = strlen(device->hardware_sig.quantum_noise);
    memcpy(hash_input + hash_input_len, device->hardware_sig.quantum_noise, noise_len);
    hash_input_len += noise_len;
    
    // Generate final key hash
    if (!SHA256(hash_input, hash_input_len, final_hash)) {
        free(raw_key);
        return QED_ERROR_HARDWARE;
    }
    
    // Store the key
    size_t key_index = device->key_count;
    strncpy(device->quantum_keys[key_index].key_id, key_id, QED_MAX_KEY_ID_LENGTH - 1);
    device->quantum_keys[key_index].key_id[QED_MAX_KEY_ID_LENGTH - 1] = '\0';
    
    size_t copy_len = key_length > QED_KEY_LENGTH ? QED_KEY_LENGTH : key_length;
    memcpy(device->quantum_keys[key_index].key_data, final_hash, copy_len);
    device->quantum_keys[key_index].in_use = true;
    device->key_count++;
    
    // Copy to output
    memcpy(key_out, final_hash, copy_len);
    
    // Secure cleanup
    qed_secure_zero(raw_key, key_length * 2);
    qed_secure_zero(hash_input, sizeof(hash_input));
    qed_secure_zero(final_hash, sizeof(final_hash));
    free(raw_key);
    
    // Print key generated message (first 16 chars for security)
    printf("🔑 Generated Quantum Key [%s]: ", key_id);
    for (i = 0; i < 8 && i < copy_len; i++) {
        printf("%02x", key_out[i]);
    }
    printf("...\n");
    
    return QED_SUCCESS;
}

qed_result_t qed_init(qed_device_t *device) {
    qed_result_t result;
    
    if (!device) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Initialize device structure
    memset(device, 0, sizeof(qed_device_t));
    
    // Detect CPU frequency
    result = qed_detect_cpu_frequency(&device->hardware_sig.cpu_frequency);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    // Detect RAM signature
    result = qed_detect_ram_signature(&device->hardware_sig);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    // Measure quantum noise
    result = qed_measure_quantum_noise(&device->hardware_sig);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    device->initialized = true;
    global_device = device;
    
    printf("🔒 Quantum Encryption Device Initialized\n");
    printf("⚡ CPU Frequency: %.2f Hz\n", device->hardware_sig.cpu_frequency);
    printf("🌌 Quantum Noise Signature: %.12s...\n", device->hardware_sig.quantum_noise);
    
    return QED_SUCCESS;
}

qed_result_t qed_cleanup(qed_device_t *device) {
    if (!device) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Securely wipe all keys
    qed_quantum_wipe_all(device);
    
    // Zero out the entire structure
    qed_secure_zero(device, sizeof(qed_device_t));
    
    if (global_device == device) {
        global_device = NULL;
    }
    
    return QED_SUCCESS;
}

qed_result_t qed_quantum_wipe(qed_device_t *device, const char *key_id) {
    size_t i;
    
    if (!device || !key_id) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    for (i = 0; i < device->key_count; i++) {
        if (device->quantum_keys[i].in_use && 
            strcmp(device->quantum_keys[i].key_id, key_id) == 0) {
            
            // Securely wipe the key
            qed_secure_zero(&device->quantum_keys[i], sizeof(qed_quantum_key_t));
            device->quantum_keys[i].in_use = false;
            
            printf("🌀 Quantum wiped key: %s\n", key_id);
            return QED_SUCCESS;
        }
    }
    
    return QED_ERROR_KEY_NOT_FOUND;
}

qed_result_t qed_quantum_wipe_all(qed_device_t *device) {
    size_t i;
    
    if (!device) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    for (i = 0; i < QED_MAX_KEYS; i++) {
        if (device->quantum_keys[i].in_use) {
            qed_secure_zero(&device->quantum_keys[i], sizeof(qed_quantum_key_t));
            device->quantum_keys[i].in_use = false;
        }
    }
    
    device->key_count = 0;
    printf("🌀 Quantum wiped all keys\n");
    
    return QED_SUCCESS;
}

void qed_print_hardware_info(const qed_hardware_sig_t *hw_sig) {
    if (!hw_sig) {
        return;
    }
    
    printf("Hardware Signature Information:\n");
    printf("  CPU Frequency: %.2f Hz\n", hw_sig->cpu_frequency);
    printf("  RAM Total: %lu bytes\n", hw_sig->ram_total);
    printf("  RAM Available: %lu bytes\n", hw_sig->ram_available);
    printf("  RAM Signature: %u\n", hw_sig->ram_signature);
    printf("  Quantum Noise: %.32s...\n", hw_sig->quantum_noise);
}
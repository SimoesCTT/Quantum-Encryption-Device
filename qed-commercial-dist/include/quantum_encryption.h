#ifndef QUANTUM_ENCRYPTION_H
#define QUANTUM_ENCRYPTION_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Quantum Encryption Device (QED) - C Implementation
 * 
 * Hardware-Dependent Cryptographic System Based on Physical Resonance
 * Copyright (C) 2025 Americo Simoes. All rights reserved.
 * 
 * This software is proprietary and confidential. Unauthorized copying,
 * distribution, or modification is strictly prohibited.
 */

#define QED_VERSION "2.0.0"
#define QED_KEY_LENGTH 32
#define QED_SIGNATURE_LENGTH 32
#define QED_QUANTUM_NOISE_LENGTH 64
#define QED_MAX_KEY_ID_LENGTH 256
#define QED_MAX_KEYS 1024

// Hardware resonance constants
#define QED_RESONANCE_BASE 1174000
#define QED_MASS_INCREASE 0.17
#define QED_T_SCALE 173000

// Error codes
typedef enum {
    QED_SUCCESS = 0,
    QED_ERROR_MEMORY = -1,
    QED_ERROR_FILE_IO = -2,
    QED_ERROR_HARDWARE = -3,
    QED_ERROR_ENCRYPTION = -4,
    QED_ERROR_DECRYPTION = -5,
    QED_ERROR_SIGNATURE_MISMATCH = -6,
    QED_ERROR_KEY_NOT_FOUND = -7,
    QED_ERROR_INVALID_INPUT = -8,
    QED_ERROR_KEY_LIMIT_REACHED = -9
} qed_result_t;

// Hardware signature structure
typedef struct {
    double cpu_frequency;
    uint64_t ram_total;
    uint64_t ram_available;
    uint32_t ram_signature;
    char quantum_noise[QED_QUANTUM_NOISE_LENGTH + 1];
} qed_hardware_sig_t;

// Quantum key structure
typedef struct {
    char key_id[QED_MAX_KEY_ID_LENGTH];
    uint8_t key_data[QED_KEY_LENGTH];
    bool in_use;
} qed_quantum_key_t;

// Main Quantum Encryption Device structure
typedef struct {
    qed_hardware_sig_t hardware_sig;
    qed_quantum_key_t quantum_keys[QED_MAX_KEYS];
    size_t key_count;
    bool initialized;
} qed_device_t;

// Core functions
qed_result_t qed_init(qed_device_t *device);
qed_result_t qed_cleanup(qed_device_t *device);

// Hardware detection functions
qed_result_t qed_detect_cpu_frequency(double *frequency);
qed_result_t qed_detect_ram_signature(qed_hardware_sig_t *hw_sig);
qed_result_t qed_measure_quantum_noise(qed_hardware_sig_t *hw_sig);

// Key generation and management
qed_result_t qed_generate_quantum_key(qed_device_t *device, const char *key_id, 
                                      uint8_t *key_out, size_t key_length);
qed_result_t qed_quantum_wipe(qed_device_t *device, const char *key_id);
qed_result_t qed_quantum_wipe_all(qed_device_t *device);

// Hardware resonance generation
qed_result_t qed_generate_hardware_resonance(const qed_hardware_sig_t *hw_sig, 
                                           uint8_t *resonance_data, size_t length);

// Encryption and decryption functions
qed_result_t qed_quantum_encrypt(qed_device_t *device, const char *key_id,
                                const uint8_t *plaintext, size_t plaintext_len,
                                uint8_t **ciphertext, size_t *ciphertext_len);

qed_result_t qed_quantum_decrypt(qed_device_t *device, const char *key_id,
                                const uint8_t *ciphertext, size_t ciphertext_len,
                                uint8_t **plaintext, size_t *plaintext_len);

// File operations
qed_result_t qed_encrypt_file(qed_device_t *device, const char *key_id,
                             const char *input_path, const char *output_path);

qed_result_t qed_decrypt_file(qed_device_t *device, const char *key_id,
                             const char *input_path, const char *output_path);

// Signature functions
qed_result_t qed_generate_quantum_signature(const qed_hardware_sig_t *hw_sig,
                                          const uint8_t *data, size_t data_len,
                                          uint8_t *signature);

qed_result_t qed_verify_quantum_signature(const qed_hardware_sig_t *hw_sig,
                                        const uint8_t *data, size_t data_len,
                                        const uint8_t *signature);

// Quantum channel (entanglement simulation)
qed_result_t qed_create_quantum_channel(qed_device_t *device, 
                                       const char *partner_device_id,
                                       uint8_t *channel_key, size_t key_length);

// Utility functions
const char* qed_get_error_string(qed_result_t result);
void qed_print_hardware_info(const qed_hardware_sig_t *hw_sig);
void qed_secure_zero(void *ptr, size_t len);

// CLI interface
int qed_cli_main(int argc, char *argv[]);

#endif // QUANTUM_ENCRYPTION_H
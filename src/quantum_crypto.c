/*
 * Quantum Encryption Device (QED) - Cryptographic Operations
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
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/kdf.h>
#include "../include/quantum_encryption.h"

qed_result_t qed_generate_quantum_signature(const qed_hardware_sig_t *hw_sig,
                                          const uint8_t *data, size_t data_len,
                                          uint8_t *signature) {
    EVP_MD_CTX *ctx = NULL;
    unsigned char *combined_data = NULL;
    size_t combined_len;
    size_t noise_len;
    unsigned int sig_len = 0;
    
    if (!hw_sig || !data || !signature || data_len == 0) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    noise_len = strlen(hw_sig->quantum_noise);
    combined_len = data_len + noise_len;
    
    combined_data = malloc(combined_len);
    if (!combined_data) {
        return QED_ERROR_MEMORY;
    }
    
    // Combine data with quantum noise
    memcpy(combined_data, data, data_len);
    memcpy(combined_data + data_len, hw_sig->quantum_noise, noise_len);
    
    // Generate SHA256 signature
    ctx = EVP_MD_CTX_new();
    if (!ctx) {
        free(combined_data);
        return QED_ERROR_ENCRYPTION;
    }
    
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1 ||
        EVP_DigestUpdate(ctx, combined_data, combined_len) != 1 ||
        EVP_DigestFinal_ex(ctx, signature, &sig_len) != 1) {
        EVP_MD_CTX_free(ctx);
        free(combined_data);
        return QED_ERROR_ENCRYPTION;
    }
    
    EVP_MD_CTX_free(ctx);
    free(combined_data);
    
    return QED_SUCCESS;
}

qed_result_t qed_verify_quantum_signature(const qed_hardware_sig_t *hw_sig,
                                        const uint8_t *data, size_t data_len,
                                        const uint8_t *signature) {
    uint8_t computed_signature[QED_SIGNATURE_LENGTH];
    qed_result_t result;
    
    if (!hw_sig || !data || !signature || data_len == 0) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    result = qed_generate_quantum_signature(hw_sig, data, data_len, computed_signature);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    if (memcmp(signature, computed_signature, QED_SIGNATURE_LENGTH) != 0) {
        qed_secure_zero(computed_signature, sizeof(computed_signature));
        return QED_ERROR_SIGNATURE_MISMATCH;
    }
    
    qed_secure_zero(computed_signature, sizeof(computed_signature));
    return QED_SUCCESS;
}

qed_result_t qed_quantum_encrypt(qed_device_t *device, const char *key_id,
                                const uint8_t *plaintext, size_t plaintext_len,
                                uint8_t **ciphertext, size_t *ciphertext_len) {
    uint8_t quantum_key[QED_KEY_LENGTH];
    uint8_t signature[QED_SIGNATURE_LENGTH];
    EVP_CIPHER_CTX *ctx = NULL;
    uint8_t iv[16]; // AES block size
    uint8_t *encrypted = NULL;
    uint8_t *signature_data = NULL;
    size_t signature_data_len;
    int len, final_len;
    qed_result_t result;
    
    if (!device || !key_id || !plaintext || !ciphertext || !ciphertext_len || 
        plaintext_len == 0) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Generate quantum key
    result = qed_generate_quantum_key(device, key_id, quantum_key, QED_KEY_LENGTH);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    // Generate random IV
    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_ENCRYPTION;
    }
    
    // Allocate memory for encrypted data (with padding)
    encrypted = malloc(plaintext_len + 16); // AES block size padding
    if (!encrypted) {
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_MEMORY;
    }
    
    // Initialize encryption context
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(encrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_ENCRYPTION;
    }
    
    // Set up AES-256-CBC encryption
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, quantum_key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_ENCRYPTION;
    }
    
    // Encrypt the data
    if (EVP_EncryptUpdate(ctx, encrypted, &len, plaintext, plaintext_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_ENCRYPTION;
    }
    
    int encrypted_len = len;
    
    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, encrypted + len, &final_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_ENCRYPTION;
    }
    
    encrypted_len += final_len;
    EVP_CIPHER_CTX_free(ctx);
    
    // Prepare data for signature (encrypted data + quantum key)
    signature_data_len = encrypted_len + QED_KEY_LENGTH;
    signature_data = malloc(signature_data_len);
    if (!signature_data) {
        free(encrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_MEMORY;
    }
    
    memcpy(signature_data, encrypted, encrypted_len);
    memcpy(signature_data + encrypted_len, quantum_key, QED_KEY_LENGTH);
    
    // Generate quantum signature
    result = qed_generate_quantum_signature(&device->hardware_sig, 
                                          signature_data, signature_data_len,
                                          signature);
    
    // Clean up signature data
    qed_secure_zero(signature_data, signature_data_len);
    free(signature_data);
    qed_secure_zero(quantum_key, sizeof(quantum_key));
    
    if (result != QED_SUCCESS) {
        free(encrypted);
        return result;
    }
    
    // Allocate final ciphertext (signature + IV + encrypted data)
    *ciphertext_len = QED_SIGNATURE_LENGTH + sizeof(iv) + encrypted_len;
    *ciphertext = malloc(*ciphertext_len);
    if (!*ciphertext) {
        free(encrypted);
        return QED_ERROR_MEMORY;
    }
    
    // Combine signature, IV, and encrypted data
    memcpy(*ciphertext, signature, QED_SIGNATURE_LENGTH);
    memcpy(*ciphertext + QED_SIGNATURE_LENGTH, iv, sizeof(iv));
    memcpy(*ciphertext + QED_SIGNATURE_LENGTH + sizeof(iv), encrypted, encrypted_len);
    
    free(encrypted);
    return QED_SUCCESS;
}

qed_result_t qed_quantum_decrypt(qed_device_t *device, const char *key_id,
                                const uint8_t *ciphertext, size_t ciphertext_len,
                                uint8_t **plaintext, size_t *plaintext_len) {
    uint8_t quantum_key[QED_KEY_LENGTH];
    uint8_t signature[QED_SIGNATURE_LENGTH];
    uint8_t iv[16];
    const uint8_t *encrypted_data;
    size_t encrypted_len;
    uint8_t *signature_data = NULL;
    size_t signature_data_len;
    uint8_t *decrypted = NULL;
    EVP_CIPHER_CTX *ctx = NULL;
    int len, final_len;
    qed_result_t result;
    
    if (!device || !key_id || !ciphertext || !plaintext || !plaintext_len ||
        ciphertext_len < QED_SIGNATURE_LENGTH + 16) { // Minimum size check
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Extract signature
    memcpy(signature, ciphertext, QED_SIGNATURE_LENGTH);
    
    // Extract IV
    memcpy(iv, ciphertext + QED_SIGNATURE_LENGTH, sizeof(iv));
    
    // Extract encrypted data
    encrypted_data = ciphertext + QED_SIGNATURE_LENGTH + sizeof(iv);
    encrypted_len = ciphertext_len - QED_SIGNATURE_LENGTH - sizeof(iv);
    
    // Generate quantum key (should be same as during encryption)
    result = qed_generate_quantum_key(device, key_id, quantum_key, QED_KEY_LENGTH);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    // Prepare data for signature verification (encrypted data + quantum key)
    signature_data_len = encrypted_len + QED_KEY_LENGTH;
    signature_data = malloc(signature_data_len);
    if (!signature_data) {
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_MEMORY;
    }
    
    memcpy(signature_data, encrypted_data, encrypted_len);
    memcpy(signature_data + encrypted_len, quantum_key, QED_KEY_LENGTH);
    
    // Verify quantum signature
    result = qed_verify_quantum_signature(&device->hardware_sig,
                                        signature_data, signature_data_len,
                                        signature);
    
    qed_secure_zero(signature_data, signature_data_len);
    free(signature_data);
    
    if (result != QED_SUCCESS) {
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        printf("❌ Quantum signature mismatch - tampering detected!\n");
        return QED_ERROR_SIGNATURE_MISMATCH;
    }
    
    // Allocate memory for decrypted data
    decrypted = malloc(encrypted_len + 16); // Extra space for padding
    if (!decrypted) {
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_MEMORY;
    }
    
    // Initialize decryption context
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(decrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_DECRYPTION;
    }
    
    // Set up AES-256-CBC decryption
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, quantum_key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(decrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_DECRYPTION;
    }
    
    // Decrypt the data
    if (EVP_DecryptUpdate(ctx, decrypted, &len, encrypted_data, encrypted_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(decrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_DECRYPTION;
    }
    
    int decrypted_len = len;
    
    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, decrypted + len, &final_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(decrypted);
        qed_secure_zero(quantum_key, sizeof(quantum_key));
        return QED_ERROR_DECRYPTION;
    }
    
    decrypted_len += final_len;
    EVP_CIPHER_CTX_free(ctx);
    qed_secure_zero(quantum_key, sizeof(quantum_key));
    
    *plaintext = decrypted;
    *plaintext_len = decrypted_len;
    
    return QED_SUCCESS;
}

qed_result_t qed_create_quantum_channel(qed_device_t *device, 
                                       const char *partner_device_id,
                                       uint8_t *channel_key, size_t key_length) {
    qed_result_t result;
    
    if (!device || !channel_key || key_length == 0) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Generate local hardware resonance for the channel
    result = qed_generate_hardware_resonance(&device->hardware_sig, 
                                           channel_key, key_length);
    if (result != QED_SUCCESS) {
        return result;
    }
    
    if (partner_device_id) {
        printf("🌌 Attempting quantum entanglement with %s...\n", partner_device_id);
        // In a real implementation, this would involve network communication
        // For now, we simulate the delay
        usleep(100000); // 0.1 second delay
    }
    
    return QED_SUCCESS;
}
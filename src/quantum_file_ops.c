/*
 * Quantum Encryption Device (QED) - File Operations
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
#include <sys/stat.h>
#include <unistd.h>
#include "../include/quantum_encryption.h"

static qed_result_t qed_read_file(const char *filepath, uint8_t **data, size_t *size) {
    FILE *file;
    struct stat st;
    
    if (!filepath || !data || !size) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Get file size
    if (stat(filepath, &st) != 0) {
        return QED_ERROR_FILE_IO;
    }
    
    *size = st.st_size;
    if (*size == 0) {
        *data = NULL;
        return QED_SUCCESS;
    }
    
    // Allocate memory for file data
    *data = malloc(*size);
    if (!*data) {
        return QED_ERROR_MEMORY;
    }
    
    // Open and read file
    file = fopen(filepath, "rb");
    if (!file) {
        free(*data);
        *data = NULL;
        return QED_ERROR_FILE_IO;
    }
    
    size_t bytes_read = fread(*data, 1, *size, file);
    fclose(file);
    
    if (bytes_read != *size) {
        free(*data);
        *data = NULL;
        return QED_ERROR_FILE_IO;
    }
    
    return QED_SUCCESS;
}

static qed_result_t qed_write_file(const char *filepath, const uint8_t *data, size_t size) {
    FILE *file;
    
    if (!filepath || !data) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    file = fopen(filepath, "wb");
    if (!file) {
        return QED_ERROR_FILE_IO;
    }
    
    size_t bytes_written = fwrite(data, 1, size, file);
    fclose(file);
    
    if (bytes_written != size) {
        return QED_ERROR_FILE_IO;
    }
    
    return QED_SUCCESS;
}

static bool qed_file_exists(const char *filepath) {
    struct stat st;
    return (stat(filepath, &st) == 0);
}

static bool qed_paths_are_same(const char *path1, const char *path2) {
    char *real_path1, *real_path2;
    bool same = false;
    
    real_path1 = realpath(path1, NULL);
    real_path2 = realpath(path2, NULL);
    
    if (real_path1 && real_path2) {
        same = (strcmp(real_path1, real_path2) == 0);
    }
    
    if (real_path1) free(real_path1);
    if (real_path2) free(real_path2);
    
    return same;
}

qed_result_t qed_encrypt_file(qed_device_t *device, const char *key_id,
                             const char *input_path, const char *output_path) {
    uint8_t *file_data = NULL;
    uint8_t *encrypted_data = NULL;
    size_t file_size, encrypted_size;
    qed_result_t result;
    
    if (!device || !key_id || !input_path || !output_path) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Check if input file exists
    if (!qed_file_exists(input_path)) {
        printf("❌ Error: Input file '%s' does not exist.\n", input_path);
        return QED_ERROR_FILE_IO;
    }
    
    // Check if input and output paths are the same
    if (qed_paths_are_same(input_path, output_path)) {
        printf("❌ Error: Output file cannot be the same as input file.\n");
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Check if output file exists (optional warning)
    if (qed_file_exists(output_path)) {
        printf("⚠️  Warning: Output file '%s' already exists and will be overwritten.\n", output_path);
    }
    
    // Read input file
    result = qed_read_file(input_path, &file_data, &file_size);
    if (result != QED_SUCCESS) {
        printf("❌ File encryption failed: %s\n", qed_get_error_string(result));
        return result;
    }
    
    // Handle empty files
    if (file_size == 0) {
        result = qed_write_file(output_path, NULL, 0);
        if (result == QED_SUCCESS) {
            printf("🔒 File encrypted successfully: %s\n", output_path);
        } else {
            printf("❌ File encryption failed: %s\n", qed_get_error_string(result));
        }
        return result;
    }
    
    // Encrypt file data
    result = qed_quantum_encrypt(device, key_id, file_data, file_size, 
                                &encrypted_data, &encrypted_size);
    
    // Clean up file data
    qed_secure_zero(file_data, file_size);
    free(file_data);
    
    if (result != QED_SUCCESS) {
        printf("❌ File encryption failed: %s\n", qed_get_error_string(result));
        return result;
    }
    
    // Write encrypted data to output file
    result = qed_write_file(output_path, encrypted_data, encrypted_size);
    
    // Clean up encrypted data
    qed_secure_zero(encrypted_data, encrypted_size);
    free(encrypted_data);
    
    if (result != QED_SUCCESS) {
        printf("❌ File encryption failed: %s\n", qed_get_error_string(result));
        return result;
    }
    
    printf("🔒 File encrypted successfully: %s\n", output_path);
    return QED_SUCCESS;
}

qed_result_t qed_decrypt_file(qed_device_t *device, const char *key_id,
                             const char *input_path, const char *output_path) {
    uint8_t *encrypted_data = NULL;
    uint8_t *decrypted_data = NULL;
    size_t encrypted_size, decrypted_size;
    qed_result_t result;
    
    if (!device || !key_id || !input_path || !output_path) {
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Check if input file exists
    if (!qed_file_exists(input_path)) {
        printf("❌ Error: Input file '%s' does not exist.\n", input_path);
        return QED_ERROR_FILE_IO;
    }
    
    // Check if input and output paths are the same
    if (qed_paths_are_same(input_path, output_path)) {
        printf("❌ Error: Output file cannot be the same as input file.\n");
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Check if output file exists (optional warning)
    if (qed_file_exists(output_path)) {
        printf("⚠️  Warning: Output file '%s' already exists and will be overwritten.\n", output_path);
    }
    
    // Read encrypted file
    result = qed_read_file(input_path, &encrypted_data, &encrypted_size);
    if (result != QED_SUCCESS) {
        printf("❌ File decryption failed: %s\n", qed_get_error_string(result));
        return result;
    }
    
    // Check minimum file size for encrypted data
    if (encrypted_size < QED_SIGNATURE_LENGTH + 16) { // signature + IV minimum
        printf("❌ File decryption failed: Input file too small to be encrypted (missing signature)\n");
        free(encrypted_data);
        return QED_ERROR_INVALID_INPUT;
    }
    
    // Decrypt file data
    result = qed_quantum_decrypt(device, key_id, encrypted_data, encrypted_size,
                                &decrypted_data, &decrypted_size);
    
    // Clean up encrypted data
    qed_secure_zero(encrypted_data, encrypted_size);
    free(encrypted_data);
    
    if (result != QED_SUCCESS) {
        printf("❌ File decryption failed: %s\n", qed_get_error_string(result));
        if (result == QED_ERROR_SIGNATURE_MISMATCH) {
            printf("   This could be due to:\n");
            printf("   - Wrong decryption key\n");
            printf("   - File was tampered with\n");
            printf("   - Different hardware than original encryption\n");
        }
        return result;
    }
    
    // Write decrypted data to output file
    result = qed_write_file(output_path, decrypted_data, decrypted_size);
    
    // Clean up decrypted data
    qed_secure_zero(decrypted_data, decrypted_size);
    free(decrypted_data);
    
    if (result != QED_SUCCESS) {
        printf("❌ File decryption failed: %s\n", qed_get_error_string(result));
        return result;
    }
    
    printf("📨 File decrypted successfully: %s\n", output_path);
    return QED_SUCCESS;
}
/*
 * Quantum Encryption Device (QED) - Command Line Interface
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
#include <getopt.h>
#include "../include/quantum_encryption.h"
#include "../include/quantum_evaluation.h"

static void print_usage(const char *program_name) {
    printf("Quantum Encryption Device (QED) v%s\n", QED_VERSION);
    printf("Hardware-Dependent Cryptographic System\n");
    printf("Copyright (C) 2025 Americo Simoes. All rights reserved.\n\n");
    
    printf("Usage: %s [OPTIONS]\n\n", program_name);
    
    printf("Options:\n");
    printf("  -e, --encrypt FILE      Encrypt a file\n");
    printf("  -d, --decrypt FILE      Decrypt a file\n");
    printf("  -o, --output FILE       Output file path\n");
    printf("  -k, --key ID            Key identifier (default: 'default')\n");
    printf("  -w, --wipe [KEY_ID]     Wipe quantum key (or all keys if no ID)\n");
    printf("  -i, --info              Show hardware information\n");
    printf("  -t, --interactive       Interactive mode\n");
    printf("  -h, --help              Show this help message\n");
    printf("  -v, --version           Show version information\n\n");
    
    printf("Examples:\n");
    printf("  %s --encrypt document.pdf --output document.qed\n", program_name);
    printf("  %s --decrypt document.qed --output document.pdf --key mykey\n", program_name);
    printf("  %s --interactive\n", program_name);
    printf("  %s --info\n", program_name);
}

static void print_version() {
    printf("Quantum Encryption Device (QED) v%s\n", QED_VERSION);
    printf("Copyright (C) 2025 Americo Simoes. All rights reserved.\n");
    printf("Hardware-Dependent Cryptographic System Based on Physical Resonance\n");
}

static char* get_user_input(const char *prompt, char *buffer, size_t buffer_size) {
    printf("%s", prompt);
    fflush(stdout);
    
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return buffer;
    }
    
    return NULL;
}

static bool get_user_confirmation(const char *prompt) {
    char response[10];
    
    printf("%s (y/n): ", prompt);
    fflush(stdout);
    
    if (fgets(response, sizeof(response), stdin) != NULL) {
        return (response[0] == 'y' || response[0] == 'Y');
    }
    
    return false;
}

static int run_interactive_mode(qed_device_t *device) {
    char input_file[512];
    char output_file[512];
    char key_id[256];
    char choice[10];
    qed_result_t result;
    
    printf("\n=== Quantum Encryption Tool ===\n");
    
    while (1) {
        printf("\nWhat do you want to do?\n");
        printf("1. Encrypt a file\n");
        printf("2. Decrypt a file\n");
        printf("3. Wipe a key\n");
        printf("4. Show hardware info\n");
        printf("5. Exit\n");
        
        if (!get_user_input("Enter choice (1-5): ", choice, sizeof(choice))) {
            continue;
        }
        
        if (strcmp(choice, "5") == 0) {
            printf("Exiting...\n");
            break;
        }
        
        if (strcmp(choice, "4") == 0) {
            qed_print_hardware_info(&device->hardware_sig);
            continue;
        }
        
        if (strcmp(choice, "3") == 0) {
            if (!get_user_input("Enter key ID to wipe (or press Enter for all keys): ", 
                               key_id, sizeof(key_id))) {
                continue;
            }
            
            if (strlen(key_id) == 0) {
                if (get_user_confirmation("Are you sure you want to wipe ALL keys?")) {
                    result = qed_quantum_wipe_all(device);
                    if (result != QED_SUCCESS) {
                        printf("❌ Error wiping keys: %s\n", qed_get_error_string(result));
                    }
                }
            } else {
                result = qed_quantum_wipe(device, key_id);
                if (result != QED_SUCCESS) {
                    printf("❌ Error wiping key: %s\n", qed_get_error_string(result));
                }
            }
            continue;
        }
        
        if (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0) {
            printf("❌ Invalid choice. Please enter 1, 2, 3, 4, or 5.\n");
            continue;
        }
        
        // Get input file
        if (!get_user_input("Enter input file path: ", input_file, sizeof(input_file))) {
            continue;
        }
        
        // Get output file
        if (!get_user_input("Enter output file path: ", output_file, sizeof(output_file))) {
            continue;
        }
        
        // Get key ID
        if (!get_user_input("Enter key ID (or press Enter for 'default'): ", 
                           key_id, sizeof(key_id))) {
            continue;
        }
        
        if (strlen(key_id) == 0) {
            strcpy(key_id, "default");
        }
        
        // Perform operation
        if (strcmp(choice, "1") == 0) {
            result = qed_encrypt_file(device, key_id, input_file, output_file);
            if (result == QED_SUCCESS) {
                printf("✅ Check the encrypted file: %s\n", output_file);
            }
        } else if (strcmp(choice, "2") == 0) {
            result = qed_decrypt_file(device, key_id, input_file, output_file);
            if (result == QED_SUCCESS) {
                printf("✅ Check the decrypted file: %s\n", output_file);
            }
        }
    }
    
    return 0;
}

int qed_cli_main(int argc, char *argv[]) {
    qed_device_t device;
    qed_result_t result;
    int opt;
    
    // Command line options
    char *encrypt_file = NULL;
    char *decrypt_file = NULL;
    char *output_file = NULL;
    char *key_id = "default";
    char *wipe_key = NULL;
    bool show_info = false;
    bool interactive = false;
    bool wipe_all = false;
    
    static struct option long_options[] = {
        {"encrypt",     required_argument, 0, 'e'},
        {"decrypt",     required_argument, 0, 'd'},
        {"output",      required_argument, 0, 'o'},
        {"key",         required_argument, 0, 'k'},
        {"wipe",        optional_argument, 0, 'w'},
        {"info",        no_argument,       0, 'i'},
        {"interactive", no_argument,       0, 't'},
        {"help",        no_argument,       0, 'h'},
        {"version",     no_argument,       0, 'v'},
        {0, 0, 0, 0}
    };
    
    // Parse command line options
    while ((opt = getopt_long(argc, argv, "e:d:o:k:w::ithv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'e':
                encrypt_file = optarg;
                break;
            case 'd':
                decrypt_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'k':
                key_id = optarg;
                break;
            case 'w':
                if (optarg) {
                    wipe_key = optarg;
                } else {
                    wipe_all = true;
                }
                break;
            case 'i':
                show_info = true;
                break;
            case 't':
                interactive = true;
                break;
            case 'v':
                print_version();
                return 0;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Check evaluation license first
    QED_EVAL_CHECK();
    
    // Show evaluation notice
    QED_EVAL_NOTICE();
    
    // Initialize device
    result = qed_init(&device);
    if (result != QED_SUCCESS) {
        printf("❌ Failed to initialize Quantum Encryption Device: %s\n", 
               qed_get_error_string(result));
        return 1;
    }
    
    // Handle commands
    if (show_info) {
        qed_print_hardware_info(&device.hardware_sig);
    }
    
    if (wipe_all) {
        if (get_user_confirmation("Are you sure you want to wipe ALL quantum keys?")) {
            result = qed_quantum_wipe_all(&device);
            if (result != QED_SUCCESS) {
                printf("❌ Error wiping keys: %s\n", qed_get_error_string(result));
            }
        }
    } else if (wipe_key) {
        result = qed_quantum_wipe(&device, wipe_key);
        if (result != QED_SUCCESS) {
            printf("❌ Error wiping key: %s\n", qed_get_error_string(result));
        }
    }
    
    if (encrypt_file) {
        if (!output_file) {
            printf("❌ Error: Output file must be specified for encryption.\n");
            qed_cleanup(&device);
            return 1;
        }
        
        // Check evaluation license before encryption
        QED_EVAL_CHECK();
        
        result = qed_encrypt_file(&device, key_id, encrypt_file, output_file);
        if (result == QED_SUCCESS) {
            printf("✅ Check the encrypted file: %s\n", output_file);
        }
    }
    
    if (decrypt_file) {
        if (!output_file) {
            printf("❌ Error: Output file must be specified for decryption.\n");
            qed_cleanup(&device);
            return 1;
        }
        
        // Check evaluation license before decryption
        QED_EVAL_CHECK();
        
        result = qed_decrypt_file(&device, key_id, decrypt_file, output_file);
        if (result == QED_SUCCESS) {
            printf("✅ Check the decrypted file: %s\n", output_file);
        }
    }
    
    if (interactive) {
        run_interactive_mode(&device);
    }
    
    // If no specific command was given, run interactive mode
    if (!show_info && !wipe_all && !wipe_key && !encrypt_file && !decrypt_file && !interactive) {
        run_interactive_mode(&device);
    }
    
    // Cleanup
    qed_cleanup(&device);
    return 0;
}

// Main function for standalone executable
int main(int argc, char *argv[]) {
    return qed_cli_main(argc, argv);
}
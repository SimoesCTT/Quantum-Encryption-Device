# Quantum Encryption Device (QED) 🔐

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/SimoesCTT/Quantum-Encryption-Device)
[![License](https://img.shields.io/badge/license-Proprietary-red.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux-green.svg)](#requirements)
[![Sponsor](https://img.shields.io/badge/Sponsor-GitHub%20Sponsors-pink.svg)](https://github.com/sponsors/SimoesCTT)

## Revolutionary Hardware-Dependent Cryptographic System

The **Quantum Encryption Device (QED)** is a groundbreaking cryptographic system that achieves quantum-like entanglement effects through **hardware-dependent encryption**. Unlike traditional mathematical cryptography, QED generates encryption keys from the unique physical characteristics of your machine, creating an unbreakable bond between encrypted files and the hardware they were created on.

### 🌟 Key Features

- **🔗 Hardware Entanglement**: Files can only be decrypted on the exact same hardware
- **⚡ CPU Resonance**: Uses real CPU frequency measurements for key generation
- **🧠 RAM Signatures**: Creates unique identifiers from memory characteristics  
- **🌌 Quantum Noise**: Hardware-based entropy for maximum security
- **🔑 AES-256 Encryption**: Military-grade encryption with quantum signatures
- **💻 Cross-Platform CLI**: Easy-to-use command-line interface
- **📚 Library Support**: Static and shared libraries for integration
- **🛡️ Tamper Detection**: Quantum signatures detect any file modifications

## 🚀 Quick Start

### Prerequisites

```bash
# Fedora/RHEL/CentOS
sudo dnf install gcc openssl-devel make

# Ubuntu/Debian  
sudo apt install gcc libssl-dev make

# Arch Linux
sudo pacman -S gcc openssl make
```

### Installation

```bash
# Clone the repository
git clone git@github.com:SimoesCTT/Quantum-Encryption-Device.git
cd Quantum-Encryption-Device

# Check dependencies
make deps

# Build the system
make all

# Install system-wide (optional)
sudo make install
```

### Basic Usage

```bash
# Interactive mode (easiest)
./bin/qed

# Encrypt a file
./bin/qed --encrypt document.pdf --output document.qed

# Decrypt a file  
./bin/qed --decrypt document.qed --output document.pdf

# Show hardware information
./bin/qed --info
```

## 📖 How It Works

The QED creates a **quantum-like entanglement** between encrypted files and hardware through:

1. **Hardware Detection**: Measures CPU frequency, RAM configuration, and system entropy
2. **Quantum Noise Generation**: Creates unique hardware fingerprints using SHA-256
3. **Resonance Key Generation**: Generates keys using hardware-specific wave functions
4. **Entangled Encryption**: AES-256 encryption with hardware-bound quantum signatures
5. **Tamper Detection**: Verifies hardware match and file integrity on decryption

```
Original File + Hardware Signature = Quantum-Entangled Encrypted File
                     ↓
        Only decryptable on same hardware
```

## 🔧 Command Line Options

```
Usage: qed [OPTIONS]

Options:
  -e, --encrypt FILE      Encrypt a file
  -d, --decrypt FILE      Decrypt a file
  -o, --output FILE       Output file path
  -k, --key ID            Key identifier (default: 'default')
  -w, --wipe [KEY_ID]     Wipe quantum key (or all keys if no ID)
  -i, --info              Show hardware information
  -t, --interactive       Interactive mode
  -h, --help              Show help message
  -v, --version           Show version information
```

### Examples

```bash
# Encrypt with custom key ID
./bin/qed --encrypt secrets.txt --output secrets.qed --key "project-alpha"

# Decrypt with custom key ID  
./bin/qed --decrypt secrets.qed --output secrets.txt --key "project-alpha"

# Wipe all quantum keys (for security)
./bin/qed --wipe

# Show detailed hardware fingerprint
./bin/qed --info
```

## 🏗️ Library Integration

Use QED in your own C projects:

```c
#include <quantum_encryption.h>

int main() {
    qed_device_t device;
    
    // Initialize quantum device
    if (qed_init(&device) != QED_SUCCESS) {
        return 1;
    }
    
    // Encrypt a file
    qed_result_t result = qed_encrypt_file(&device, "default", 
                                         "input.txt", "output.qed");
    
    // Cleanup
    qed_cleanup(&device);
    return (result == QED_SUCCESS) ? 0 : 1;
}
```

Compile with:
```bash
gcc myprogram.c -lqed -lssl -lcrypto -o myprogram
```

## 🛡️ Security Features

### Hardware Dependency
- **CPU Frequency Binding**: Keys tied to processor clock speeds
- **RAM Configuration**: Memory size and availability signatures
- **Quantum Noise**: Hardware-specific entropy generation

### Cryptographic Strength
- **AES-256-CBC**: Industry-standard encryption algorithm
- **SHA-256 Signatures**: Quantum signature verification
- **Secure Key Wiping**: Memory is securely zeroed after use
- **Anti-Tampering**: Hardware signature verification prevents unauthorized access

### Use Cases
- **Secure Document Storage**: Documents that can't leave your machine
- **Hardware-Locked Backups**: Backups only accessible on specific hardware
- **Anti-Theft Protection**: Files become useless on stolen hardware
- **Compliance Requirements**: Meeting strict data locality requirements

## 📊 Performance

- **Encryption Speed**: ~1 MB/s (limited by hardware detection)
- **Decryption Speed**: ~1 MB/s 
- **Memory Usage**: <10 MB for typical operations
- **Hardware Detection**: <100ms initialization time

## 🏢 Commercial Licensing

QED is available under different licensing models:

### 📋 Evaluation License (Free)
- 30-day evaluation period
- Full functionality for testing
- Non-commercial use only

### 💼 Commercial License
- Unlimited commercial use
- Integration into products
- Technical support included
- Custom licensing terms available

### 🎯 Enterprise License  
- Volume licensing
- Priority support
- Customization services
- Training and consulting

**Contact**: [licensing@quantumencryption.dev](mailto:licensing@quantumencryption.dev)

## 💖 Support Development

Support the development of QED:

[![Sponsor](https://img.shields.io/badge/GitHub-Sponsors-pink.svg)](https://github.com/sponsors/SimoesCTT)
[![PayPal](https://img.shields.io/badge/PayPal-Donate-blue.svg)](https://paypal.me/amexsimoes)

- ⭐ **Star this repository**
- 💝 **GitHub Sponsors** - Monthly contributions  
- 💰 **PayPal Donations** - [paypal.me/amexsimoes](https://paypal.me/amexsimoes)
- 💳 **One-time support** - Support specific features
- 🔄 **Contribute** - Submit pull requests
- 🐛 **Report bugs** - Help improve stability

## 📚 Documentation

- [API Documentation](docs/API.md)
- [Technical White Paper](docs/The_Quantum_Encryption_Device.pdf)
- [Integration Guide](docs/INTEGRATION.md)
- [Security Analysis](docs/SECURITY.md)
- [Build Instructions](docs/BUILD.md)

## 🔬 Research Background

Based on original research: *"The Quantum Encryption Device: A Hardware-Dependent Cryptographic System Based on Physical Resonance and Entanglement"* by Americo Simoes, September 2025.

Key innovations:
- Hardware-dependent key generation algorithms
- Physical resonance simulation for cryptographic entropy  
- Quantum-like entanglement effects in classical systems
- Tamper-evident quantum signatures

## ⚖️ Legal Notice

**Copyright (C) 2025 Americo Simoes. All rights reserved.**

This software contains **proprietary algorithms** and **trade secrets**. Patents pending on key innovations. Unauthorized use, reproduction, or distribution is strictly prohibited.

See [LICENSE](LICENSE) and [COPYRIGHT](COPYRIGHT) for complete terms.

## 🤝 Contributing

We welcome contributions! See our [Contributing Guide](CONTRIBUTING.md) for details.

### Development
```bash
# Build debug version
make debug

# Run tests
make test

# Check dependencies
make deps
```

## 📞 Contact & Support

- **GitHub Issues**: Bug reports and feature requests
- **Email**: [licensing@quantumencryption.dev](mailto:licensing@quantumencryption.dev)  
- **Discussions**: GitHub Discussions for community support
- **Security**: [security@quantumencryption.dev](mailto:security@quantumencryption.dev)

---

⚡ **Powered by Quantum Resonance** | 🔐 **Secured by Hardware Entanglement** | 🌌 **Protected by Physical Laws**
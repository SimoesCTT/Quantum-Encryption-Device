Name:           qed
Version:        2.0.0
Release:        1%{?dist}
Summary:        Quantum Encryption Device - Hardware-Dependent Cryptographic System

License:        Proprietary
URL:            https://github.com/SimoesCTT/Quantum-Encryption-Device-Marketing
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  openssl-devel
Requires:       openssl

%description
The Quantum Encryption Device (QED) is a revolutionary cryptographic system 
that achieves quantum-like entanglement effects between files and hardware. 
Unlike traditional encryption, QED-encrypted files can ONLY be decrypted on 
the exact same hardware that encrypted them.

Key Features:
- Hardware-dependent key generation using CPU frequency and RAM signatures
- AES-256 encryption with quantum signatures for tamper detection
- Anti-theft protection - files become useless on different hardware
- Professional C implementation with CLI and library support

This is proprietary software. Copyright (C) 2025 Americo Simoes.
Commercial licensing required. Contact: amexsimoes@gmail.com

%prep
%setup -q

%build
# No build needed - using pre-compiled commercial binary

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}

# Install commercial binary
install -m 755 bin/qed $RPM_BUILD_ROOT%{_bindir}/qed

%files
%{_bindir}/qed

%post
echo "=================================================="
echo "Quantum Encryption Device v%{version} installed!"
echo "=================================================="
echo ""
echo "🔐 PROPRIETARY SOFTWARE - COMMERCIAL LICENSE REQUIRED"
echo ""
echo "This software is licensed, not sold. Usage requires"
echo "a valid commercial license from Americo Simoes."
echo ""  
echo "📧 Contact: amexsimoes@gmail.com"
echo "💳 License: https://paypal.me/amexsimoes/2000"
echo ""
echo "Run 'qed --help' to get started"
echo "Run 'qed --info' to see hardware information"
echo ""
echo "© 2025 Americo Simoes. All rights reserved."
echo "Patent pending. Unauthorized use prohibited."
echo "=================================================="

%changelog
* Mon Sep 16 2025 Americo Simoes <amexsimoes@gmail.com> - 2.0.0-1
- Initial RPM release of Quantum Encryption Device
- World's first hardware-dependent encryption system
- Revolutionary quantum-like entanglement for file security
- Professional C implementation with CLI and library support
- Commercial licensing available - contact amexsimoes@gmail.com
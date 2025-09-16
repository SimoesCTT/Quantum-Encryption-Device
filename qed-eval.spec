Name:           qed-eval
Version:        2.0.0
Release:        1%{?dist}
Summary:        Quantum Encryption Device - 30-day Evaluation Version
License:        Proprietary Commercial Software
URL:            https://github.com/americosimoes/qed-quantum-encryption
Source0:        qed-eval-2.0.0.tar.gz

BuildRequires:  gcc, openssl-devel
Requires:       openssl

%description
The Quantum Encryption Device (QED) - 30-Day Evaluation Version.

Revolutionary quantum encryption technology that provides hardware-entangled
cryptographic security by binding encryption keys to specific CPU frequency
signatures and RAM configurations.

This is a LIMITED 30-DAY EVALUATION VERSION with full functionality but
time-based expiry. Commercial licenses available for $2,000 (binaries) or
$25,000 (full source code with development rights).

For commercial licensing contact: amexsimoes@gmail.com

Features in this evaluation:
- Hardware-bound quantum encryption
- CPU frequency signature binding
- RAM configuration entanglement
- AES-256 quantum-enhanced encryption
- File encryption/decryption with quantum signatures
- Command-line interface
- 30-day evaluation period
- License compliance checks

%prep
%setup -q -n qed-eval-2.0.0

%build
# No build needed - using pre-compiled evaluation binary

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
install -m 755 bin/qed $RPM_BUILD_ROOT%{_bindir}/qed-eval

%files
%{_bindir}/qed-eval

%post
echo ""
echo "=================================================="
echo " QED Quantum Encryption Device - EVALUATION"
echo "=================================================="
echo ""
echo "Thank you for installing the QED evaluation!"
echo ""
echo "⚠️  EVALUATION VERSION - 30 DAY TRIAL PERIOD"
echo ""
echo "This evaluation version provides full QED functionality"
echo "for 30 days from first use, after which it will expire."
echo ""
echo "Usage: qed-eval [options] <command>"
echo ""
echo "🔐 Revolutionary quantum encryption technology"
echo "🔒 Hardware-bound cryptographic security"
echo "⚡ CPU frequency signature binding"
echo "🧠 RAM configuration entanglement"
echo ""
echo "Commercial Licensing Available:"
echo "• Binary License: $2,000 USD"
echo "• Full Source + Dev Rights: $25,000 USD"
echo ""
echo "Contact: amexsimoes@gmail.com"
echo "GitHub: https://github.com/americosimoes/qed-quantum-encryption"
echo ""
echo "=================================================="
echo ""

%changelog
* Mon Dec 23 2024 Americo Simoes <amexsimoes@gmail.com> - 2.0.0-1
- Initial QED evaluation RPM package
- 30-day evaluation version with full functionality
- Hardware-bound quantum encryption technology
- Commercial licensing available
/*
 * Quantum Encryption Device (QED) - Evaluation Version
 * 
 * 30-Day Time-Limited Evaluation License
 * Copyright (C) 2025 Americo Simoes. All rights reserved.
 * 
 * This software is proprietary and confidential. This evaluation version
 * expires 30 days after compilation and is for evaluation purposes only.
 * 
 * Commercial licensing required for production use.
 * Contact: amexsimoes@gmail.com
 */

#ifndef QUANTUM_EVALUATION_H
#define QUANTUM_EVALUATION_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Compilation timestamp (automatically set during build)
#define QED_EVAL_BUILD_TIME __DATE__ " " __TIME__

// 30 days in seconds (30 * 24 * 60 * 60)
#define QED_EVAL_PERIOD_SECONDS (30 * 24 * 60 * 60)

// Function prototypes
int qed_check_evaluation_license(void);
void qed_show_evaluation_notice(void);
void qed_show_license_expired_message(void);
time_t qed_parse_build_time(const char* build_time_str);

// Evaluation license check macro - use in all major functions
#ifdef QED_EVALUATION_VERSION
#define QED_EVAL_CHECK() do { \
    if (!qed_check_evaluation_license()) { \
        qed_show_license_expired_message(); \
        exit(1); \
    } \
} while(0)

// Evaluation notice macro - show at startup
#define QED_EVAL_NOTICE() qed_show_evaluation_notice()
#else
// Commercial version - no evaluation checks
#define QED_EVAL_CHECK() do { } while(0)
#define QED_EVAL_NOTICE() do { } while(0)
#endif

#endif // QUANTUM_EVALUATION_H
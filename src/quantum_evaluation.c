/*
 * Quantum Encryption Device (QED) - Evaluation License Implementation
 * 
 * 30-Day Time-Limited Evaluation License
 * Copyright (C) 2025 Americo Simoes. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/quantum_evaluation.h"

// Parse build time string into time_t
time_t qed_parse_build_time(const char* build_time_str) {
    struct tm tm_build = {0};
    char month_name[12];
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    // Parse format: "Sep 16 2025 11:30:45"
    sscanf(build_time_str, "%s %d %d %d:%d:%d",
           month_name, &tm_build.tm_mday, &tm_build.tm_year,
           &tm_build.tm_hour, &tm_build.tm_min, &tm_build.tm_sec);
    
    // Convert month name to number
    for (int i = 0; i < 12; i++) {
        if (strncmp(month_name, months[i], 3) == 0) {
            tm_build.tm_mon = i;
            break;
        }
    }
    
    tm_build.tm_year -= 1900; // years since 1900
    tm_build.tm_isdst = -1;    // let mktime determine DST
    
    return mktime(&tm_build);
}

int qed_check_evaluation_license(void) {
    time_t build_time = qed_parse_build_time(QED_EVAL_BUILD_TIME);
    time_t current_time = time(NULL);
    
    // Check if current time is within evaluation period
    double seconds_elapsed = difftime(current_time, build_time);
    
    return (seconds_elapsed >= 0 && seconds_elapsed <= QED_EVAL_PERIOD_SECONDS);
}

void qed_show_evaluation_notice(void) {
    time_t build_time = qed_parse_build_time(QED_EVAL_BUILD_TIME);
    time_t current_time = time(NULL);
    time_t expire_time = build_time + QED_EVAL_PERIOD_SECONDS;
    
    double seconds_remaining = difftime(expire_time, current_time);
    int days_remaining = (int)(seconds_remaining / (24 * 60 * 60));
    
    printf("┌─────────────────────────────────────────────────────────────┐\n");
    printf("│                    🔐 QED EVALUATION VERSION                │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  Quantum Encryption Device v2.0.0 - 30 Day Trial          │\n");
    printf("│  Copyright (C) 2025 Americo Simoes. All rights reserved.   │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  📅 Build Date: %s                            │\n", QED_EVAL_BUILD_TIME);
    printf("│  ⏰ Days Remaining: %2d days                               │\n", days_remaining > 0 ? days_remaining : 0);
    printf("│  🎯 Status: ");
    
    if (seconds_remaining > 0) {
        if (days_remaining > 7) {
            printf("ACTIVE ✅                                    │\n");
        } else if (days_remaining > 0) {
            printf("EXPIRING SOON ⚠️                            │\n");
        } else {
            printf("EXPIRES TODAY ⚠️                            │\n");
        }
    } else {
        printf("EXPIRED ❌                                     │\n");
    }
    
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  💼 COMMERCIAL LICENSE REQUIRED FOR PRODUCTION USE         │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  💰 Commercial License: $2,000 USD                         │\n");
    printf("│  💳 Purchase: https://paypal.me/amexsimoes/2000             │\n");
    printf("│  📧 Contact: amexsimoes@gmail.com                           │\n");
    printf("│  🔗 Info: github.com/SimoesCTT/Quantum-Encryption-Device-Marketing │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  This evaluation version is for testing purposes only.     │\n");
    printf("│  Commercial use requires a paid license.                   │\n");
    printf("│  Revolutionary hardware-dependent encryption technology!    │\n");
    printf("└─────────────────────────────────────────────────────────────┘\n");
    printf("\n");
}

void qed_show_license_expired_message(void) {
    printf("┌─────────────────────────────────────────────────────────────┐\n");
    printf("│                        🚨 LICENSE EXPIRED                   │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  Your 30-day evaluation period has ended.                  │\n");
    printf("│  This software can no longer be used.                      │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│                  💰 PURCHASE REQUIRED                       │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  🎯 Commercial License: $2,000 USD                         │\n");
    printf("│  💳 Buy Now: https://paypal.me/amexsimoes/2000              │\n");
    printf("│  📧 Contact: amexsimoes@gmail.com                           │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  ✅ Unlimited commercial use                                │\n");
    printf("│  ✅ 1 year of updates included                              │\n");
    printf("│  ✅ Email support included                                  │\n");
    printf("│  ✅ Professional documentation                              │\n");
    printf("│  ✅ API libraries for integration                           │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  🚀 World's first hardware-dependent encryption!           │\n");
    printf("│  🔐 Files can only decrypt on the same hardware!           │\n");
    printf("│  🛡️ Ultimate anti-theft protection for your data!          │\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│  Don't lose access to this revolutionary technology!       │\n");
    printf("│  Purchase your license today: amexsimoes@gmail.com         │\n");
    printf("└─────────────────────────────────────────────────────────────┘\n");
    printf("\nEvaluation period expired. Exiting.\n");
}

int qed_get_evaluation_days_remaining(void) {
    time_t build_time = qed_parse_build_time(QED_EVAL_BUILD_TIME);
    time_t current_time = time(NULL);
    time_t expire_time = build_time + QED_EVAL_PERIOD_SECONDS;
    
    double seconds_remaining = difftime(expire_time, current_time);
    return (int)(seconds_remaining / (24 * 60 * 60));
}
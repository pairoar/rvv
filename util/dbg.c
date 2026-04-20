/**
 * @file   dbg.c
 * @brief  debugging features
 * @author Seongjin Oh
 * @date   03-17-2026
 */

/* ======================================================================== */
/* Standard Library Headers                                                 */
/* ======================================================================== */
#include <stdarg.h>
#include <stdio.h>

/* ======================================================================== */
/* User-Defined Headers                                                     */
/* ======================================================================== */
#include "dbg.h"

/* ======================================================================== */
/*  Macros (#define)                                                        */
/* ======================================================================== */

/* ======================================================================== */
/* Local Types (enum, struct, typedef)                                      */
/* ======================================================================== */
char *const type[DBG_MAX] = {
    "LOW",
    "MID",
    "HIGH",
    "ERR",
};

char *const colors[DBG_MAX] = {"\x1b[0m", "\x1b[32m", "\x1b[1;33m", "\x1b[31m"};

/* ======================================================================== */
/* Global Variables (Minimize usage recommended)                            */
/* ======================================================================== */

/* ======================================================================== */
/* Static Variables (File-scope global variables)                           */
/* ======================================================================== */

/* ======================================================================== */
/* Static Function Prototypes (Forward declarations)                        */
/* ======================================================================== */

/* ======================================================================== */
/* Public Functions (API Implementations)                                   */
/* ======================================================================== */
void dbg_msg(DBG_LEVEL level, const char *fmt, ...) {
    va_list args = {0};
    va_start(args, fmt);
    time_t current_time = {0};
    struct tm *m_time = NULL;

    time(&current_time);
    m_time = localtime(&current_time);

#ifndef NO_ANSI
    printf("%s", colors[level]);
#endif

    printf("[%02d/%02d/%04d %02d:%02d:%02d][%4s] ", m_time->tm_mday, m_time->tm_mon,
           m_time->tm_year + BASE_YEAR, m_time->tm_hour, m_time->tm_min, m_time->tm_sec, type[level]);
    vfprintf(stdout, fmt, args);
    printf("\n%s", colors[DBG_LOW]);
    va_end(args);
}
/* ======================================================================== */
/* Static Functions (Internal implementations)                              */
/* ======================================================================== */

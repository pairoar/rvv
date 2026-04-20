/**
 * @file   dbg.h
 * @brief  Support debugging
 * @author Seongjin Oh
 * @date   03-17-2026
 */

#ifndef AERMOB_DBG_H
#define AERMOB_DBG_H

#include <time.h>

/* ======================================================================== */
/*    Public Macros & Types                                                 */
/* ======================================================================== */
#define BASE_YEAR       1900

typedef enum { DBG_LOW, DBG_MID, DBG_HIGH, DBG_ERR, DBG_MAX } DBG_LEVEL;

#ifdef __EMSCRIPTEN
#define NO_ANSI
#endif

/* ======================================================================== */
/*    Inline Functions                                                      */
/* ======================================================================== */

/* ======================================================================== */
/*    C++ Name Mangling Prevention (Crucial for C/C++ interop)              */
/* ======================================================================== */
// Tells the C++ compiler to treat these functions as C-style functions,
// preventing it from altering their names (mangling) during compilation.

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*    Public Function Prototypes (Exposed APIs)                             */
/* ======================================================================== */

extern void dbg_msg(DBG_LEVEL level, const char *fmt, ...);

#define DBG_LOW(...) dbg_msg(DBG_LOW, __VA_ARGS__)
#define DBG_MID(...) dbg_msg(DBG_MID, __VA_ARGS__)
#define DBG_HIGH(...) dbg_msg(DBG_HIGH, __VA_ARGS__)
#define DBG_ERR(...) dbg_msg(DBG_ERR, __VA_ARGS__)

/* ======================================================================== */
/*    Close C++ Block                                                       */
/* ======================================================================== */
#ifdef __cplusplus
}
#endif

#endif
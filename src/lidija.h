#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gmp.h>

#define GC_MALLOC malloc
#define GC_REALLOC realloc
#define GC_FREE free

#include "ast.h"
#include "parser.h"
#include "value.h"
#include "closure.h"
#include "funcs.h"
#include "eval.h"
#include "error.h"
#include "debug.h"
#include "lib/base.h"
#include "lib/list.h"
#include "lib/logic.h"
#include "lib/num.h"
#include "lib/str.h"
#include "lib/stream.h"

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define L_ERR_MISSING_LIST  "Must supply a list."
#define L_ERR_MISSING_INDEX "Must supply a list index."
#define L_ERR_MISSING_FUNC  "Must supply a function."

#define L_DEBUG_HEAP 0
#define L_DEBUG_CALL 0

#define L_ENABLE_GC 1

#define L_SKIP_LIB 0

#define L_VERSION "0.1"

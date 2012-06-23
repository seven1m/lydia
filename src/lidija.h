#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <gmp.h>

#include <gc.h>
#define malloc GC_MALLOC
#define realloc GC_REALLOC
#define free GC_free

#include <hashmap.h>
#include <vector.h>
#include <strutils.h>

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

#define L_SKIP_LIB 0

#define L_VERSION "0.1"

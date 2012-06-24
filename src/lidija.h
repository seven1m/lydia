#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <gmp.h>

#include <gc.h>

#include "ds/hashmap.h"
#include "ds/vector.h"
#include "ds/strutils.h"
#include "ds/list.h"

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
#define L_ERR_MISSING_STR   "Must supply a string."

#define L_DEBUG_HEAP 0
#define L_DEBUG_CALL 0

#define L_SKIP_LIB 0

#define L_VERSION "0.1"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gmp.h>
#include "ast.h"
#include "parser.h"
#include "value.h"
#include "heap.h"
#include "closure.h"
#include "funcs.h"
#include "eval.h"
#include "error.h"
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

#define L_DEBUG_HEAP 0

#define L_VERSION "0.1"

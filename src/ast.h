#define ERR_TYPE  0
#define INT_TYPE  1
#define STR_TYPE  2
#define VAR_TYPE  3
#define LST_TYPE  4
#define RNG_TYPE  5
#define FUNC_TYPE 6
#define CALL_TYPE 7
#define ASSN_TYPE 8

struct list {
};

struct range {
};

struct func {
};

struct call {
};

struct assign {
};

struct var {
};

typedef int node_type;

struct node {
  node_type type;
  union {
    int            i;
    char          *s;
    char          *e;
    struct var    *v;
    struct list   *l;
    struct range  *r;
    struct func   *f;
    struct call   *c;
    struct assign *a;
  } value;
};

typedef struct node node;

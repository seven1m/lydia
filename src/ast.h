enum node_type { err_type, num_type, str_type, var_type, list_type, range_type, func_type, call_type, assign_type };

typedef struct node {
  enum node_type type;
  union {
    int                      num;
    char*                    str;
    char*                    err;
    char*                    var;
    struct {
      /* TODO */ }           list;
    struct {
      struct node* first;
      struct node* last; }   range;
    struct {
      /* TODO */ }           func;
    struct {
      char* name;
      int argc;
      struct node** args; } call;
    struct {
      /* TODO */ }           assign;
  } value;
} node;

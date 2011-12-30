enum airb_node_type { err_type, num_type, str_type, var_type, list_type, range_type, func_type, call_type, assign_type };

typedef struct airb_node {
  enum airb_node_type type;
  union {
    int                      num;
    char*                    str;
    char*                    err;
    char*                    var;
    struct {
      int count;
      struct airb_node** items; } list;
    struct {
      struct airb_node* first;
      struct airb_node* last; }   range;
    struct {
      struct airb_node* args;
      int exprc;
      struct airb_node** exprs; } func;
    struct {
      char* name;
      int argc;
      struct airb_node** args; } call;
    struct {
      /* TODO */ }           assign;
  } value;
} airb_node;

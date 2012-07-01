#include <argp.h>
#include "../lidija.h"

const char *argp_program_version = "lidija " L_VERSION;
const char *argp_program_bug_address = "https://github.com/seven1m/lidija/issues";
static char doc[] = "Lidija -- a toy functional language";
static char args_doc[] = "script";

static struct argp_option options[] = {
  //{"debug", 'd', 0, OPTION_ARG_OPTIONAL, "Produce some debug output" },
  { 0 }
};

struct arguments
{
  char *args[255];
  int argc;
  //int debug;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    //case 'd':
    //  arguments->debug = 1;
    //  break;

    case ARGP_KEY_ARG:
      /*if (state->arg_num >= 1)*/
        /*argp_usage(state); // too many args*/
      arguments->args[state->arg_num] = arg;
      arguments->argc++;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        argp_usage (state); // not enough args
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv) {
  // parse args
  struct arguments arguments;
  arguments.argc = 0;
  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  char *filename = arguments.args[0];

  LClosure *closure = l_closure_new(NULL);
  l_closure_set_funcs(closure);

  LValue* f = l_value_new(L_STR_TYPE, closure);
  f->core.str = make_stringbuf((char*)filename);
  l_closure_set(closure, "-script", f, true);

  LValue *arg;
  LValue *args = l_value_new(L_LIST_TYPE, closure);
  args->core.list = create_vector();
  int i;
  for(i=1; i<arguments.argc; i++) {
    arg = l_value_new(L_STR_TYPE, closure);
    arg->core.str = make_stringbuf(arguments.args[i]);
    vector_add(args->core.list, &arg, sizeof(arg));
  }
  l_closure_set(closure, "-args", args, true);

  l_eval_path(filename, closure);

  exit(0);
}

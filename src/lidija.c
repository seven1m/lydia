#include <argp.h>
#include "lidija.h"

const char *argp_program_version = "lidija " L_VERSION;
const char *argp_program_bug_address = "<tim@timmorgan.org>";
static char doc[] = "Lidija -- a toy functional scripting language";
static char args_doc[] = "script";

static struct argp_option options[] = {
  //{"debug", 'd', 0, OPTION_ARG_OPTIONAL, "Produce some debug output" },
  { 0 }
};

struct arguments
{
  char *args[1]; // filename
  //int debug;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    //case 'd':
    //  arguments->debug = 1;
    //  break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
        argp_usage(state); // too many args
      arguments->args[state->arg_num] = arg;
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
  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  char *filename = arguments.args[0];

  // open file
  FILE *fp = fopen(filename, "r");
  if(fp == NULL) {
    printf("An error occurred while opening the file %s.\n", filename);
    exit(1);
  }

  // read source
  GString *source = g_string_new("");
  char buf[1024];
  while(fgets(buf, 1024, fp)) {
    g_string_append(source, buf);
  }
  fclose(fp);

  // eval source
  l_eval(source->str);

  exit (0);
}

#ifndef HB_PARSE
#define HB_PARSE

#include "grammar.h"

/**
 * Parses an input and returns an array of command runs.
 **/
array_t *B_parse_input_runs (char *input, unsigned length);

/**
 * Parses an input and parses commands into run struct.
 **/
void B_parse_input_cmds (char *input, unsigned length, run_t *runs);

/**
 * Parses a path for a given string.
 *
 * @return array with string and length
 * @return 0 if not valid
 **/
array_t *B_parse_input_string (char *input, unsigned length);

/**
 * Parses the arguments in a given string.
 *
 * @return array with string and length
 * @return 0 if not valid
 **/
array_t *B_parse_input_args (char *input, unsigned length);


/**
 * Parses and returns cmd object.
 *
 * @return cmd object on success
 **/
cmd_t *B_parse_cmd (char *input, unsigned length);

#endif

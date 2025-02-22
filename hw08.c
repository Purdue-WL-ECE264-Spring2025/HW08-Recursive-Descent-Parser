#include "hw08.h"

#include <string.h>

/**
 * expr := term expr_rest
 * expr_rest := '+' expr | '-' expr | {}
 *
 * term := factor term_rest
 * term_rest = '*' term | {}
 *
 * factor := int | '(' expr ')'
 */

enum parse_result parse_expr(struct stream *s) { return PARSE_SUCCESS; }

enum parse_result parse_expr_rest(struct stream *s) { return PARSE_SUCCESS; }

enum parse_result parse_term(struct stream *s) { return PARSE_SUCCESS; }

enum parse_result parse_term_rest(struct stream *s) { return PARSE_SUCCESS; }

enum parse_result parse_factor(struct stream *s) { return PARSE_FAILURE; }

enum parse_result is_valid_expr(const char *expr) {
  struct stream s = {.text = expr, .pos = 0, .length = strlen(expr)};
  if (parse_expr(&s) == PARSE_SUCCESS && next(&s).type == EOF) {
    return PARSE_SUCCESS;
  }
  return PARSE_FAILURE;
}
#ifndef HW08
#define HW08

/*

1 - 2

expr = term + expr | term - expr | term
term = factor | factor * term
factor = int | `(` expr `)`
int = ...

A = B C | B
C = x y z | x y w

(1 + 2) * 3

expr := term expr_rest
expr_rest := '+' expr | '-' expr | {}

term := factor term_rest
term_rest = '*' term | {}

factor := int | '(' expr ')'
*/

#include <stddef.h>

enum parse_result { PARSE_SUCCESS, PARSE_FAILURE };
enum token_type { LPAREN, RPAREN, BINOP, LITERAL, EOF };

struct stream {
  const char *text;
  int pos;
  size_t length;
};

struct token {
  enum token_type type;
  const char *start;
  size_t length;
};

struct token peek(struct stream *s);
struct token next(struct stream *s);

void skip_whitespace(struct stream *s);

enum parse_result parse_expr(struct stream *s);
enum parse_result parse_expr_rest(struct stream *s);
enum parse_result parse_term(struct stream *s);
enum parse_result parse_term_rest(struct stream *s);
enum parse_result parse_factor(struct stream *s);

enum parse_result is_valid_expr(const char *expr);
#endif
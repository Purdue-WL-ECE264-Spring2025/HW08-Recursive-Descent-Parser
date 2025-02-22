#include "hw08.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void skip_whitespace(struct stream *s) {
  while (s->pos < s->length && isspace(s->text[s->pos]))
    s->pos++;
}

struct token peek(struct stream *s) {
  skip_whitespace(s);
  if (s->pos == s->length)
    return (struct token){.type = EOF, .start = NULL, .length = 0};
  switch (s->text[s->pos]) {
  case '(':
    return (struct token){
        .type = LPAREN, .start = s->text + s->pos, .length = 1};
  case ')':
    return (struct token){
        .type = RPAREN, .start = s->text + s->pos, .length = 1};
  case '+':
  case '-':
  case '*':
    return (struct token){
        .type = BINOP, .start = s->text + s->pos, .length = 1};
  default:
    if (!isnumber(s->text[s->pos])) {
      fprintf(stderr, "Unrecognized symbol at position %d\n", s->pos);
      exit(1);
    }

    int i;
    for (i = 0; isnumber(s->text[s->pos + i]); i++)
      ;
    return (struct token){
        .type = LITERAL, .start = s->text + s->pos, .length = i};
  }
}

struct token next(struct stream *s) {
  struct token tok = peek(s);
  s->pos += tok.length;
  return tok;
}

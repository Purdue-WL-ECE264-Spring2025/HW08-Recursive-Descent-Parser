# Recursive Descent Parser

## Learning Goals

In this assignment, you will learn a basic parsing technique called _recursive descent parsing_, and implement a recursive descent parser that determines whether a string is a valid arithmetic expression.

## Background

For our purposes, we define valid arithmetic expressions as being built out of integers, the operations `+`, `-`, and `*`, and optional parentheses.
More formally, we define them with the following rules:

```
expr := term expr_rest
expr_rest := '+' expr | '-' expr | {}

term := factor term_rest
term_rest := '*' term | {}

factor := int | '(' expr ')'
```

> **Terminology**: The set of rules defining what strings are valid is called a _grammar_, and each individual rule is called a _production_. Symbols that appear on the left side of a production are called _nonterminals_ (for example, `expr` and `expr_rest` are nonterminals); all other symbols are called `terminals` (for example, `'+'` and `int` are terminals).

These productions recursively define a class of valid strings for each nonterminal.
For example, the first production says an `expr` consists of a `term` followed by an `expr_rest`, and the second production says an `expr_rest` consists of one of the following _variants_:

1. A `'+'` token followed by an `expr`
2. A `'-'` token followed by an `expr`
3. An empty string (represented here by `{}`)

> The grammar presented here uses the `expr_rest` and `term_rest` productions to factor out the recursion. This is to make sure the grammar has unambiguous _first sets_; that is, for each production, peeking at the next token is enough to determine which alternative to use; that is, the grammar is $LL(1)$. [You can read more about $LL(k)$ grammars here!](https://en.wikipedia.org/wiki/LL_grammar)

The rules are _recursive_, which allows arithmetic expressions to be nested arbitrarily deep.
They also "bake in" an order of operations: the string "1 + 2 \* 3" is interpreted as "1" plus "2 \* 3", instead of as "1 + 2" multiplied by "3".

The job of the parser is to ensure that a given string matches the grammar; that is, it can be built out of the rules defined above.
For example, consider the strings below:

```
"10" -> Valid
"450 - 30" -> Valid
"(21 + 2) * 3" -> Valid
"400 +" -> Invalid: the `+` doesn't have a right operand
"(100 - 10" -> Invalid:  `(` doesn't have a matching `)`
```

## Tokenizing

The first step in parsing is _tokenizing_: turning the string of characters into a string of "words" (_tokens_) that represent the individual pieces of an expression. For example, the tokenizing turns a string like

```
(1 + 2) * 3
```

into something like

```
LPAREN LIT(1) BINOP(+) LIT(2) RPAREN BINOP(*) LIT(3) EOF
```

where `LPAREN` and `RPAREN` refer to the characters '(' and ')', `LIT` refers to an integer literal, `BINOP` refers to a binary operation like `+` or `*`, and `EOF` is a special token marking the end of the string.

> It might seem like adding an extra step to turn a fairly simple expression into a bunch of tokens, but this is useful because that way later stages of parsing don't have to worry about things like skipping over white space, or deciding whether "8))" is one word or three (in this case, it's three!).

The tool that tokenizes a string is called a _tokenizer_, or _scanner_. We have provided an implementation of a tokenizer in the file `tokenizer.c`. The tokenizer provides a function called `peek` that "peeks" at the next token without consuming it, and a function called `next` that consumes and returns the next token.

## Recursive Descent Parsing

There are, in general, many algorithms that will determine whether a given string is valid according to a set of rules.
In this assignment, you will implement a basic one called [_recursive descent parsing_](https://en.wikipedia.org/wiki/Recursive_descent_parser).
At a high level, the algorithm tries to match a string against a given production as follows:

1. If there are multiple variants, peek at the next token to decide which variant to use.
2. If/when you have a single variant, try to match everything on the right side in sequence. If anything fails, return a failure; otherwise, return a success. (Note that an "empty string" variant automatically succeeds!)
3. If there are no available variants to use, return a failure.
4. Once you've matched the top level production (in this case, `expr`), make sure there's no more tokens left besides `EOF`.

For example:

1. To match an `expr`, start by matching a `term` and then an `expr_rest`.
2. To match an `expr_rest`, peek at the first token. If its a `'+'` or a `'-'`, consume it and then match an `expr` (note the recursive call!) Otherwise, return a success.

Think about what it means to match a `factor` or `term`!

## Examples

### Valid string

Lets attempt to parse the string "10 \* 3" using the recursive descent algorithm from above. Tokenizing it yields:

```
LIT(10) BINOP(*) LIT(3) EOF
```

and parsing proceeds as follows:

- We start by trying to parse an `expr`. There's only one variant to follow, so we try to parse a `term`.
  - We're trying to parse a `term`; there is also only one variant, so we try to parse a `factor`.
    - `factor` has two variants, so we peek at the next token to decide. Since its a `LIT`, we choose the first variant, consume the `LIT`, and succeed.
  - The `factor` succeeded, so we try to parse a `term_rest`
    - `term_rest` has two variants, so we peek, see a `BINOP(*)`, consume it, and try to parse a `term`
      - We try to parse a `factor`, peek and see a `LIT`, consume it, and succeed
      - We try to parse a `term_rest`, peek, and don't see a `BINOP(*)` so we choose the second variant and immediately succeed.
  - The `factor` and `term_rest` succeeded, so the `term` succeeds
- The `term` succeeded so we try to parse an `expr_rest`
  - There's multiple variants; we peek and don't see a `BINOP(+)` or `BINOP(-)` so we choose the last variant and immediately succeed.
- Since the `term` and `expr_rest` succeeded, `expr` suceeds. The last token left is `EOF` so we're done!

<!-- | Nonterminal | Next token | Action |
| -------- | -------- | -------- |
| `expr` | `LPAREN` | Parse `term` |
| `term` | `LPAREN` | Parse `factor` |
|  -->

### Invalid string

Lets try to parse "10 +", which gets tokenized to:

```
LIT(10) BINOP(+) EOF
```

- We start by trying to parse an `expr`
  - We try to parse a `term`
    - We try to parse a `factor`, see a `LIT`, consume it, and suceed
    - We try to parse a `term_rest`, don't see a `BINOP(*)`, and succeed
  - We try to parse an `expr_rest`
    - We see a `BINOP(+)` so we consume it and try to parse an `expr`
      - We try to parse a `term`
        - We try to parse a `factor`. We peek and see an `EOF` which doesn't match either variant, so we fail!
      - We fail!
    - We fail!
  - We fail!
- We fail!

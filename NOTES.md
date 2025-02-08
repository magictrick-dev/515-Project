# Notes for NCC

Various notes for CSCI515 course related to completing the assignments.

## UTF-8 Encoding

Represented as `\u _ _ _ _ _ _`. Check RFC-3629.

`0xxxxxxx` One byte, ASCII range.

`110xxxxx 10xxxxxx` Two byte. 

`1110xxxx 10xxxxxx 10xxxxxx` Three byte.

`11110xxx 10xxxxxx 10xxxxxx 10xxxxxx` Four byte.

Check [Unicode.org](home.unicode.org).

Eg. U+1F3A4, microphone: `\u01F3A4`.

`1|F3A4`, 3 byte format.

=> `1110xxx 10xxxxxx 10xxxxxx` Starting format.

=> `0x1 -> 0b0001`
=> `0xF -> 0b1111`
=> `0x3 -> 0b0011`
=> `0xA -> 0b1010`
=> `0x4 -> 0b0100`

This is encoded as: `11110001 10011111 10001110 10100100`. Bit shift the number
`0x1F3A4` right 6, right 6, right 6, right 3. This constructs the unicode in the
right places. In hex: `0xF09F8EA4`.

## Fractional Decimals

Take for example `17.45`.

`10001` Is the integer portion.

`0.45 * 2 -> 0.90 * 2 -> 1.80`
0 -> 90
1 -> 1.80
1 -> 1.60
1 -> 1.20
0 -> 0.40
0 -> 0.80
1 -> 1.60 (Repeats)

.`011100` with `.--1100` repeats until out of digits.

Another example is:

`0.1`
0 -> 0.2
0 -> 0.4
0 -> 0.8
1 -> 1.60
1 -> 1.20
0 -> 0.40
Where `.-0011` repeats.

## Parse Trees

Translate lower order structures to higher order structures; specifically a tree.
Think grammar.

Context Free Grammars

G -> s
G -> sRt
R -> ss
R -> ts
R -> tt
R -> tRs

Is 'tsss' in the language?

Two different ways to handle this: top-down or bottom-up.

```
// Bottom up.
R   -> t
    -> s

R   -> s
    -> s

R   -> t
    -> R    -> s
            -> s
    -> s

```

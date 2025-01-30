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
Common Element Types
====================================================================================================

**This document is currently a working sketch. There's enough here to get an idea of how you might
use it, but I haven't taken great care to include a good set of useful basic types, nor eliminate
any holes or contradictions.**

Since LSON supports arbitray element types, it is useful to standardize these types, their
associated type IDs, and their grammars. Note that these are only suggestions for common
interpretations. If you disagree with a particular representation, feel free to define your own.

- [Null]
- [Boolean Types]
  + [Boolean (simple)]
  + [Boolean (extended)]
- [Numeric Types]
  + [Count]
  + [CountN]
  + [Int]
  + [IntN]
  + [Real]
  + [RealN]
  + [Real Extended]
  + [Number]
- [Color]


Null
-----
The null type has only a single value: `null`. This value may be represented with the string `null`,
or with `∅`, the Unicode character U+2205 (EMPTY SET: null set).


Boolean Types
--------------

### Boolean (simple)
The simple boolean type has only two values: `true` and `false`.

### Boolean (extended)
The extended boolean type has the following values:

| True      | False      |
|:---------:|:----------:|
| `true`    | `false`    |
| `yes`     | `no`       |
| `on`      | `off`      |
| `T`       | `F`        |
| `enable`  | `disable`  |
| `enabled` | `disabled` |

### Ternary
Also known as trinary, trivalent, or trilean, this type has values `true`, `false`, and `unknown`.


Numeric Types
--------------

### Count
Counting numbers are unsigned integer values, including zero. The syntax is as follows:

    "0" | [1-9][0-9]*

In other words, any sequence of decimal digits, without leading zeros, in order to avoid confusion
with the C convention of using leading zero to denote octal values.

In addition, counting numbers support hexadecimal notation, as follows:

    "0x"[0-9]+

For this element type, the ultimate size is unhandled. If an application is unable to support a
particular count element, it must preserve the value's string representation on output.

Because these values can collide with other numeric types, bare word expressions are sensitive to
the exact order of element type recognition, which the application must make clear.

### CountN
These element types denote the storage size for a particular value, where N denotes the number of
bits. For example, `count8` stores unsigned 8-bit values, and `count64` would be unsigned 64-bit
values. The particular set of supported variants is up to the application. Because a specific size
is specified, it is undefined how any particular application handles values too large to fit (such
as `(count8:772)`).

### Int
Integer types are signed values. Values take the following form:

    "0" | [+-][1-9][0-9]*

Leading zeros are disallowed to avoid confusion with some notations that interpret values with
leading zeros as octal.

### IntN
Like the `countN` type, the `intN` type specifies the storage size for values, where N is the number
of bits. `int8` refers to an 8-bit signed value, and so forth. The set of supported variants is up
to the application. It is undefined how a particular application handles values that are too large
to fit the specified storage size (such as `(int16:-923445221)`).

### Real
The `real` data type specifies real-valued numbers. Such numbers can be expressed using integers,
decimal points, or scientific notation (C style).

### RealN

### Real Extended

### Number


Color (CSS)
------------



[Null]:               #null
[Boolean Types]:      #boolean-types
[Boolean (simple)]:   #boolean-simple
[Boolean (extended)]: #boolean-extended
[Numeric Types]:      #numeric-types
[Count]:              #count
[CountN]:             #countn
[Int]:                #int
[IntN]:               #intn
[Real]:               #real
[RealN]:              #realn
[Real Extended]:      #real-extended
[Number]:             #number
[Color]:              #color

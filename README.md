Humon (Human-Friendly Object Notation)
================================================================================

Humon is a concise data representation that has the simplicity and
expressiveness of JSON, but differs in two primary areas:

  1. It's intended to be concise and friendly to _both_ computers and humans,
     and

  2. It does not aim to mirror JavaScript.

Any legal JSON string can be encoded as a Humon string.


Key Differences from JSON
-------------------------

1. Rich comments are supported.
2. Commas are treated as whitespace. Put them anywhere you want, or nowhere.
3. String quoting is optional when unnecessary.
4. New special values are supported (NaN, Infinity).
5. String values may be broken across lines.
6. Block strings are supported.
7. CSV-like tabular notation without row brackets?


Sample Humon File
------------------

    (First, notice that you can have comments. Take that, JSON.)

    ((  Double-parentheses comments require whitespace after the opener and
        before the closer (new lines count as whitespace). This means that
        including ((doubled)) parentheses is fine as well (since they're only
        closed with "<whitespace>))"). This is probably the best form for block
        comments. If you must have multiple closing parentheses in your block
        comment, then just use ") )" or any other "escape" character, like this:
        ((x)_).
    ))

    commas: "are" just: "white space"


Reserved Words
--------------

The following words are reserved in Humon:

  - `null`
  - `true`
  - `false`
  - `NaN`
  - `Infinity`


Character Escapes
-----------------

Strings may contain the following escaped characters:

| Sequence   | Description
|:-----------|:-----------------------------------------------------
| `\\`       | (backslash)
| `\"`       | (double quote)
| `\'`       | (single quote)
| `\n`       | (new line)
| `\r`       | (carriage return)
| `\t`       | (horizontal tab)
| `\u####`   | (Unicode character with four hexadecimal digits)
| `\U######` | (Unicode character with six hexadecimal digits)


Humon to JSON Examples
-----------------------



CSV Format
----------

    [ Count Dollars Name "Full Address" Status ]
    [ 1 23.33 "Roger Smith" "123 Maple Ave Anytown XX" blue  ]
    [ 7 38.18 "Jim Blarg"   "456 Rickl Ave Anytown XX" green ]
    [ 3 92.15 "Penelope X"  "789 Furpl Ave Anytown XX" red   ]
    ...

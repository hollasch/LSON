LSON: Lucid Serialized Object Notation
====================================================================================================

1. Introduction
2. Example LSON
3. Whitespace
4. Comments
5. Strings
   1. Quoted Strings
   2. Unquoted Strings
   3. Escape Sequences
   4. String Concatenation
6. Numbers
7. Reserved Values
8. Arrays
9. Dictionaries
10. Structures
11. Appendix
    1. Macros / Definitions (Abandoned)


Introduction
------------

LSON is a concise data representation that has the simplicity and expressiveness of JSON, but
differs in two primary areas:

  1. It's intended to be concise and friendly to humans as well as computers,
  2. It does not aim to mirror JavaScript.

Any legal JSON content can be interpreted as legal LSON.


### Key Differences from JSON
1. Rich comments are supported.
2. Commas are treated as whitespace. Put them anywhere you want, or nowhere.
3. String quoting is optional when unnecessary.
4. Special values are handled seamlessly (_e.g._ NaN, infinity, undefined, 0xfffe, #ff8800).
5. String values may be broken across lines.
6. Includes templated objects (structures).


Example LSON
------------
```
(Comments are delimited by parentheses, or by doubled parentheses.)

(( This is an example using doubled parentheses. They must be separated by whitespace. ))

{
    glossary: {
        title: 'example glossary' (There are six legal string-delimeter pairs.)
        "Gloss Div": {
            title: S
            "Gloss List": {
                "Gloss Entry": {
                    (Strings may be unquoted as long as they contain no whitespace.)
                    ID: SGML

                    SortAs:  SGML
                    Acronym: SGML
                    "Gloss Term": "Standard Generalized Markup Language"

                    (Unquoted strings may contain whitespace if escaped.)
                    Abbrev: ISO\ 8879:1986

                    "Gloss Def": {
                        para: "A meta-markup language, used to create markup languages "
                            + "such as DocBook."

                        (Note that commas and semicolons are considered whitespace.)
                        "Gloss SeeAlso": [ GML, XML ];
                        "Gloss See": markup;
                    }
                }
            }
        }
    }
}
```

```
(An Example Menu Description)
{
    menu: {
        id:    file;
        value: file;
        popup: {
            menuitem <value, onclick>: [       (Structured objects)
                < New,   CreateNewDoc() >
                < Open,  OpenDoc()      >
                < Close, CloseDoc()     >
            ]
        };
    }
}
```

```
{
    widget: {
        debug: on
        "debug:Level"=  1.0      (The literal string value "1.0")
        "debug:Weight": Infinity (Converts to floating-point +infinity if understood, else string)
        "debug:Prefix": null     (Converts to null value if understood, otherwise string)
        "debug:Mask":   0xffe0   (Converts to hex number value if understood, else string)

        window: {
            title:  "Sample Konfabulator Widget"
            name:   main_window
            width:  500  (Converts to number value if understood)
            height: 500
        }
        image: {
            src:       Images/Sun.png
            name:      sun1
            hOffset:   250
            vOffset:   250
            alignment: center
        }
        text: {
            data:      Click\ Here
            size:      36
            style:     bold
            name:      text1
            hOffset:   250
            vOffset:   100
            alignment: center
            onMouseUp: "sun1.opacity = (sun1.opacity / 100) * 90;"
        }
    }
}
```


Whitespace
----------
In addition to the standard Unicode whitespace characters, LSON also considers commans and
semicolons to be whitespace:

| Unicode | Escape | Description
|:-------:|:------:|:-----------
| U+0009  |  \t    | Tab
| U+000a  |  \n    | Newline, or line feed
| U+000b  | \u000b | Vertical tab
| U+000c  |  \f    | Form feed
| U+000d  |  \r    | Carriage return
| U+0020  | \u0020 | Standard space character
| U+002c  | \u002c | Comma
| U+003b  | \u003b | Semicolon
| U+0085  | \u0085 | Next line
| U+00a0  | \u00a0 | No-break space
| U+1680  | \u1680 | Ogham space mark
| U+2000  | \u2000 | En quad
| U+2001  | \u2001 | Em quad (mutton quad)
| U+2002  | \u2002 | En space (nut)
| U+2003  | \u2003 | Em space (mutton)
| U+2004  | \u2004 | Three-per-em-space (thick space)
| U+2005  | \u2005 | Four-per-em-space (mid space)
| U+2006  | \u2006 | Six-per-em-space
| U+2007  | \u2007 | Figure space
| U+2008  | \u2008 | Punctuation space
| U+2009  | \u2009 | Thin space
| U+200a  | \u200a | Hair space
| U+2028  | \u2028 | Line separator
| U+2029  | \u2029 | Paragraph separator
| U+202f  | \u202f | Narrow no-break space
| U+205f  | \u205f | Medium mathematical space
| U+3000  | \u3000 | Ideographic space


Comments
--------
```
    (Anything in parentheses is a comment. Take that, JSON.)

    ((  Double-parentheses comments require whitespace after the opener and
        before the closer (new lines count as whitespace). This means that
        including ((doubled)) parentheses is fine as well (since they're only
        closed with "<whitespace>))"). This is probably the best form for block
        comments. If you must have multiple closing parentheses in your block
        comment, then just use ") )" or any other "escape" character, like this:
        ((x)_).
    ))
```


Strings
-------

### Quoted Strings

Strings may be quoted with any of the following pairs:

|  Quotes    | Character Codes                                                      |
|:----------:|:---------------------------------------------------------------------|
|  "string"  |  U+0022 U+0022 (_Quotation Mark_)                                    |
|  'string'  |  U+0027 U+0027 (_Apostrophe_)                                        |
|  «string»  |  U+00ab U+00bb (_{Left,Right}-Pointing Double Angle Quotation Mark_) |
|  ‘string’  |  U+2018 U+2019 (_{Left,Right} Single Quotation Mark_)                |
|  “string”  |  U+201c U+201d (_{Left,Right} Double Quotation Mark_)                |
|  ‹string›  |  U+2039 U+203a (_Single {Left,Right}-Pointing Angle Quotation Mark_) |

### Unquoted Strings
Strings require quotes only if they contain unescaped whitespace. Remember that in LSON, commas and
semicolons are considered whitespace.

It it important to note that this feature is more useful than just as a shorthand for more efficient
representation: it also provides an excellent mechanism for handling special values.

For example, consider the values `null`, `undefined`, `QNaN`, `SNaN`, and `maybe`. Various targets
will be able to support subsets of this set. For example, C++ has no concept of `undefined`, `null`
only applies to pointers, and has no support for`Maybe` (an indefinite Boolean value).
In JavaScript, a `NaN` value is a `QNaN`, doesn't provide a way to create `SNaN`, and doesn't
support `Maybe`. One of the weaknesses of standard JSON is that it implicitly favors JavaScript's
data model.

Since single-word strings don't require quotes, special values can be naturally expressed in LSON,
and interpreters can handle them as native values, or fall back to string values. In order to make
such values durable, it is best if encoders always omit quotes when possible.

For example, consider the following fragment:

    {
        redLevel: null
        blueLevel: undefined
        isElevated: true
        isReady: maybe
    }

In this case, a JavaScript decoder would yield the following object:

    {
        redLevel: null,
        blueLevel: undefined,
        isElevated: true,
        isReady: "maybe"
    }

A C++ decoder might yield the following equivalent:

    struct {
        char *redLevel;
        char *blueLevel;
        bool  isElevated;
        char *isReady;
    } {
        "null", "undefined", true, "maybe"
    };

**NOTE**: This allows special (and arbitrarily complex) values to be introduced at will, though it
is likely that decoders will interpret these values as strings.

### Escape Sequences
Strings may contain the following escape sequences:

| Sequence   | Description                                    |
|:-----------|:-----------------------------------------------|
| `\n`       | new line                                       |
| `\r`       | carriage return                                |
| `\t`       | horizontal tab                                 |
| `\u####`   | Unicode character with four hexadecimal digits |
| `\U######` | Unicode character with six hexadecimal digits  |
| `\<any>`   | Yields that character unchanged                |


### String Concatenation
In order to support human-readable long strings, the `+` operator may be used to construct
concatenations. For example:

    {
        strBlock: "Knock knock.\n"
                + "Who's there?\n"
                + "Bug in your state machine.\n"
                + "Who's there?\n"
    }

Note that the concatenation operator always promotes all values to strings. Non-string values are
promoted to strings of their verbatim representation. Thus:

    X: 1.000 + null + false    (Gets the string value "1.000nullfalse")


Numbers
-------
Numbers can be expressed in standard exponential notation, as in JSON. In addition, numbers can be
expressed with leading or trailing decimal points, can begin with a plus sign. The number notation
is more like C than like JSON — there are many reasonable values that are illegal in JSON.

|  Value  | Legal JSON? |
|--------:|:-----------:|
|      0  |     Yes     |
|    0.1  |     Yes     |
|     .1  |     No      |
|    -10  |     Yes     |
|    +10  |     No      |
|     1.  |     No      |
|   1.2e2 |     Yes     |
|  1.2e-2 |     Yes     |
|  1.2e+2 |     Yes     |

Note that alternate number formats, such as `0xffeb` or `0b00100001` will fail to be recognized as
LSON numbers, and will instead be interpreted as unquoted strings. Optional string quoting then
allows such numbers can be interpreted naturally by a particular interpreter (such as C++), so that
these special formats can be recognized & processed natively.


Reserved Values
---------------
The following special values are reserved in LSON:

  - `null`
  - `true`
  - `false`
  - `NaN`
  - `infinity`

In other words, `null` always means the value `null`, and never the string `"null"`. Similarly for
all other reserved values.


Arrays
------
Arrays encode ordered lists of items. They have the following properties:

1. They begin with a left square bracket (`[`, `U+005b`), followed by zero or more values, and
   terminated with a right square bracket (`]`, `U+005d`).

2. Array values may be strings, arrays, or dictionaries.

3. Array values are separated by whitespace.

4. Arrays are contiguous. That is, there is no way in LSON to indicate an undefined element. For
   example, the LSON value `[ a, b,,, e ]` yields the array `[ "a", "b", "e" ]`. In this example,
   `a`, `b` and `e` are considered unquoted strings, and commas and spaces are considred whitespace.
   If sparse arrays are desired for a particular encoding, it is recommended that dictionaries be
   used with numeric key values. Encoders should follow the same convention, encoding sparse arrays
   as dictionaries.


Dictionaries
------------
Dictionaries (referred to as objects in JSON) are sets of key-value pairs. Keys are string values,
and hence may be either quoted or unquoted. Dictionaries have the following properties:

1. They begin with a left curly bracket (`{`, `U+007b`), followed by zero or more key-value pairs,
   followed by a right curly bracket (`}`, `U+007d`).


Structures
----------
The following fragment:

    {
        someStruct <key1 key2 key3>: [
            < thing1 false  3 >
            < thing2 false 13 >
            < thing3 true  37 >
        ]
    }

is a concise way to express the following:

    {
        someStruct: [
            {   key1: thing1
                key2: false
                key3: 3
            }
            {   key1: thing2
                key2: false
                key3: 13
            }
            {   key1: thing3
                key2: true
                key3: 37
            }
        ]
    }

If more values are given for a row than were specified in the structure template, they will be
ignored. If fewer values are given, they will be interpreted as `null`.

As a side note, this is _not_ the most efficient way to express CSV. That would be more like this:

    {
        fields: [ key1 key2 key3 ]
        rows: [
            [ thing1 false  3 ]
            [ thing2 false 13 ]
            [ thing3  true 37 ]
        ]
    }

or just this (where row 0 is special):

    [
        [ key1 key2 key3 ]
        [ thing1 false  3 ]
        [ thing2 false 13 ]
        [ thing3  true 37 ]
    ]


Appendix
--------

### Macros / Definitions (abandoned)
Macros improve readability, decrease errors, and significantly improve maintainability. Adding
macros to object notation, however, should not be taken lightly, as this introduces all sorts of
non-trivial complexity. For example:

- Should definitions be scoped?
- Should there be an "undef" mechanism?
- Should they be treated as a separate mechanism (like C's CPP) or integrated into the language?
- Can definitions cascade?

The first obvious addition would be a simple id-value replacement. For example:

    {
        lessThan    = -1
        equalTo     =  0
        greaterThan =  1

        [
            [  20,    5, greaterThan ]
            [  17,   47, lessThan ]
            [ 100, -200, greaterThan ]
            [  40,   40, equalTo ]
        ]
    }

There's a decision to be made about legal right-hand sides of a macro definition, which gets into
all of the arcane aspects of the C++ processor, such as token pasting. For example, what about this:

    {
        start = {
        end = }

        start blah:blech end
    }

Of course, that then gets into scoping of the definition statement. It is to end of line? If so,
should we introduce a line continuation operator? Ugh.

And what about tricks like this (using C++'s CPP token-pasting operator ##):

    {
        foo = is
        v0  = False
        v1  = True
        isFalse = false
        isTrue  = true

        x: foo##v0    (foo##v0 -> isFalse -> false)
        y: foo##v1    (foo##v1 -> isTrue  -> true )
    }

Ugh again.

This hasn't even gotten into parameterized definitions like this:

    {
        !! logMsg <type msg> = "[" + type + "] " + msg

        noFile:   !logMsg <ERROR   "Can't find file">
        openFail: !logMsg <ERROR   "Can't open file">
        dateFail: !logMsg <WARNING "Couldn't update file date">
    }

And so forth.

However, just because this could get vastly more complicated, that doesn't mean we should close the
door on every subset. It should be pretty clear that simple value definition is immediately and
widely useful, and relatively easy to implement. Undefining a variable is as easy as restoring
default behavior:

    previously_defined_definition = "previously_defined_definition"

Definitions can have two scopes: global and object/array. Definitions can appear anywhere in an
object, but must occur before use.

Definitions can cascade, but each definition is resolved _immediately_ as it is encountered. If it
uses other definitions, their already-resolved values are used in the new definition. This allows a
large degree of composition at minimal cost and without risk of infinite recursion. For example:

    A = B             (A = "B")
    B = A + foo       (B = "Bfoo")
    C = A             (C = "B")
    C = C + B         (C = "BB")
    D = D + A         (D = "DB")
    B = A + C         (B = "BBB")

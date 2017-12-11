LSON: Lucid Serialized Object Notation
====================================================================================================

1. [Introduction]
   1. [Key Differences from JSON]
   2. [Special Values]
2. [Example LSON]
3. [Whitespace]
4. [Comments]
5. [Strings]
   1. [Escape Sequences]
   2. [String Concatenation]
6. [Words]
   1. [Word Concatenation]
7. [Arrays]
8. [Dictionaries]
9. [Structures]


Introduction
------------
LSON is a concise data representation that has the simplicity and expressiveness of JSON, but
differs in two primary areas:

  1. It’s intended to be concise and friendly to humans as well as computers,
  2. It does not aim to mirror JavaScript.

LSON is a superset of JSON: Any legal JSON is legal LSON.

LSON expresses data using five primitives: words, strings, arrays, dictionaries, and
structures. It has no inherently special values like `true`, `false`, `null`, or numbers, and
instead uses generic _words_ to express values that may have additional meaning and semantics to
encoders beyond their string value.


### Key Differences from JSON
1. LSON supports comments.
2. Commas (and semicolons) are treated as whitespace. Put them anywhere you want, or nowhere.
3. String quoting is optional for words (strings without whitespace).
4. All possible special (word) values are handled seamlessly (_e.g._ `NaN`, `infinity`, `undefined`,
   `maybe`, `0xfffe`, `#ff8800`).
5. LSON supports templated objects (structures).


Example LSON
------------
Following are some example LSON snippets to illustrate various 
```
// Comments are C-style: double slash to end of line, or enclosed with `/*` and `*/`.

/* This is an example using slash-star delimeters. */

{
    glossary: {
        title: 'example glossary'  // There are six legal string-delimeter pairs.
        "Gloss Div": {
            title: S
            "Gloss List": {
                "Gloss Entry": {
                    /* Strings may be unquoted as long as they contain no whitespace. */
                    ID: SGML

                    SortAs:  SGML
                    Acronym: SGML
                    "Gloss Term": "Standard Generalized Markup Language"

                    // Unquoted strings may contain whitespace if escaped.
                    Abbrev: ISO\ 8879:1986

                    "Gloss Def": {
                        para: "A meta-markup language, used to create markup languages "
                            + "such as DocBook."

                        // Note that commas and semicolons are considered whitespace.
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
// An Example Menu Description
{
    menu: {
        id:    file;
        value: file;
        popup: {
            menuitem <value, onclick>: [       // Structured objects
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
        "debug:Level"=  1.0      // The literal string value "1.0"
        "debug:Weight": Infinity // Converts to floating-point +infinity if understood, else string
        "debug:Prefix": null     // Converts to null value if understood, otherwise string
        "debug:Mask":   0xffe0   // Converts to hex number value if understood, else string

        window: {
            title:  "Sample Konfabulator Widget"
            name:   main_window
            width:  500  // Converts to number value if understood
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
LSON whitespace includes all
[standard Unicode whitespace characters](https://en.wikipedia.org/wiki/Whitespace_character#Unicode),
as well as commas and semicolons:

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
    // Single line comments run from double forward slashes to end of line.

    /*  Slash-star comments: this is probably the best form for block
        comments. */


Strings
-------
Strings are delimited with any of the following character pairs:

|  Quotes    | Character Codes                                                      |
|:----------:|:---------------------------------------------------------------------|
|  "string"  |  U+0022 U+0022 (_Quotation Mark_)                                    |
|  'string'  |  U+0027 U+0027 (_Apostrophe_)                                        |
|  «string»  |  U+00ab U+00bb (_{Left,Right}-Pointing Double Angle Quotation Mark_) |
|  ‘string’  |  U+2018 U+2019 (_{Left,Right} Single Quotation Mark_)                |
|  “string”  |  U+201c U+201d (_{Left,Right} Double Quotation Mark_)                |

### Escape Sequences
Strings may contain the following escape sequences:

| Sequence   | Description                                        |
|:-----------|:---------------------------------------------------|
| `\0`       | Null byte                                          |
| `\n`       | new line                                           |
| `\r`       | carriage return                                    |
| `\t`       | horizontal tab                                     |
| `\u####`   | Unicode character with four hexadecimal digits     |
| `\U######` | Unicode character with six hexadecimal digits      |
| `\<any>`   | Yields that character unchanged, such as `\'` or `\\`  |

### String Concatenation
In order to support human-readable long strings, the `+` operator may be used to construct
concatenations. For example:

    {
        strBlock: "Knock knock.\n"
                + "Who's there?\n"
                + "Bug in your state machine.\n"
                + "Who's there?\n"
    }


Words
-----
Words are unquoted strings. For example,

    node: {
        id:       1223-02
        class:    sphere
        weight:   112.23e-6
        previous: null
        next:     0xff128bc5
    }

It it important to note that this feature is more useful than just as a shorthand for string values:
it also provides an excellent mechanism for conveying arbitrary special values.

JSON defines several special values: `true`, `false`, `null` and numbers. Numbers are a _subset_ of
legal JavaScript (the “J” in JSON) representations. They lack, for example, numbers of the form
".12", where JSON requires a leading zero. In addition, the IEEE special values `NaN` and `Infinity`
are unsupported. Other JavaScript values, such as `0x77` and `undefined` are also lacking.

The elegance of JSON, however, has given rise to its overwhelming success as a data interchange
format for all kinds of situations. In this sense, it's as useful for Python or C++ as it is for
JavaScript. Given this, however,  what to do about Python's `None`, CSS's `#23ec98`, C++'s `0xfffe`
or Scala's `Any`? The temptation for those who wish to expand on JSON is to formalize these special
values, usually starting with the introduction of additional JavaScript values.

LSON takes a different approach. Instead of adding additional value types, LSON handles all of them
as simple bare words. This approach provides implicit support for any and all special value types
where it makes sense – the encoders and decoders decide. If a decoder does not understand a special
value (for example, a C++ parser that encounters the word `#ff7e22`), the word is simply interpreted
as the string value `"#ff7e22"`. A decoder that understands CSS color values, however, _might_ parse
this string as a color, with value `rgb(255,126,34)`. Or it might not. It's really not the job of
the serialization protocol to determine interpretation. Note, however, what happens on
re-serialization: the C++ encoder writes the value back out as the word value `#ff7e22`, and the CSS
encoder writes it back out as the word value `#ff7e22` as well. If the value is transformed, then
it's written out as the application deems proper. It's not up to the serialization format to dictate
usage.

This provides a simple, stable mechanism for the interchange of data across many different types of
encoders and decoders, and additionally provides for a way to convey domain-specific data values.

### Word Concatenation
The concatenation operator always promotes words to strings, to produce a string-valued result. For
example, the LSON `red + green + blue` would yield the string value `"redgreenblue"`.


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
Dictionaries (referred to as _objects_ in JSON) are sets of key-value pairs. Keys are string values,
and hence may be either quoted or unquoted. Dictionaries have the following properties:

1. They begin with a left curly bracket (`{`, `U+007b`), followed by zero or more key-value pairs,
   followed by a right curly bracket (`}`, `U+007d`).


Structures
----------
A _structure_ is really just a shorthand for expressing dictionaries, without repeating key names.
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
            {   key1:thing1 key2:false key3:3  }
            {   key1:thing2 key2:false key3:13 }
            {   key1:thing3 key2:true  key3:37 }
        ]
    }

If more values are given for a row than were specified in the structure template, the additional
values will be ignored. If fewer values are given, the missing values will be undefined. For
example,

    {
        collection <value isPositive>: [
            < -1.5 false >
            <  0         >
            <  2.4 true  red null >
        ]
    }

would yield the same result as this LSON:

    {
        collection: [
            { value: -1.5, isPositive: false }
            { value: 0 }
            { value: 2.4, isPositive: true }
        ]
    }

As a side note, this is _not_ necessarily the best or most efficient way to express tabular data.
That would be more like this:

    {
        fields: [ first_name last_name ID ]
        rows: [
            [ Ariel   Astro    48844757 ]
            [ Blue    Blastar  23cc418e ]
            [ Castor  Cantrod  b12b4f89 ]
        ]
    }

or just this (where row 0 is special and holds the column names):

    [
        [ first_name  last_name  ID       ]
        [ Ariel       Astro      48844757 ]
        [ Blue        Blastar    23cc418e ]
        [ Castor      Cantrod    b12b4f89 ]
    ]



[Introduction]:                     #introduction
[Key Differences from JSON]:        #key-differences-from-json
[Special Values]:                   #special-values
[Example LSON]:                     #example-lson
[Whitespace]:                       #whitespace
[Comments]:                         #comments
[Strings]:                          #strings
[Escape Sequences]:                 #escape-sequences
[String Concatenation]:             #string-concatenation
[Words]:                            #words
[Word Concatenation]:               #word-concatenation
[Arrays]:                           #arrays
[Dictionaries]:                     #dictionaries
[Structures]:                       #structures
[Conclusion]:                       #conclusion

LSON: Lucid Serialized Object Notation
====================================================================================================

1. [Introduction]
   - [Key Differences from JSON]
2. [LSON By Example]
3. [Whitespace]
4. [Comments]
5. [Strings]
   - [Escape Sequences]
   - [String Concatenation]
6. [Words]
   - [Word Concatenation]
7. [Arrays]
8. [Dictionaries]
9. [Structures]
10. [Grammar]
11. [Appendix A: String Little Languages]


Introduction
-------------
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
2. Items are terminated by whitespace, commas or semi-colons.
3. String quoting is optional for words (strings without whitespace).
4. All possible special (word) values are handled seamlessly (_e.g._ `NaN`, `infinity`, `undefined`,
   `maybe`, `0xfffe`, `#ff8800` and so on).
5. LSON supports templated objects (structures).
6. LSON has only five fundamental types: words, strings, dictionaries, arrays, and structures.


LSON By Example
----------------
Following are some example LSON snippets to illustrate various aspects.
```
// Comments are C-style: double slash to end of line, or enclosed with `/*` and `*/`.
/* This is an example using slash-star delimeters. */

// Items may be terminated with whitespace, commas, semi-colons, or object/array terminators.

{
    glossary: {
        title: 'example glossary'
        'Gloss Div': {                // There are six legal string-delimeter pairs.
            title: S
            "Gloss List": {
                `Gloss Entry`: {
                    /* Strings (words) may be unquoted as long as they contain no whitespace. */
                    ID: SGML

                    SortAs:  SGML
                    Acronym: SGML
                    «Gloss Term»: "Standard Generalized Markup Language"

                    Abbrev: ISO\ 8879:1986 // Unquoted strings may contain whitespace if escaped.

                    ‘Gloss Def’: {
                        para: "A meta-markup language, used to create markup languages "
                            + "such as DocBook."

                        “Gloss SeeAlso”: [ GML, XML, HTML ]
                        'Gloss See': markup
                    }
                }
            }
        }
    }
}
```

```
// Example Menu Description
{
    menu: {
        id:    file;
        value: file;
        popup: {
            menuitem1: {
                { value:New;   action:CreateNewDoc }
                { value:Open;  action:OpenDoc      }
                { value:Close; action:CloseDoc     }
            }

            // Structured Objects
            menuitem2 <value, action>: [
                < Copy,  CopySelection >
                < Cut,   CutSelection  >
                < Paste, PasteItem     >
            ]
        };
    }
}
```

```
// An example using word values
{
    widget: {
        debug: "On"              // The literal string value "On"
        "debug:Level":  1.0      // Converts to floating-point 1.0 if understood, else string
        "debug:Weight": Infinity // Converts to floating-point +infinity if understood, else string
        "debug:Prefix": null     // Converts to null value if understood, otherwise string
        "debug:Mask":   0xffe0   // Converts to hex number value if understood, else string

        window: {
            title:  'Sample Konfabulator Widget'
            name:   main_window

            width:  500           // Converts to number value if understood
            height: 500
        }
        image: {
            src:         Images/Sun.png
            name:        sun1
            hOffset:     250
            vOffset:     250
            alignment:   center
            description: null
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
-----------
LSON whitespace includes all
[standard Unicode whitespace characters](https://en.wikipedia.org/wiki/Whitespace_character#Unicode).

| Unicode | Escape | Description
|:-------:|:------:|:-----------
| U+0009  |  \t    | Tab
| U+000a  |  \n    | Newline, or line feed
| U+000b  | \u000b | Vertical tab
| U+000c  |  \f    | Form feed
| U+000d  |  \r    | Carriage return
| U+0020  | \u0020 | Standard space character
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
---------
    // Single line comments run from double forward slashes to end of line.

    /*  Slash-star comments: this is probably the best form for block
        comments. */


Strings
--------
Strings are delimited with any of the following character pairs:

|  Quotes    | Character Codes | Description                                       |
|:----------:|:---------------:|:--------------------------------------------------|
|  "string"  |     U+0022      | Quotation Mark                                    |
|  'string'  |     U+0027      | Apostrophe                                        |
| \`string\` |     U+0060      | Grave Accent (Backtick)
|  «string»  | U+00ab, U+00bb  | {Left,Right}-Pointing Double Angle Quotation Mark |
|  ‘string’  | U+2018, U+2019  | {Left,Right} Single Quotation Mark                |
|  “string”  | U+201c, U+201d  | {Left,Right} Double Quotation Mark                |

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
------
Words are simply unquoted strings. For example,

    node: {
        id:       1223-02
        class:    sphere
        weight:   112.23e-6
        previous: null
        next:     0xff128bc5
    }

This feature is more useful than just as a shorthand for string values: it also provides an
excellent mechanism for conveying an arbitrary set of domain-specific values.

JSON defines several special values: `true`, `false`, `null` and numbers. Numbers are a _subset_ of
legal JavaScript (the “J” in JSON) representations. They lack, for example, numbers of the form
`.12` (JSON requires numbers to have a leading zero). In addition, the IEEE special values `NaN` and
`Infinity` are unsupported. Other JavaScript values, such as `0x77` and `undefined` are also
lacking.

The elegance of JSON has given rise to its overwhelming success as a data interchange format for all
kinds of situations. It's as useful for Python or C++ as it is for JavaScript. Given this, what to
do about Python's `None`, CSS's `#23ec98`, C++'s `0xfffe` or Scala's `Any`? The temptation for those
who wish to expand JSON formats is to formalize these special values as reserved words, usually
starting with the introduction of missing JavaScript values.

LSON takes a different approach. Instead of formalizing additional value types, LSON handles all of
them as simple bare words. This approach provides implicit support for any and all special value
types where it makes sense – the encoders and decoders decide. If a decoder does not understand a
special value (for example, a C++ parser that encounters the word `#ff7e22`), the word is simply
interpreted as the string value `"#ff7e22"`. A decoder that _does_ understand CSS color values,
however, _might_ parse this string as a color, with value `rgb(255,126,34)`. Or it might not. It's
really not the job of the serialization protocol to determine interpretation.

On re-serialization, _an encoder must preserve bare words as bare words_. A C++ encoder would
write the value back out as the bare (unquoted) word value `#ff7e22`, and a CSS encoder would write
it back out as the bare word value `#ff7e22` as well. If a value is transformed, then it's written
out as the application deems proper. It's not up to the serialization format to dictate usage. This
further implies that decoders must note and distinguish between strings and bare words in order to
preserve that form on output.

This provides a simple, stable mechanism for the interchange of data across many different types of
encoders and decoders, and additionally provides for a way to convey domain-specific data values.

### Word Concatenation
The concatenation operator always promotes words to strings, to produce a string-valued result. For
example, the LSON `red + green + blue` would yield the string value `"redgreenblue"` (not the bare
word `redgreenblue`).


Arrays
-------
Arrays encode ordered lists of items. They have the following properties:

1. They begin with a left square bracket (`[`, `U+005b`), followed by zero or more values, and
   terminated with a right square bracket (`]`, `U+005d`).

2. Array values may be strings, arrays, or dictionaries.

3. Each array value is terminated with whitespace, a comma, a semi-colon, or the array terminator.

4. Arrays are contiguous. That is, there is no way in LSON to indicate an undefined element. If
   sparse arrays are desired for a particular encoding, it is recommended that dictionaries be used
   with numeric key values. Encoders should follow the same convention, encoding sparse arrays as
   dictionaries. An alternative would be to adopt a special word, such as `undefined` to denote
   undefined values. Interpreting this would depend on a particular encoder/decoder.


Dictionaries
-------------
Dictionaries (referred to as _objects_ in JSON) are sets of key-value pairs. Keys are string values,
and hence may be either quoted or unquoted. Dictionaries have the following properties:

1. They begin with a left curly bracket (`{`, `U+007b`), followed by zero or more key-value pairs,
   followed by a right curly bracket (`}`, `U+007d`).

2. Each dictionary pair is terminated with whitespace, a comma, a semi-colon, or the dictionary
   terminator.


Structures
-----------
A _structure_ is really just a shorthand for expressing dictionaries without repeating key names.
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

As a side note, this is _not_ necessarily the best or most efficient way to store tabular data.
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


Grammar
--------
```
lson-file ::= <value>

line-terminator ::= U+000a | U+000b | U+000c | U+000d | U+0085 | U+2028 | U+2029

comment-line-remainder ::= "//" <any character not line-terminator>* <line-terminator>
comment-block ::= "/*" <any character sequence not containing "*/"> "*/"

whitespace-item ::= <comment-line-remainder> | <comment-block> | <line-terminator>
    | U+0009 | U+0020 | U+00a0 | U+1680 | U+2000 | U+2001 | U+2002 | U+2003 | U+2004 | U+2005
    | U+2006 | U+2007 | U+2008 | U+2009 | U+200a | U+2028 | U+2029 | U+202f | U+205f | U+3000

whitespace ::= <whitespace-item>+

value ::= <word> | <string> | <dictionary> | <array> | <structure>

string-character ::= <non whitespace character>
    | "\0" | "\n" | "\r" | "\t" | "\u" <hex4> | "\U" <hex6> | "\" <character>

hex4 ::= <hex> <hex> <hex> <hex>
hex6 ::= <hex> <hex> <hex> <hex> <hex> <hex>

hex ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
      | "a" | "b" | "c" | "d" | "e" | "f"
      | "A" | "B" | "C" | "D" | "E" | "F"

word ::= <string-character>+

string ::= <string-begin-quote> <any <string-character> not matching begin quote>*
    <string-end-quote> <concatenated-string>*

string-begin-quote ::= U+2022 | "'" | "«" | "‘" | "“"
string-end-quote   ::= U+2022 | "'" | "»" | "’" | "”"

concatenated-string ::= "+" ( <word> | <string> )

terminator ::= "," | ";" | whitespace | empty-before-closing-character

dictionary ::= "{" <dictionary-body> "}"

dictionary-body ::= <dictionary-item>*
dictionary-item ::= <key> ":" <value> <terminator>
key ::= <word> | <string>

array ::= "[" <array-item>* "]"
array-item ::= <value> <terminator>

structure ::= <key> "<" ( <key> <terminator>? )+ ">" ":" "[" <structure-item>* "]"
structure-item ::= "<" <array-item>* ">" <terminator>?
```

Appendix A: String Little Languages
------------------------------------
This is implied above, but provided here to be more explicit. Words such as `null` and `#ffee05` can
be thought of as “little languages”. In order for word→value promotion to work, words must be
recognizable from only their string value, and unambiguous. Again, this process lies entirely within
the domain of the application; LSON does not stipulate the format of special word values in any way.

In the same manner, any _string_ value can also be a little language, again defined entirely in the
domain of the application and outside the domain of LSON. For example, a string value parsed by a
JavaScript application may describe a function definition, such as `"(x,y,z) => { return
Math.sqrt(x*x + y+y + z*z) }"`. As long as something can be decoded from (and encoded to) a string,
it's representable in an agnostic way in LSON.

Contrast this to JSON's inclusion of number values, `true`, `false` and `null`, which are actually
domain-specific values (exceedlingly common, but still domain specific).



[Appendix A: String Little Languages]: #appendix-a-string-little-languages
[Arrays]:                    #arrays
[Comments]:                  #comments
[Conclusion]:                #conclusion
[Dictionaries]:              #dictionaries
[Escape Sequences]:          #escape-sequences
[LSON By Example]:           #lson-by-example
[Grammar]:                   #grammar
[Introduction]:              #introduction
[Key Differences from JSON]: #key-differences-from-json
[Special Values]:            #special-values
[String Concatenation]:      #string-concatenation
[Strings]:                   #strings
[Structures]:                #structures
[Whitespace]:                #whitespace
[Word Concatenation]:        #word-concatenation
[Words]:                     #words

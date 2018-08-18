LSON: Lucid Serialized Object Notation
====================================================================================================

1.  [Overview]
2.  [LSON Example]
3.  [Whitespace]
4.  [Comments]
5.  [Strings]
    - [Escape Sequences]
    - [String Concatenation]
6.  [Elements]
7.  [Bare Words]
    - [Word → Element Promotion]
    - [Bare Word Concatenation]
8.  [Arrays]
9.  [Dictionaries]
10. [Tables]
11. [Graphs]
12. [Appendix A: Grammar]
13. [Appendix B: String Little Languages]
14. [Appendix C: Common Element Types]


Overview
---------
LSON is a data representation that aims for simplicity and expressiveness of JSON, but differs
in the following ways:

  + It's intended to be both concise and readable by humans as well as computers. It supports
    comments. Items are optionally terminated by whitespace, end delimiters, commas, or semi-colons.

  + It does not aim to mirror JavaScript, and thus is not a JavaScript subset. At the same time,
    LSON is a superset of JSON: any legal JSON file is legal LSON.

  + LSON supports arbitrary _elements_: data values with declared or unknown type. Strings are the
    single element type that LSON supports natively. Elements provide support for domain-specific
    values, such as `true`, `null`, `infinity`, `2018-07-02`, `#6b17ec`, `0x1138`,
    `(x,a,b) => { a <= x && x <= b }` and so forth. LSON encoders and decoders can handle both known
    and unknown types in a consistent and predictable manner.

  + LSON supports four intrinsic data structures:
    - array
    - dictionary (a set of name-value pairs)
    - table
    - graph


LSON Example
-------------
Following are some example LSON snippets to illustrate various aspects of the notation, before we
dive deeper:

    // Comments are C-style: double slash to end of line, or enclosed with `/*` and `*/`.
    /* This is an example using slash-star delimeters. */

    // Items may be terminated with whitespace, commas, semi-colons, or object/array terminators.

    {
        index: {
            'Gloss Div': {                // There are six legal string-delimeter pairs.
                title: S                  // No need to quote strings that lack whitespace.
                "Gloss List": {
                    `Gloss Entry`: {
                        ID:      x112-223
                        SortAs:  SGML
                        Acronym: SGML
                        «Gloss Term»: "Standard Generalized Markup Language"

                        Abbrev: ISO\ 8879:1986     // Whitespace can be escaped.

                        ‘Gloss Def’: {
                            para: "A meta-markup language, used to create markup languages "
                                + "such as DocBook."

                            “Gloss SeeAlso”: [ GML, XML, HTML ]
                            'Gloss See': markup
                        }
                    }
                }
            }
            EntryCount: (count32:1123)      // Element of some type "count32", value "1123"

            // Table
            Content: [%
                Term                ; Pages           ; See Also
                :
                "ABC Dry-Clean Pad" ; (30)            ; null             ;
                "Abstract grids"    ; (46-58, 92-104) ; "Gridded Layout" ;
                "Abstract ideas"    ; (37-38, 74-77)  ; null             ;
                "Acceleration"      ; (408-409)       ; Velocity         ;
                // ...
            %]
        }
    }


Whitespace
-----------
LSON whitespace includes all [standard Unicode whitespace characters].

| Unicode |   Escape   | Description
|:-------:|:----------:|:-----------
| U+0009  |    `\t`    | Tab
| U+000a  |    `\n`    | Newline, or line feed
| U+000b  |  `\u{0b}`  | Vertical tab
| U+000c  |    `\f`    | Form feed
| U+000d  |    `\r`    | Carriage return
| U+0020  |  `\u{20}`  | Standard space character
| U+0085  |  `\u{85}`  | Next line
| U+00a0  |  `\u{a0}`  | No-break space
| U+1680  | `\u{1680}` | Ogham space mark
| U+2000  | `\u{2000}` | En quad
| U+2001  | `\u{2001}` | Em quad (mutton quad)
| U+2002  | `\u{2002}` | En space (nut)
| U+2003  | `\u{2003}` | Em space (mutton)
| U+2004  | `\u{2004}` | Three-per-em-space (thick space)
| U+2005  | `\u{2005}` | Four-per-em-space (mid space)
| U+2006  | `\u{2006}` | Six-per-em-space
| U+2007  | `\u{2007}` | Figure space
| U+2008  | `\u{2008}` | Punctuation space
| U+2009  | `\u{2009}` | Thin space
| U+200a  | `\u{200a}` | Hair space
| U+2028  | `\u{2028}` | Line separator
| U+2029  | `\u{2029}` | Paragraph separator
| U+202f  | `\u{202f}` | Narrow no-break space
| U+205f  | `\u{205f}` | Medium mathematical space
| U+3000  | `\u{3000}` | Ideographic space


Comments
---------
    // Single line comments run from double forward slashes to end of line.

    /* Slash-star comments:
    this is probably
    the best form
    for block comments. */


Strings
--------
Strings are delimited with any of the following character pairs:

|  Quotes    | Character Codes | Description                                       |
|:----------:|:---------------:|:--------------------------------------------------|
|  "string"  |     U+0022      | Quotation Mark                                    |
|  'string'  |     U+0027      | Apostrophe                                        |
| \`string\` |     U+0060      | Grave Accent (Backtick)                           |
|  «string»  | U+00ab, U+00bb  | {Left,Right}-Pointing Double Angle Quotation Mark |
|  ‘string’  | U+2018, U+2019  | {Left,Right} Single Quotation Mark                |
|  “string”  | U+201c, U+201d  | {Left,Right} Double Quotation Mark                |

### Escape Sequences
Strings may contain the following escape sequences:

| Sequence   | Description                                           |
|:-----------|:------------------------------------------------------|
| `\b`       | Backspace                                             |
| `\f`       | Form feed                                             |
| `\n`       | New line                                              |
| `\r`       | Carriage return                                       |
| `\t`       | Horizontal tab                                        |
| `\uXXXX`   | Unicode character from four hexadecimal digits        |
| `\u{X...}` | Unicode character from one or more hexadecimal digits |
| `\<any>`   | Yields that character unchanged, such as `\'` or `\\` |

### String Concatenation
In order to support human-readable long strings, the `+` operator may be used to construct
concatenations. For example:

    {
        strBlock: "Knock knock.\n"
                + "Who's there?\n"
                + "Bug in your state machine.\n"
                + "Who's there?\n"
    }


Elements
---------
Elements are distinguished from strings by enclosing parentheses. For example, `(null)` denotes
the special value `null`. Unlike JSON, any string-representable value is allowed, and interpretation
is up to the decoding application. Applications that do not handle a particular element value
natively will process that value as a simple string, while preserving the notion that it's a special
value. The value type (or non-type) is to be preserved if it is written back out to LSON.

### Typed Elements
In order to facilitate processing, or to provide precision, or to resolve ambiguity, element values
may also include a specific type (such as `color`, `float32`, `readyState`, or `boolean`). In this
case, the value is prefixed with a type id, followed by a colon, like so:

    (color:#f863b2)
    (float32:334.1)
    (readyState:armed)
    (boolean:true)

Note that type IDs are case-insensitive. Type names must escape colons, or must be quoted. Once the
first colon is encountered, any subsequent colon is interpreted as part of the value. For example,
if a element has type `width:height` and value `150:400`, it could be expressed in any of the
following ways:

    (width\:height: 150:400)
    ("width:height": 150:400)
    ('150:400')                 // Omitted value, assumed recognizable
    (150\:400)                  // Omitted value, assumed recognizable
    (:150:400)                  // Omitted value, assumed recognizable

In general, avoid type names with colons if possible. Values with colons are easy enough to specify
using the last form above for unspecified types, or just including a type, like so:

    (position:150:400)          // Type 'position', value '150:400'

### Element Values with Whitespace
Element values with embedded whitespace should either escape the whitespace or quote the value, as
whitespace may preceed or follow the value, as in `(position:  center  )`.


Bare Words
-----------
Bare words are simply unquoted strings. For example,

    node: {
        id:       1223-02
        class:    sphere
        weight:   112.23e-6
        previous: null
        next:     0xff128bc5
    }

A word is parsed as an element with unknown type. Its type may be recognized by a domain-specific
decoder. If it is not recognized, then it is treated as a untyped element. This feature is more
useful than just as a shorthand for string values: it also provides a mechanism for conveying an
arbitrary set of domain-specific values.

JSON defines several special values: `true`, `false`, `null` and numbers. Numbers are a _subset_ of
legal JavaScript (the “J” in JSON) representations. They lack, for example, numbers of the form
`.12` (JSON requires numbers to have a leading zero). In addition, the IEEE special values `NaN` and
`Infinity` are unsupported. Other JavaScript values, such as `0x77` and `undefined` are also
lacking.

The elegance of JSON has given rise to its overwhelming success as a data interchange format for all
kinds of purposes. It's as useful for Python or C++ as it is for JavaScript. Given this, what to do
about Python's `None`, CSS's `#23ec98`, C++'s `0xfffe` or Scala's `Any`? The temptation for those
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

### Word → Element Promotion
A specific application may choose to recognize and handle a select set of element types. In such a
case, it is up to the application to specify the types handled, and the order in which they are
recognized.

For example, a standard JSON-type parser would handle the following ordered list of element types:

  1. null (`null`)
  2. boolean (`true`, `false`)
  3. real numbers (`[-]*[<digit>]*[.<digit>*][[eE][+-]?<digit>+]`, or some such syntax)

Other common parsers might support CSS values, ±infinity, and so forth. See [Elements.md] for a set
of common element types.

### Bare Word Concatenation
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

LSON dictionaries differ from JSON in that multiple keys may specified with a single value, using an
array-like syntax:

    {
        [ red orange yellow ]: true
        [ green cyan blue violet ]: false
    }


Tables
-------
Tables (like CSV files) are 2D entities with multiple rows (points) of data, where each dimension
has an associated label. Tables have the following properties:

  + Tables are delimited with `[#` and `#]` tokens (no whitespace is allowed between delimiter
    characters).

  + Tables may optionally use `[` and `]` delimiters around field names and row data, as an aid to
    readability and catching errors.

  + Tables do not allow implicit null data; all row dimensions must be specified. Thus, for a table
    with *N* columns, there must be *M⋅N* data points in the table, where *M* is the number of table
    rows.

  + Each cell (dimension) may be any legal LSON value. So you could have cells of arrays, objects,
    graphs, or even other tables.

The following is an example LSON table:

    [#
        [key1    key2   key3]:
        [thing1  false     3]
        [thing2  false    13]
        [thing3  true     37]
    #]

The fragment above uses brackets to delimit table rows, which can aid legibility and debugging with
erroneous inputs. If brackets are used to delimit the column names, then they are required around
each row of data, and must yield a parsing error for a row that does not contain the same number of
values as the header row.

However, brackets are optional, and the same table could be expressed thus:

    [#
        key1   ; key2  ; key3
    ://--------;-------;------
        thing1 ; false ;    3
        thing2 ; false ;   13
        thing3 ; true  ;   37
    #]

While the absence of square brackets means that rows cannot be validated individually for the
expected number of values, the entire table must still contain a number of values that is a multiple
of the number of header values.

As with objects and arrays, optional comma, or semi-colon terminators may be used to aid
readability, like so:

    [# key1,key2,key3: thing1,false,3; thing2,false,13; thing3,true,37; #]

Potential ambiguous sequences can usually be solved with whitespace, like so:

    [ #ff8cee #Nan# ]   // An array with a CSS color and a special value; NOT a table.


Graphs
-------
LSON supports graph data, where a graph is defined by a set of nodes and a set of edges between
those nodes. Graphs have the following properties:

  + Graphs are delimited with `[%` and `%]` tokens (no whitespace is allowed between delimiter
    characters.)
  + Each node has associated data.
  + Each edge may or may not have associated data.
  + Nodes may be unnamed, in which case they are referenced by index.
  + Nodes may be named, in which case they are referenced by name.
  + Edges may be directed or undirected.
  + An edge may leave and arrive at the same node.
  + There may be many edges between a pair of nodes.


### Overall Structure
Each LSON graph is expressed in the following pattern:

    [%
        // Node Data
        // Edge Data
    %]


### Node Data
Graph nodes may be expressed in a number of ways, depending on

  - whether they are to be referenced by index or by name, and
  - whether they have associated data.

Unnamed nodes use an array-like container, while named nodes use a dictionary-like container.

#### Unnamed Nodes Without Data
If nodes are to be referenced by index from 0 onward, and have no data, then simply specifying the
number of nodes is sufficient. Node count must be greater than or equal to zero, and is expressed as
a positive decimal integer.

    1000

#### Unnamed Nodes With Data
The following (unnamed) nodes can be referenced by index. Each node has 2D coordinate data.

    [  [3 2] [2 2] [2 1] [1 3] [1 2] [1 1]  ]
    // ----- ----- ----- ----- ----- -----
    //   0     1     2     3     4     5

Note: in the event that nodes are named with number-like values, they must still be referenced by
their array-like index:

    [
      1  // Node 0 has data (1)
      2  // Node 1 has data (2)
      3  // Node 2 has data (3)
      5  // Node 3 has data (5)
      8  // Node 4 has data (8)
    ]

#### Named Nodes Without Data
To express a set of named nodes without additional data, use a dictionary with multi-keyed dummy
data, like so:

    { [ bargaining testing anger shock acceptance depression denial ]: null }

#### Named Nodes With Data
To express a set of named nodes with additional data per node, use a dictionary. Each entry
specifies the node data by name. Here, the seven standard colors of the rainbow are given with their
CSS color values:


    {
        red:    { css:#ff0000  rgb:'rgb(255,0,0)'     }
        orange: { css:#ffa500  rgb:'rgb(255,165,0)'   }
        yellow: { css:#ffff00  rgb:'rgb(255,255,0)'   }
        green:  { css:#008000  rgb:'rgb(0,128,0)'     }
        blue:   { css:#0000ff  rgb:'rgb(0,0,255)'     }
        indigo: { css:#4b0082  rgb:'rgb(75,0,130)'    }
        violet: { css:#ee82ee  rgb:'rgb(238,130,238)' }
    }

### Edge Data
Edges are expressed as a set of node pair relationships. A node relationship is either a node index
or name, followed by a relationship character, followed by the second node index or name.

The following are examples of node edges:

| Edge                 | Interpretation
|----------------------|------------------------------------------
| `a - b` <br> `a ↔ b` | An undirected edge between nodes a and b
| `a > b` <br> `a → b` | A directed edge from node a to node b
| `a < b` <br> `a ← b` | A directed edge to node a from node b

\* The special Unicode characters above are ↔ [U+2194], → [U+2192], and ← [U+2190].

Because node names may themselves contain relationship characters, ambiguity is possible. In
general, parsing will consider the first encountered relationship character as a part of the edge
description, and not part of the node name. If a node name contains any of the above six characters,
they must either be escaped, or the node name should be quoted. Here are parsing examples for edge
specifications:

| Edge Spec | Result
|:----------|:----------------------------------------------------------------
| `a\-b-c`  | Legal: interpreted as `'a-b' - 'c'`
| `a-'b-c'` | Legal: interpreted as `'a' - 'b-c'`
| `a>b`     | Legal: interpreted as `'a' > 'b'`
| `a-b>c`   | **Illegal**: could be `'a-b' > 'c'` or `'a' - 'b>c'`
| `a-b-c`   | **Illegal**: could be `'a-b' - 'c'` or `'a' - 'b-c'`
| `a-b - c` | **Illegal**: interpreted as `'a' - 'b'`, followed by illegal spec `-`


#### Edges Without Data
Edges without data are specified as an array of edges. Here's an example using node indices:

    [ 0→500 1→548 2→23 3→897 ... ]

Another example, this time using node names:

    [
        shock → denial
        denial → anger
        anger → bargaining
        bargaining → depression
        depression → testing
        testing → acceptance
    ]

#### Edges With Data
Edges with data use a dictionary where each property is a node edge, and the property value is the
data associated with the edge. This example references named nodes to specify edges colored with CSS
colors.

    {
      'upper-left' ← 'upper-right': #888888
      'mid-left'   - 'upper-left' : #666666
      'mid-left'   → 'mid-right'  : #444444
      'mid-right'  - 'lower-right': #222222
      'lower-left' ← 'lower-right': #000000
    }

### Some Final Graph Examples
Indexed nodes without data, edges without data:

    [%
        1000
        [ 0→500; 1→548; 2→ 23; 3→897; ... ]
    %]

Indexed nodes with 2D coordinate data, plus edges without data:

    [%
        [ [3 2] [2 2] [2 1] [1 3] [1 2] [1 1] ]
        [ 0-3, 1-4, 2-5, 3-4, 1-2 ]
    %]

The seven stages of grief:

    [%
        { [ bargaining testing anger shock acceptance depression denial ]: null }

        [   shock → denial
            denial → anger
            anger → bargaining
            bargaining → depression
            depression → testing
            testing → acceptance
        ]
    %]

Finally, a railroad (parsing) graph for floating point numbers:

    [%
        {[
          start  wholeDigit    fractionalDigit  exponentCharacter
          sign   decimalPoint  exponentSign     exponentDigit
          end
        ]:null}

        [
            start → sign
            start → wholeDigit
            start → decimalPoint

            wholeDigit → wholeDigit
            wholeDigit → exponentCharacter
            wholeDigit → decimalPoint
            wholeDigit → end

            decimalPoint → fractionalDigit
            decimalPoint → exponentCharacter
            decimalPoint → end

            fractionalDigit → fractionalDigit
            fractionalDigit → exponentCharacter
            fractionalDigit → end

            exponentCharacter → exponentSign
            exponentCharacter → exponentDigit

            exponentSign → exponentDigit

            exponentDigit → exponentDigit
            exponentDigit → end
        ]
    %]


Appendix A: Grammar
--------------------
    lson-file ::= <value>

    line-terminator ::= U+000a | U+000b | U+000c | U+000d | U+0085 | U+2028 | U+2029

    comment-line-remainder ::= "//" <any character not line-terminator>* <line-terminator>
    comment-block ::= "/*" <any character sequence not containing "*/"> "*/"

    whitespace-item ::= <comment-line-remainder> | <comment-block> | <line-terminator>
        | U+0009 | U+0020 | U+00a0 | U+1680 | U+2000 | U+2001 | U+2002 | U+2003 | U+2004 | U+2005
        | U+2006 | U+2007 | U+2008 | U+2009 | U+200a | U+2028 | U+2029 | U+202f | U+205f | U+3000

    whitespace ::= <whitespace-item>+

    value ::= <word> | <string> | <element> | <array> | <dictionary> | <table> | <graph>

    string-character ::= <non whitespace character>
        | "\b" | "\f" | "\n" | "\r" | "\t" | "\u" <hex>{4} | "\u{" <hex>+ "}" | "\" <character>

    hex ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
          | "a" | "b" | "c" | "d" | "e" | "f"
          | "A" | "B" | "C" | "D" | "E" | "F"

    word ::= <string-character>+

    string ::= <unquoted-string> | <quoted-string>
    quoted-string ::= <string-begin-quote> <any <string-character> not matching begin quote>*
        <string-end-quote> <concatenated-string>*
    unquoted-string ::= <string-character>+ <concatenated-string>*

    string-begin-quote ::= U+0022 | "'" | "«" | "‘" | "“"
    string-end-quote   ::= U+0022 | "'" | "»" | "’" | "”"

    concatenated-string ::= "+" ( <word> | <string> )

    id ::= <string>

    terminator ::= "," | ";" | <whitespace> | empty-before-closing-delimiter

    element ::= <untyped-element> | <typed-element>
    untyped-element ::= "(" <string> ")"
    typed-element ::= "(" <typeID> ":" <string> ")

    dictionary ::= "{" <dictionary-body> "}"

    dictionary-body ::= <dictionary-item>*
    dictionary-item ::= <key> ":" <value> <terminator>
    key ::= <id> | "[" <key>+ "]"

    array ::= "[" <array-item>* "]"
    array-item ::= <value> <terminator>

    table ::= "[#" <table-body> "#]"
    table-body ::= <table-body-unbracketed> | <table-body-bracketed>

    table-body-unbracketed ::= ( <id> <terminator> ){n+} ":" <table-row-bare>(n+)*
    table-row-bare(n) ::= ( <value> <terminator> ){n}

    table-body-bracketed ::= "[" ( <id> <terminator> ){n+} "]" ":" <table-row-bracketed>(n+)*
    table-row-bracketed(n) ::= "[" <table-row-bare>(n) "]"

    graph ::= "[%" <graph-nodes> <graph-edges> "%]"

    graph-nodes ::= <counting-number> | <array> | <dictionary>

    graph-edges ::= <edge-array> | <edge-dictionary>
    edge-array ::= "[" <edge>+ "]"
    edge-dictionary ::= "{" (<edge-key> ":" <value> <terminator>)* "}"
    edge-key ::= <edge> | "[" (<edge> <terminator>)+ "]"

    edge ::= <node-ref> <edge-type> <node-ref>
    node-ref ::= <node-index> | <id>
    node-index ::= <counting-number>

    edge-type ::= '-' | '↔' | '>' | '→' | '<' | '←'

    ____

    <token>?    Denotes zero or one <token>
    <token>*    Denotes zero or more <token>
    <token>+    Denotes one or more <token>
    <token>{n}  Denotes exactly n <token>s
    <token>{n+} Denotes common n <token>s, where n is one or more


Appendix B: String Little Languages
------------------------------------
This is implied above, but provided here to be more explicit. Words such as `null` and `#ffee05` can
be thought of as “little languages”. In order for word→element promotion to work, words must be
recognizable from only their string value, and unambiguous. Again, this process lies entirely within
the domain of the application; LSON does not stipulate the format of special word values in any way.

In the same manner, any _string_ value can also be a little language, again defined entirely in the
domain of the application and outside the domain of LSON. For example, a string value parsed by a
JavaScript application may describe a function definition, such as `"(x,y,z) => { return
Math.sqrt(x*x + y+y + z*z) }"`. As long as something can be decoded from (and encoded to) a string,
it's representable in an agnostic way in LSON.

Contrast this to JSON's inclusion of number values, `true`, `false` and `null`, which are actually
domain-specific values (exceedlingly common, but still domain specific).

Appendix C: Common Element Types
---------------------------------
For a set of common element types, see [ElementTypes.md].




[Arrays]:                   #arrays
[Bare Words]:               #bare-words
[Bare Word Concatenation]:  #bare-word-concatenation
[Comments]:                 #comments
[Conclusion]:               #conclusion
[Dictionaries]:             #dictionaries
[Escape Sequences]:         #escape-sequences
[LSON Example]:             #lson-example
[Graphs]:                   #graphs
[Overview]:                 #overview
[Elements]:                 #elements
[ElementTypes.md]:          ./ElementTypes.md
[Special Values]:           #special-values
[String Concatenation]:     #string-concatenation
[Strings]:                  #strings
[Tables]:                   #tables
[Whitespace]:               #whitespace
[Word Concatenation]:       #word-concatenation
[Words]:                    #words
[Word → Element Promotion]: #word--element-promotion
[Appendix A: Grammar]:                 #grammar
[Appendix B: String Little Languages]: #appendix-b-string-little-languages
[Appendix C: Common Element Types]:    #appendix-c-common-element-types

[standard Unicode whitespace characters]: https://en.wikipedia.org/wiki/Whitespace_character#Unicode

LSON: Lucid Serialized Object Notation
====================================================================================================

1.  [Overview][]
2.  [LSON Example][]
3.  [Whitespace][]
4.  [Comments][]
5.  [Strings][]
    - [Escape Sequences][]
    - [String Concatenation][]
6.  [Elements][]
    - [Element Types][]
    - [Element Values][]
    - [Elements of Type String][]
    - [Element Value Blocks][]
    - [Decoding Elements][]
    - [Untyped Elements][]
7.  [Bare Values][]
    - [Word → Element Promotion][]
    - [Bare Value Concatenation][]
8.  [Arrays][]
9.  [Dictionaries][]
10. [Tables][]
    - [Default Table Values][]
11. [Graphs][]
    - [General Graph Structure][]
    - [Graph Nodes][]
      + [Unnamed Graph Nodes Without Data][]
      + [Unnamed Graph Nodes With Data][]
      + [Named Graph Nodes Without Data][]
      + [Named Graph Nodes With Data][]
      + [Graph Nodes With Tabular Data][]
    - [Graph Edges][]
      + [Graph Edges Without Data][]
      + [Graph Edges With Data][]
      + [Graph Edges With Tabular Data][]
      + [Directed Graph Edges Via Adjacency Matrix][]
      + [Undirected Graph Edges Via Adjacency Matrix][]
    - [Graph Examples][]
12. [Appendix A: Grammar][]
13. [Appendix B: Common Element Types][]
14. [Appendix C: LSON Examples][]


Overview
---------
LSON is a data representation that aims for simplicity and expressiveness of JSON, but differs
in the following ways:

  + It's intended to be both concise and readable by humans as well as computers. It supports
    comments. Items are optionally terminated by whitespace, end delimiters, commas, or semi-colons.

  + It does not aim to mirror JavaScript, and thus is not a JavaScript subset. At the same time,
    LSON is a superset of JSON: any legal JSON file is legal LSON.

  + LSON is focused on data _representation_, not data _usage_. With the single exception of string
    values, there is no intrinsic support for numbers, boolean, or any other primitive type.

  + LSON supports arbitrary _elements_: domain-specific data values with declared or unknown type.
    Elements provide support for arbitrary domain-specific values, such as `true`, `null`,
    `infinity`, `2018-07-02`, `#6b17ec`, `0x1138`, `(x,a,b) => { a <= x && x <= b }` and so forth.
    LSON encoders and decoders handle both known and unknown types in a consistent and predictable
    manner.

  + LSON supports four intrinsic data structures:
    - array
    - dictionary (a set of name-value pairs)
    - table
    - graph


LSON Example
-------------
The following is an LSON snippet to illustrate various aspects of the notation, before we dive
deeper:

    // Comments are C-style: double slash to end of line, or enclosed with `/*` and `*/`.
    /* This is an example using slash-star delimeters. */

    // Items may be terminated with whitespace, commas, semi-colons, or object/array terminators.

    {
        index: {
            'Gloss Div': {                // There are six legal string-delimeter pairs.
                title: "S"                // No need to quote strings that lack whitespace.
                "Gloss List": {
                    `Gloss Entry`: {
                        ID:      x112-223
                        SortAs:  SGML
                        Acronym: SGML
                        «Gloss Term»: "Standard Generalized Markup Language"

                        Abbrev: (ISO: 8879:1986)  // Element of some type "ISO", value "8879:1986"

                        ‘Gloss Def’: {
                            para: "A meta-markup language, used to create markup languages "
                                + "such as DocBook."

                            “Gloss SeeAlso”: [ 'GML', 'XML', 'HTML' ]
                            'Gloss See': "markup"
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
LSON whitespace includes all [standard Unicode whitespace characters][].

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
Strings are the only natively-supported element type (more on LSON elements later).

In addition to standard double quotes, strings may be quoted with any of five additional quote
delimiter pairs. This provides a clean way to avoid the necessity of escaping delimiters in most
complex strings.

|  Quotes    | Character Codes | Description                                       |
|:----------:|:---------------:|:--------------------------------------------------|
|  "string"  |     U+0022      | Quotation Mark                                    |
|  'string'  |     U+0027      | Apostrophe                                        |
| \`string\` |     U+0060      | Grave Accent (Backtick)                           |
|  «string»  | U+00ab, U+00bb  | {Left,Right}-Pointing Double Angle Quotation Mark |
|  ‘string’  | U+2018, U+2019  | {Left,Right} Single Quotation Mark                |
|  “string”  | U+201c, U+201d  | {Left,Right} Double Quotation Mark                |

Strings may contain newlines and linefeeds.

### Escape Sequences
Strings may contain the following escape sequences:

| Sequence        | Description                                           |
|:----------------|:------------------------------------------------------|
| `\b`            | Backspace                                             |
| `\f`            | Form feed                                             |
| `\n`            | New line                                              |
| `\r`            | Carriage return                                       |
| `\t`            | Horizontal tab                                        |
| `\uXXXX`        | Unicode character from four hexadecimal digits        |
| `\uXXany`       | Not a legal Unicode escape; yields `uXXany`           |
| `\u{X...}`      | Unicode character from 1-8 hexadecimal digits         |
| `\u{}`          | Not a legal Unicode escape; yields `u{}`              |
| `\u{123456790}` | Not a legal Unicode escape; yields `u{1234567890}`    |
| `\<any>`        | Yields that character verbatim, such as `\'` or `\\`  |

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
LSON really has a single value type: the _element_. Unlike JSON, any string-representable value is
supported and handled consistently, but interpretation is up to the decoding application or context.
Applications that do not handle a particular element type natively will process that value by way of
its string representation, while preserving its (possibly unknown) type. The value type (which might
be "unknown") is preserved when re-encoding after any transformation or transmission.

### Element Types
Elements have two components: type and value. The element type is optional, and defaults to
"unknown" if not specified. At its most basic, an element has the following syntax:

    ( <type> : <value> )

Elements with a declared type may take several forms. For example:

    (type:value)                // Type "type", value "value"
    ("thing" : "xyzzy")         // Type "thing", value "xyzzy"
    (color:#f863b2)             // Type "color", value "#f863b2"
    ( float32 : 334.1 )         // Type "float32", value "334.1"
    (readyState: armed)         // Type "readyState", value "armed"
    ( a b c : This is a test )  // Type "a b c", value "This is a test"

Note that leading and trailing whitespace is ignored, and not considered part of the type or value.
However, both types and values may themselves _contain_ whitespace. Quoting can be used to preserve
leading or trailing whitepace in types or values.

Type IDs are case-insensitive, and followed by a colon. (Thus, type names must either be quoted or
escape any contained colons.) Once the first colon is encountered, any subsequent colon is
interpreted as part of the value. Types may be omitted. If the type is omitted, the colon itself may
be present or omitted. The following are equivalent examples of an _untyped_ element, both with
value `"a:b:c"`:

    ("a:b:c")
    (:a:b:c)

The first colon after an element open parentheses is treated as terminating the element's type. All
subsequent colons are considered part of the element value. For example, if a element has type
`width:height` and value `150:400`, it could be expressed in any of the following ways:

    (width\:height: 150:400)
    ("width:height": 150:400)
    ('150:400')                 // Omitted type
    (150\:400)                  // Omitted type
    (:150:400)                  // Omitted type

In general, either avoid or quote type names with colons. Untyped values with colons are easy enough
to specify using the last form above for unspecified types, with a colon immediately following the
opening parenthesis.

### Element Values
As shown in examples above, the element value may be either unquoted or quoted in its entirety.
Quoted element values obey the conventions outlined in [Strings][], using any of the six string
delimiters. Because elements may contain values of foreign syntax, **LSON interprets any contained
`)` character as the element terminator**. Consider the following (**erroneous**) example:

    ( gronkScript: konst force ← "gravity(2.3)"; konst elapsed ← 1.223; )    // ERROR

As soon as the LSON parser encounters `konst`, it parses the element value as an unquoted string. In
that mode, it will terminate the element value at `2.3`, _not_ at `1.223;`. Elements that might
contain complex values should therefore either be quoted entirely, or contained in
[Element Value Blocks][] (described below).

### Elements of Type String
As pointed out earlier, strings are the only element type that LSON recognizes implicitly. Since
strings are natively supported, string quotes are sufficient to recognize the element type
("string") and value (the quoted content). Thus, the following are all equivalent:

    (string:"This is a string.")    // Fully-qualified element of type "string"
    (string: This is a string)      // Value quotes optional when inside parentheses
    "This is a string"              // Value recognized as type "string"

### Element Value Blocks
As noted above, an _element_ is bascially an arbitrary foreign syntactic structure. Most uses of
LSON elements will be for simple values in other domains, as we've seen above. Some elements,
however, might have quite a complex representation, both in syntax and in length. For example, it
should be simple to embed a 250-line script inside LSON. In my experience, I've seen JSON inside a
script inside JSON (not because it's good, but because it's necessary).

To this end, elements may also employ block delimiters, which use a unique identifier.
Block-delimited elements begin with `((<id>` where `<id>` is an arbitrary string _that appears
nowhere in the element value_. As soon as the nearest`<id>))` is encountered, the element is closed.

Here's an example of a complex element with block delimiters:

    frotz: ((xyzzy python :
        db = MySQLdb.connect("localhost","username","password","dbname")
        cursor = db.cursor()
        sql = "select Column1,Column2 from Table1"
        cursor.execute(sql)
        results = cursor.fetchall()

        for row in results:
            print row[0]+row[1]

        db.close()
    xyzzy))

Note that the closing identifier must have the identical case as the opening identifier. For
example, `GREEN))` matches `((GREEN`, but not `((Green`.

Now consider the following (**erroneous**) LSON fragment:

    frotz: ((Klaatu blargScript :
        gargle("Hey, I have a ((Klaatu)) inside me!")
    Klaatu))

The element above terminates at `Klaatu))` inside the string, not at the last line (thus yielding a
syntax error). This strict syntax is actually an advantage, because it leaves the element value free
to use any arbitrary syntax, and LSON will dutifully accumulate the string representation of that
element until it encounters the element block terminator.

As another example of the syntactic freedom, here's a fragment that totally diverges from LSON
syntax:

    frotz: ((barada niktoScript:
        Look! Unterminated string chars: " ' » )
        ... wait, there's more ...
        ] } %> #>
        Zing!
    barada))

Safe and legal.

As a final note, because element values encode a foreign syntax, language constructs such as
linefeeds, whitespace, and comments are all interpreted **literally** as part of the element value.
Thus:

    jimjam: ((block someScript:
        (1.2 / 3 * (25.6))    // I am not an LSON comment.
    block))

is equivalent to:

    jimjam: (someScript: "\n        (1.2 / 3 * (25.6))    // I am not an LSON comment.\n    ")


### Decoding Elements
All elements have a string representation of their value. In addition, for elements with declared
type, decoders may use this information to generate a native value of that type. For example, the
element `(boolean:true)` always has the string representation "true", and may have a decoder's
native Boolean value `True`. Decoders are thus domain-specific, and may handle a mix of elements of
both known and unknown types. This approach to typing allows unknown types to be handled
consistently across encode-decode transitions, and across data queries and transforms.

In this manner, a C++ decoder could meaningfully operate on LSON intended for a Python endpoint,
with values like `False` or `None`.

### Untyped Elements
Elements may omit type information, as in `(1.23456)` or `(:s/ab/xy/g)`. As for all elements, both
of these cases have their string representation. However, decoders will typically be able to deduce
the type of an element, according to a scheme of their choosing. For some element types and
decoders, this can be fairly trivial:

    (nullptr) → (nullptr:nullptr)     // C++
    (true) → (bool:true)              // C++
    (true) → (Boolean:true)           // JavaScript
    (true) → (Boolean:True)           // Python

A JSON-style decoder might employ a sequence of three recognizers:

1. null
2. boolean
3. number

Other common types may have associated type recognizers:

    (-1.234e6) → (Number:-1.234e6)    // JavaScript
    (-1.234e6) → (double:-1.234e6)    // C++
    (1..10) → (range:"range(1,10)")   // Python
    (#a3f4b9) → (color:0xa3f4b9ff)    // CSS color in C++
    (0x3ff0'0000'0000'0000) ...       // In C++, could be recognized as `uint64_t`

One reasonable approach for decoders is to maintain an ordered list of recognizers that are employed
in sequence to attempt to recognize a given untyped value. Enumerations such as `true`, `false`,
`null`, `None` and so forth would be first in the list, with more complicated types (like numbers,
colors, regular expressions) tried later in the sequence.

Note that values need only be recognized if the decoder intends to perform native operations with
those values. Decoders that just perform queries, transforms, or transmission need not care about
underlying implementations.


Bare Values
------------
LSON supports four data structures — everything else is considered an element. Elements may or may
not include a type declaration. In addition, any bare (undelimited) value is considered an untyped
element, with strings as the only natively-recognized type. Strings are recognized as such due to
any of the six quotation delimiters.

Consider the following bare values:

| Bare Value    | Recognized As
|---------------|-------------------------
| `"true"`      | `(string:true)`
| `"two words"` | `(string:"two words")`
|  `true`       | `(:true)`
|  `1.37`       | `(:1.37)`
| `plaid`       | `(:plaid)`
| `red\ blue`   | `(:"red blue")`
| `(a + b)`     | `(:a + b)`

Bare values that contain whitespace must either escape that whitespace (by prefixing with the `\`
character) or must use parenthesis delimeters.

Bare values are one of the key features of LSON. Whereas JSON supports a partial set of values
(boolean, null, numbers), it lacks other values that would be equally useful in different contexts:
hexadecimals, CSS colors, native primitive values (`None`, `Any`), and so on. The promotion of bare
value to untyped element provides a succint way to express arbitrary values in different domains,
while at the same time allowing for consistent treatment and handling of unrecognized types. Thus,
LSON establishes a hard boundary between data _representation_ and data _usage_.

On re-encoding, _an encoder should preserve original bare values as bare values_. For example, if a
transforming program decodes the bare literal `cromulent`, then any subsequent encoding should emit
`cromulent`, not `(cromulent)` or `(:cromulent)`.

In situations where it makes sense, recognizable values such as `true` or `0x113f` should also be
encoded as bare values, if these types are expected to be recognizable by the next decoder.

### Word → Element Promotion
A specific application may choose to recognize and handle a select set of element types. In such a
case, it is up to the application to specify the types handled, and the order in which they are
recognized.

For example, a standard JSON-type parser would handle the following ordered list of element types:

  1. null (`null`)
  2. boolean (`true`, `false`)
  3. real numbers (`[-]*[<digit>]*[.<digit>*][[eE][+-]?<digit>+]`, or some such syntax)

Other common parsers might support CSS values, ±infinity, and so forth. See [Elements.md][] for a
set of common element types.

### Bare Value Concatenation
The concatenation operator always promotes words to strings, to produce a string-valued result. For
example, the result of `0. + 123 + e10` would yield (string:0.123e10), not (0.123e10), which might
get promoted to (number:0.123e10).


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

  + Tables do not allow implicit null data; all row dimensions must be specified (except when using
    [Default Table Values][] described below). Thus, for a table
    with _N_ columns,
    there must be *M⋅N* data points in the table,
    where *M* is the number of table rows.

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

### Default Table Values
Tables may define default values for each column. When using bracketed tables, default values will
be used to set missing values when fewer values are specified than there are columns. For example:

    [#
        [ id status=idle ttl=120 ] :
        [ a173  running  300 ]    // id = a173, status=running, ttl=300
        [ b2fc  init         ]    // id = b2fc, status=init,    ttl=120
        [ 781d               ]    // id = 781d, status=idle,    ttl=120
    #]

In addition, the special character `~` can be used to specify a default value in a table row,
allowing for sparse table specifications:

    [#
        id=0000  status=idle  ttl=120 :

        ~     stopped    0     // id = 0000, status=stopped, ttl=0
        b2fc  init       ~     // id = b2fc, status=init,    ttl=120
        781d  ~          240   // id = 781d, status=idle,    ttl=240
        ~     running    ~     // id = 0000, status=running, ttl=120
    #]

The above two examples use bare values as the default. However, default values can be any value,
like so:

    [#  id=(count32:0)  lat=(real:0.00)  lon=(real:0.00)  strength=(HCategory:1) : ...

One can use a valueless element to set the default _type_ of table values, while also requiring a
value of that type, like so:

    [#
        [ id=(count32:)  lat=(real:)  long=(real:)  strength=(HCat:) ] :
        //____  ______  _______  _
        [ 01ca  -12.30   110.41  1 ]  // (count32:01ca), (real:-12.30), (real:110.41), (HCat:1)
        [ 021s       ~    70.58  3 ]  // ERROR: No default value for 'lat'.
        [ 9afb ]                      // ERROR: No default value for 'id', 'lat', 'lon'.
        ...

A standard CSV-type table might default all values to `(null)`:

    [#
        invoice=(null)  date=(null)  customerid=(null)  amount=(null)  address=(null):
        ...
    #]

Since any type can be specified, arrays, dictionaries, tables and graphs can also be used as default
values.


Graphs
-------
LSON supports graph data, where a graph is defined by a set of nodes and a set of edges between
those nodes. Graphs have the following properties:

  + Graphs are delimited with `[%` and `%]` tokens (no whitespace is allowed between delimiter
    characters.)
  + Each node may or may not have associated data.
  + Each edge may or may not have associated data.
  + Nodes may be unnamed, in which case they are referenced by index.
  + Nodes may be named, in which case they are referenced by name.
  + Edges may be directed or undirected.
  + An edge may leave and arrive at the same node.
  + There may be many edges between any node pair.


### General Graph Structure
Each LSON graph is expressed in the following pattern:

    [%
        // Node Data
        // Edge Data
    %]


### Graph Nodes
Graph nodes may be expressed in a number of ways, depending on

  - whether they are to be referenced by index or by name, and
  - whether they have associated data.

Unnamed nodes use an array-like container, while named nodes use a dictionary-like container.

#### Unnamed Graph Nodes Without Data
If nodes are to be referenced by index from 0 onward, and have no data, then simply specifying the
number of nodes is sufficient. Node count must be greater than or equal to zero, and is expressed as
a positive decimal integer.

    1000

#### Unnamed Graph Nodes With Data
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

#### Named Graph Nodes Without Data
To express a set of named nodes without additional data, use a dictionary with multi-keyed dummy
data, like so:

    { [ bargaining testing anger shock acceptance depression denial ]: null }

#### Named Graph Nodes With Data
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

#### Graph Nodes With Tabular Data
Node data may be expressed in the form of a table. Node IDs must be the first column of a node
table (the column name is arbitrary), and all such ID values must be unique (whether referenced or
not). Here's an example:

    [%
        [# nodeID, color=(color:black), weight=(number:0):
            a107c5, ~,      1.00;
            8c78e5, blue,   1.00;
            73ba4d, ~,      2.30;
            2b0ebb, indigo, 0.21;
        #]
        [
            a107c5 → a107c5,
            a107c5 → 2b0ebb,
            ...
        ]
    %]

### Graph Edges
Edges are expressed as a set of node pair relationships. A node relationship is either a node index
or name, followed by a relationship character, followed by the second node index or name.

The following are examples of node edges:

| Edge                 | Interpretation
|----------------------|------------------------------------------
| `a - b` <br> `a ↔ b` | An undirected edge between nodes a and b
| `a > b` <br> `a → b` | A directed edge from node a to node b
| `a < b` <br> `a ← b` | A directed edge to node a from node b

\* The special Unicode characters above are ↔ \[U+2194], → \[U+2192], and ← \[U+2190].

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


#### Graph Edges Without Data
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

#### Graph Edges With Data
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

#### Graph Edges With Tabular Data
Graph edges may be expressed using tabular data. As with node tables, the first column of edge
tables is special, and expected to hold the edge expressions. The name of the first is not
significant. Here's an example of edge data using a table:

    [%
        .... // Row data
        [#
            [ edge  ; status ; frinkiness    ]:
            //===== ; ====== ; ==============
            [ 2 > 0 ; hot    ; zoobnificent  ]
            [ 2 > 1 ; tepid  ; cromulipitant ]
            [ 1 > 2 ; molten ; breg          ]
            ...
        #]
    %]

#### Directed Graph Edges Via Adjacency Matrix
Directed graph edges may be expressed via adjacency matrix. In a directed adjacency matrix, each
edge is found in a square **N**×**N** matrix, where **N** is the number of nodes in the graph. In
this form, nodes are referenced by index (not by name), and the edge from node **A** to node **B**
is found in the **A**th row, **B**th column, with that entry containing the data for that edge.

Graphs using adjacency matrices must have indexable nodes. Thus, named nodes are not supported in
conjunction with adjacency matrices.

Here's an example of an adjacency matrix for a directed graph of three nodes:

    [%
        [ scout  dig  sleep ]
        [
            [ .67  .12  .21 ]    // Probability of scout→scout, scout→dig, scout→sleep
            [ .74  .21  .05 ]    // Probability of   dig→scout,   dig→dig,   dig→sleep
            [ .23  .29  .48 ]    // Probability of sleep→scout, sleep→dig, sleep→sleep
        ]
    %]

#### Undirected Graph Edges Via Adjacency Matrix
Adjacency matrixes with undirected edges require only an upper triangular matrix, where each entry
describes an edge between one index and an equal or greater index. Thus, the *i*th row has **N**-*i*
entries.

Graphs using adjacency matrices must have indexable nodes. Thus, named nodes are not supported in
conjunction with adjacency matrices.

Here's an example of an undirected adjacency matrix:

    [%
        3                           // Three nodes without data: 0, 1, 2
        [
            [ 23.1  445.  0.12 ]    // 0-0  0-1  0-2
                  [ 1.72  34.7 ]    //      1-1  1-2
                        [ 7.56 ]    //           2-2
        ]
    %]


### Graph Examples
Indexed nodes without data, edges without data:

    [%
        1000
        [ 0→500; 1→548; 2→23; 3→897; ... ]
    %]

Indexed nodes with 2D coordinate data, plus edges without data:

    [%
        [ [3 2] [2 2] [2 1] [1 3] [1 2] [1 1] ]
        [ 0-3, 1-4, 2-5, 3-4, 1-2 ]
    %]

The seven stages of grief: named nodes without data, edges without data:

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
        // Named nodes without data
        {[
          start  wholeDigit    fractionalDigit  exponentCharacter
          sign   decimalPoint  exponentSign     exponentDigit
          end
        ]:null}

        // Edges without data
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

    string-begin-quote ::= "\"" | "'" | "`" | "«" | "‘" | "“"
    string-end-quote   ::= "\"" | "'" | "`" | "»" | "’" | "”"

    concatenated-string ::= "+" ( <word> | <string> )

    id ::= <string>

    terminator ::= "," | ";" | <whitespace> | empty-before-closing-delimiter

    element ::= <untyped-element> | <typed-element>
    untyped-element ::= "(" <string> ")" | "(("<id> <text> <id>"))"
    typed-element ::= "(" <typeID> ":" <string> ") | "(("<id> <typeID> ":" <text> <id>"))"
    valueless-element ::= "(" <typeID> ":" ")"

    dictionary ::= "{" <dictionary-body> "}"

    dictionary-body ::= <dictionary-item>*
    dictionary-item ::= <key> ":" <value> <terminator>
    key ::= <id> | "[" <key>+ "]"

    array ::= "[" <array-item>* "]"
    array(n) ::= "[" <array-item>{n} "]"
    array-item ::= <value> <terminator>

    table ::= "[#" <table-body> "#]"
    table-body ::= <table-body-bracketed> | <table-body-unbracketed>

    table-body-bracketed ::= "[" <table-feature>* "]" ":" <table-row-bracketed>*
    table-row-bracketed(n) ::= "[" <table-row-bare>(*) "]"

    table-feature ::= <id> <terminator>
                    | <id> "=" <value> <terminator>
                    | <id> "=" <valueless-element>

    table-body-unbracketed ::= <table-feature>{n+} ":" <table-row-bare>(n+)*
    table-row-bare(n) ::= (<table-row-feature> <terminator>){n}
    table-row-feature ::= <value> | "~"

    graph ::= explicit-graph | adjacency-graph
    explicit-graph ::= "[%" <graph-nodes> <explicit-graph-edges> "%]"
    adjacency-graph ::= "[%" <indexed-nodes>(n) <adjacency-matrix>(n) "%]"

    "[%" <graph-nodes> <graph-edges> "%]"

    graph-nodes ::= <indexed-nodes> | <named-nodes> | <node-table>
    indexed-nodes ::= <counting-number> | <array>(n)
    named-nodes ::= <dictionary>
    node-table ::= <table>

    explicit-graph-edges ::= <edge-array> | <edge-dictionary> | <edge-table>
    edge-array ::= "[" <edge>+ "]"
    edge-dictionary ::= "{" (<edge-key> ":" <value> <terminator>)* "}"
    edge-key ::= <edge> | "[" (<edge> <terminator>)+ "]"
    edge-table ::= <table>

    adjacency-matrix ::= <directed-adjacency-matrix> | <undirected-adjacency-matrix>
    directed-adjacency-matrix ::= "[" <array>(n){n} "]"
    undirected-adjacency-matrix ::= "[" <array>(n) <array>(n-1) .. <array>(1) "]"

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



[Appendix A: Grammar]:                         #appendix-a-grammar
[Appendix B: Common Element Types]:            ./ElementTypes.md
[Appendix C: LSON Examples]:                   ./Examples.md
[Arrays]:                                      #arrays
[Bare Value Concatenation]:                    #bare-value-concatenation
[Bare Values]:                                 #bare-values
[Comments]:                                    #comments
[Conclusion]:                                  #conclusion
[Decoding Elements]:                           #decoding-elements
[Default Table Values]:                        #default-table-values
[Dictionaries]:                                #dictionaries
[Directed Graph Edges Via Adjacency Matrix]:   #directed-graph-edges-via-adjacency-matrix
[Element Types]:                               #element-types
[Element Values]:                              #element-values
[Element Value Blocks]:                         #element-value-blocks
[ElementTypes.md]:                             ./ElementTypes.md
[Elements]:                                    #elements
[Elements of Type String]:                     #elements-of-type-string
[Escape Sequences]:                            #escape-sequences
[General Graph Structure]:                     #general-graph-structure
[Graph Edges With Data]:                       #graph-edges-with-data
[Graph Edges With Tabular Data]:               #graph-edges-with-tabular-data
[Graph Edges Without Data]:                    #graph-edges-without-data
[Graph Edges]:                                 #graph-edges
[Graph Examples]:                              #graph-examples
[Graphs]:                                      #graphs
[LSON Example]:                                #lson-example
[Named Graph Nodes With Data]:                 #named-graph-nodes-with-data
[Named Graph Nodes Without Data]:              #named-graph-nodes-without-data
[Graph Nodes]:                                 #graph-nodes
[Graph Nodes With Tabular Data]:               #graph-nodes-with-tabular-data
[Overview]:                                    #overview
[Special Values]:                              #special-values
[String Concatenation]:                        #string-concatenation
[Strings]:                                     #strings
[Tables]:                                      #tables
[Undirected Graph Edges Via Adjacency Matrix]: #undirected-graph-edges-via-adjacency-matrix
[Unnamed Graph Nodes With Data]:               #unnamed-graph-nodes-with-data
[Unnamed Graph Nodes Without Data]:            #unnamed-graph-nodes-without-data
[Untyped Elements]:                            #untyped-elements
[Whitespace]:                                  #whitespace
[Word Concatenation]:                          word-concatenation
[Word → Element Promotion]:                    #word--element-promotion
[Words]:                                       #words

[standard Unicode whitespace characters]: https://en.wikipedia.org/wiki/Whitespace_character#Unicode

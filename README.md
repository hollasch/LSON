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


Character Escapes
-----------------

Strings may contain the following escaped characters:

| Sequence   | Description
|:-----------|:-----------------------------------------------------
| `\\`       | backslash
| `\"`       | double quote
| `\'`       | single quote
| `\n`       | new line
| `\<cr><lf>`| empty
| `\<lf>`    | empty
| `\r`       | carriage return
| `\t`       | horizontal tab
| `\u####`   | Unicode character with four hexadecimal digits
| `\U######` | Unicode character with six hexadecimal digits


Optional String Quoting
-----------------------

Strings that do not contain whitespace do not require quotes.

It it important to note that this feature is more useful than just as a shorthand for more efficient
representation: it also provides a great mechanism for handling special values.

For example, consider the values `null`, `undefined`, `QNaN`, `SNaN`, and `Maybe`. Various targets
will be able to support subsets of this set. For example, C++ has no concept of `undefined`, `null`
only applies to pointers, and has no support for`Maybe` (an indefinite Boolean value).
In JavaScript, a `NaN` value is a `QNaN`, doesn't provide a way to create `SNaN`, and doesn't
support `Maybe`. One of the weaknesses of standard JSON is that it implicitely favors JavaScript's
data model.

If single-word strings don't require quotes, then special values can be naturally expressed in
humon, and interpreters can handle them as native values, or as string values. In order to make such
values durable, it is best if encoders always omit quotes when possible.

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

Note that this suggests that special values can be introduced at will, though it is likely that
decoders will interpret these values as strings.


Reserved Words
---------------

The following special values are reserved in Humon:

  - `null`
  - `true`
  - `false`
  - `NaN`
  - `infinity`


String Concatenation
--------------------

In order to support human-readable long strings, the `+` operator may be used to construct
concatenations. For example:

    {
        strBlock: "Knock knock.\n"
                + "Who's there?\n"
                + "Bug in your state machine.\n"
                + "Who's there?\n"
    }

Note that string concatenation promotes all values to strings. Non-string values are promoted to
strings of their verbatim representation. Thus:


    X: 1.000 + null + false    (Gets the string value "1.000nullfalse")


Structures
----------

The following fragment:

    {
        someStruct <key1 key2 key3>: [
            < thing1 false  3 >
            < thing2 false 13 >
            < thing3  true 37 >
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

Note that this is not the most efficient way to express CSV, would be more like this:

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


Macros / Definitions
--------------------
Developers have long lamented the lack of any decent macro or definition construct in CSS. Such
features improve readability, decrease errors, and significantly improve maintainability. Adding
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
        logMsg(type,msg) = "[" + type + "] " + msg

        noFile:   logMsg (ERROR,   "Can't find file")
        openFail: logMsg (ERROR,   "Can't open file")
        dateFail: logMsg (WARNING, "Couldn't update file date")
    }

And so forth. However, just because this could get vastly more complicated, that doesn't mean we
should close the door on every subset. It should be pretty clear that simple value definition is
immediately and widely useful, and relatively easy to implement. Undefining a variable is as easy as
restoring default behavior:

    previously_defined_definition = "previously_defined_definition"

Definitions can have two scopes: global and object. Definitions can appear anywhere in an object,
but must occur before use.

Definitions can cascade. Each definition is resolved _immediately_ as it is encountered. If it uses
other definitions, their already-resolved values are used in the new definition. This allows a large
degree of composition at minimal cost and without risk of infinite recursion. For example:

    A = B             (A = "B")
    B = A + foo       (B = "Bfoo")
    C = A             (C = "B")
    C = C + B         (C = "BB")
    D = D + A         (D = "DB")
    B = A + C         (B = "BBB")

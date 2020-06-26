LSON: Lucid Serialized Object Notation
====================================================================================================
_If I had a dime for every developer who thought of creating a better JSON…_

- [LSON Specification][]

About
------
The JSON file format is beautiful in its simplicity and power. With a surprisingly succinct syntax,
it is able to express data in a huge variety of formats. Yet there are little annoyances of the
format: the lack of comments, mandated string quoting, mandated key quoting, double quotes only,
strict syntax for commas, limitations in numeric values, and so forth.

LSON was a personal doodle in my spare time to see how I'd like to see the format evolve. In the
beginning, I made all the standard tweaks. However, in working and re-working it, I encountered
ideas that I found to be quite powerful, and ideas building from those. In the end, I arrived at a
format that I believe keeps the ultimate simplicity of the file format, with greater power and
expressiveness.

Here's a list of some of LSON's more interesting features:

  1. It's intended to be both concise and readable by humans as well as computers. It supports
     comments. Items are optionally terminated by whitespace, end delimiters, commas or semi-colons.

  2. It does not aim to mirror JavaScript, and thus is not a JavaScript subset. At the same time,
     LSON is a superset of JSON: any legal JSON file is legal LSON.

  3. LSON focuses on data _representation_, not data _usage_. With the single exception of string
     values, there is no intrinsic support for numbers, boolean, or any other primitive type.

  4. LSON supports arbitrary _elements_: domain-specific data values with declared or unknown type.
     Elements provide first-class support for domain-specific values such as `true`, `null`,
     `infinity`, `2018-07-02`, `#6b17ec`, `(x,a,b) => { a <= x && x <= b }`, and so forth. LSON
     encoders and decoders handle both known and unknown types in a consistent and predictable
     manner.

  5. LSON adds two new data structures to JSON's array and dictionary: table and graph.

You can browse https://hollasch.github.io/LSON/examples.md.html for a quick scan of what LSON looks
like.


LSON Tools
-----------
I am just starting on an LSON parser and command-line utility in C++. Ultimately this will fork into
an LSON C++ library, and one or several standalone LSON command-line utilities. You can find this in
[the `src` directory](src/).

In addition, [Connor Hollasch][] has written  a [Java library][] for an early version of LSON.


Building
----------
This project uses the CMake build tool. CMake is a meta-build system that locates and uses your
local development tools to build the project if possible.

To build, first install [CMake][https://cmake.org/]. Then go to the project root directory and run
the following command:

    cmake -B build

This will locate your installed development tools and configure your project build in the `build/`
directory. After that, whenever you want a new build, run this command:

    cmake --build build

This will build a debug version of the project, located in `build/Debug/`. To build a release
version, run

    cmake --build build --config release

You can find the built release executable in `build/Release/`.



[Connor Hollasch]:    https://github.com/chollasch/
[Examples.md]:        https://hollasch.github.io/LSON/examples.md.html
[Java library]:       https://github.com/chollasch/LSON4J
[LSON Specification]: https://hollasch.github.io/LSON


LSON Notes
====================================================================================================

Naming
-------
    structured   data    form
    lucid                format
    simple               shape
    concise              language
    general              protocol
                         lattice
                         mold
                         scaffold
                         armature
                         structure
                         skeleton
                         frame
                         truss
                         gantry
                         framework

    + dProt (data protocol) - Available
    + dSkel (data skeleton)

    > dataform - taken
    > dlang - commonly used for D language
    > dForm - commonly used


Alternate Table Format
-----------------------
    [# [ feature1  feature2:foo  feature3:(float:1) ]    // Dictionary style
       // Rows
    #]


Identifiers
------------

### Form 1
    [ @foo ... ]                          // Array
    { @bar ... }                          // Dictionary
    [# @baz [ @qux ... ] ... #]           // Can reuse whole table or just schema (not body only)
    [% @gor [ @hoz ... ] [ ... ] %]       // Can reuse whole graph or just nodes (not edges only)
    [% @lof { @oog ... } { ... } %]       // ^

### Form 2
    zuz=#ae32fd                         // Can be used with any value
    foo=[ ... ]                         // Array
    bar={ ... }                         // Dictionary
    baz=[# qux=[ ... ] ... #]           // Can reuse whole table or just schema (not body only)
    gor=[% hoz=[ ... ] [ ... ] %]       // Can reuse whole graph or just nodes (not edges only)
    lof=[% oog={ ... } { ... } %]       // ^


References
-----------
    ^foo                // Value expected, cannot use schema or nodes(?)
    [# ^grotz ... #]    // Schema expected, must use schema
    [% ^quonk ... %]    // Nodes expected, must use nodes

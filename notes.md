
====================================================================================================

[
    "string" 'string' `string` “string” ‘string’ «string» // Strings

    ()                // Null Element

    (none)            // Untyped Values
    (x)
    (#ff7d11)
    (true)
    (off)

    none              // Bare Words
    x
    #ff7d11
    true
    off

    (signed:73)       // Typed Values
    (double:24.8)
    (optional:none)
    (rgba: 0.87 0.44 0.21 0.40)

    (reference:)      // Typed Null

    ((block script/javascript:
        (a,b,c) => { return 2*a + b*b - c*c }
    block))
]

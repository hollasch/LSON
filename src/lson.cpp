//==================================================================================================
// lson.cpp
//
// This is the first draft of an LSON lexer/parser. Eventually it will grow into a C++ library and a
// command-line utility.
//==================================================================================================

#include <stdio.h>
#include <stdlib.h>

#include <string>


struct Parameters
{
    bool someFlag { false };
    bool printHelp { false };
};


// Function Declarations
bool getParameters (Parameters& params, int argc, wchar_t* argv[]);


//--------------------------------------------------------------------------------------------------
int wmain (int argc, wchar_t* argv[])
{
    Parameters params;
    if (!getParameters(params, argc, argv)) return -1;

    wprintf (L"Someflag = %s\n", params.someFlag ? L"true" : L"false");

    if (params.printHelp) {
        wprintf (L"Help!\n");
    }

    return 0;
}


//--------------------------------------------------------------------------------------------------
bool getParameters (Parameters& params, int argc, wchar_t* argv[])
{
    // Processes command-line arguments, and loads resulting values into the Parameters structure.
    // This function returns true if all parameters are valid.

    for (auto i=1;  i < argc;  ++i) {
        auto argptr = argv[i];

        // Handle Windows standard "/?" help switch.
        if (0 == _wcsicmp(argptr, L"/?")) {
            params.printHelp = true;
            return true;
        }

        auto unexpectedArgument = false;

        if (argptr[0] != L'-') {

            // Non-dash Arguments
            unexpectedArgument = true;

        } else if (argptr[1] == L'-') {

            // Double-Dash Options
            argptr += 2;

            if (0 == _wcsicmp(argptr, L"help")) {
                params.printHelp = true;
                return true;
            } else {
                unexpectedArgument = true;
            }

        } else {

            // Single-Dash Options
            argptr += 1;
            if (*argptr == L'h' || *argptr == L'H')
                params.printHelp = true;
            else if (*argptr == L'f')
                params.someFlag = true;
            else
                unexpectedArgument = true;
        }

        if (unexpectedArgument) {
            fwprintf (stderr, L"ERROR: Unexpected argument (%s).\n", argv[i]);
            return false;
        }
    }

    return true;
}

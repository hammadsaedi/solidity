contract X{
    function foo(string code input) internal pure {
        string code test = input;
    }
}
// ----
// SyntaxError 2397: (29-46): Usage of "code" as a data location is not yet supported.
// SyntaxError 2397: (72-88): Usage of "code" as a data location is not yet supported.
// TypeError 6651: (29-46): Data location must be "storage", "memory" or "calldata" for parameter in function, but "code" was given.
// TypeError 6651: (72-88): Data location must be "storage", "memory" or "calldata" for variable, but "code" was given.

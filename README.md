# nulascript

[![Unit Tests Nulascript](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml)

[![Build Nulascript REPL](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml)

### How to test:

1. `make install-gtest` - pulls googletest locally
2. `make run-tests` - links googletest, builds and runs the tests executable

### Syntax:
#### Conditional Statements:
```javascript
// equals
if (100 is 100) {
    return ...;
} 

// doesn't equal
if (100 is not 200) {
    return ...;
}

/* "not" keyword is interchangeable for '!'
all of the below evaluate to true:

   true is not !true
   true is not not true
   true is ! ! true
*/

// equal or greater than
if (100 >= 200) {
    return ...;
}

// equal or less than
if (100 <= 200) {
    return ...;
}
```

#### Operators:
```javascript
let a = not true;   // assigned value is false
let a = !true;      // assigned value is false

TBA
```
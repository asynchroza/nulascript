# nulascript

[![Unit Tests Nulascript](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml)

### How to test:

1. `make install-gtest` - pulls googletest locally
2. `make run-tests` - links googletest, builds and runs the tests executable

### Syntax:
#### Conditional Statements:
```javascript
// equals
if (100 is 100) {
    return true;
} 

// doesn't equal
if (100 is not 200) {
    return false;
}

// equal or greater than
if (100 >= 200) {
    return false;
}

// equal or less than
if (100 <= 200) {
    return true;
}
```

#### Operators:
```javascript
let a = not true  // bang operator negates the value

TBA
```
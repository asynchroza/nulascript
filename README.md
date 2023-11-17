# nulascript

[![Unit Tests Nulascript](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml)

[![Build Nulascript REPL](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml)

## About the language:
- Recursive descent parser (Pratt parser - "top down operator precedence")

### Referencing:

```
Nulascript:
> def sayHello = func() { "hello"; }  
[function]:
    arguments: []
    location: "0x12ef04db0"

> sayHello()
hello

> def sayByeToPerson = func(name) { "bye " + name; }
[function]:
    arguments: [name]
    location: "0x12f904f10"

> sayByeToPerson("Misho")
bye Misho

> def exampleRef = &sayByeToPerson
[function]:
    arguments: [name]
    location: "0x12f904f10"

> exampleRef("Toni")
bye Toni

> sayByeToPerson = 5 
5

> exampleRef()
[ERROR]: An invocation was executed on an element which is not a function

> exampleRef
5

```

### Dereferencing:

```
Nulascript:
> def somethingToBeReferenced = 15;
15

> def reference = &somethingToBeReferenced
15

> reference + 20
[ERROR]: Type missmatch. Left side is REFERENCE and right side is INTEGER

> *reference + 20
35

> reference = *reference + 40
55

> somethingToBeReferenced
55

```

### Commands:

- `make install-gtest` - pulls googletest locally
- `make run-tests` - links googletest, builds and runs the tests executable
- `make run-repl` - builds and runs the REPL

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
```python
def a = not true;   # assigned value is false
def a = !true;      # assigned value is false
```

#### Functions:
```python
# functions are first class citizens in nulascript
def a = func(x, y) {
    x + y;
}
```

### Loops
#### Incremental
```python
> for (def i = 5; i < 10; true) { log(i); };
5
6
7
8
9

```

#### Examples:
```
Nulascript:
> def a = func() {};
[ERROR]: Functions with empty bodies are not allowed

> "some string" + 5
[ERROR]: Type missmatch. Left side is STRING and right side is INTEGER
```

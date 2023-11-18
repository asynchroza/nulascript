# nulascript

[![Unit Tests Nulascript](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml)

[![Build Nulascript REPL](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml)

## About the language:
- Recursive descent parser (Pratt parser - "top down operator precedence")

### Referencing:

```python
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

```python
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
* Syntax structure of the loop requires the initialization of a variable in the loop header
* Initialized variable can be either a reference to an integer or an integer
* There is no need to dereference in the loop header, but it's required in the body
* Loops work with the following arithmetic operators: `+`, `-`, `*`, `/`


```python
> for (def i = 5; i < 10; i + 1) { log(i); };
5
6
7
8
9
```

#### Referencing variables in loops
```python
> def a = 5;
5

# NB: you don't need to dereference references in loop conditionals
> for (def b = &a; b < 10; i + 1) { log("hello"); }
hello
hello
hello
hello
hello

> a
10

> b
[ERROR]: b is undefined
```

#### No header dereferencing, but you should do it in the body
```python
> for (def b = &a; b < 10; b + 1) { log(*b); }
5
6
7
8
9
```

#### Cheating the system with built-ins
```python
def something = func() { log("say hello"); }

> loop(5, something)
say hello
say hello
say hello
say hello
say hello
```

#### Examples:
```python
> def a = func() {};
[ERROR]: Functions with empty bodies are not allowed

> "some string" + 5
[ERROR]: Type missmatch. Left side is STRING and right side is INTEGER
```

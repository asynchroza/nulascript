# nulascript

[![Unit Tests Nulascript](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/unit-tests.yaml)

[![Build Nulascript REPL](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/build-repl.yaml)

[![Interpreter Test Nulascript](https://github.com/asynchroza/nulascript/actions/workflows/interpreter-tests.yaml/badge.svg)](https://github.com/asynchroza/nulascript/actions/workflows/interpreter-tests.yaml)

## References

```python
def referred = 5;

def a = &referred;

a = 10;
log(referred);      # 10

log(*a + referred); # 20
```

## Conditionals

```python
def x = 5;

if (x is 10) {  # "is" is interchangeable with "=="
    log("hello world");
} else {
    log("bye world");
}

if (x is not 10) {  # "is not" is interchangeable with "!="
    log("hello world");
} else {
    log("bye world");
}
```

### Find more code examples [here](/examples)

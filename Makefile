PHONY: run-tests
run-tests:
	cd ctests && cmake . && cmake --build . && ./tests --gtest_break_on_failure

PHONY: run-repl
run-repl:
	cd nulascript/repl/build && cmake . && cmake --build . && ./repl

PHONY: install-gtest
install-gtest:
	git clone https://github.com/google/googletest.git -b v1.14.0; \
	cd googletest; \
	mkdir build; \
	cd build; \
	cmake ..
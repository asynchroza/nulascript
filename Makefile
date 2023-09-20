PHONY: run-tests
run-tests:
	cd ctests && cmake . && cmake --build . && ./tests
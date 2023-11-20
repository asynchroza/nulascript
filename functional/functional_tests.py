# test_interpreter.py
import unittest
from interpreter import run_interpreter


class TestInterpreter(unittest.TestCase):
    def test_example_nula(self):
        filenames = {
            "loops.nula": "5 \n10 \n20 \n40 \n80 \nThe value of the referred variable is:  1250 \nThe value of the referred variable is:  3125 \nThe value of the referred variable is:  7812 \n15624",
            "closures.nula": "Hello Misho! \nBye Misho!",
            "logging.nula": "5 \n5 \n5 5",
            "conditionals.nula": "hello world \ntrue \nfalse \nfalse \ntrue \ntrue \nfalse \nfalse \ntrue \n5 is a truthy value"
        }

        for filename, expected_output in filenames.items():
            actual_output = run_interpreter(f"../examples/{filename}")
            self.assertEqual(actual_output, expected_output)


if __name__ == "__main__":
    unittest.main()

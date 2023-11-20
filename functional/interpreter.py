import subprocess
import sys

def run_interpreter(filename):
    try:
        result = subprocess.check_output(["../bin/nulascript", filename], universal_newlines=True)
        return result.strip()
    except subprocess.CalledProcessError as e:
        return f"Error: {e}"

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python interpreter.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    output = run_interpreter(filename)
    print(output)

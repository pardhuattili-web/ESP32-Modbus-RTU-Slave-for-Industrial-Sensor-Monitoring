import pathlib
import subprocess
ROOT = pathlib.Path(__file__).resolve().parents[1]
def test_make_test():
    result = subprocess.run(["make", "test"], cwd=ROOT, capture_output=True, text=True, check=False)
    assert result.returncode == 0, result.stdout + result.stderr
    assert "All Modbus RTU tests passed." in result.stdout

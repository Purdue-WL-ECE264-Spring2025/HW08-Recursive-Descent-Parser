import glob
import subprocess
from gradescope_utils.autograder_utils.files import check_submitted_files # type: ignore

RESULTS_FILE = "results/results.json"
TEST_WEIGHT = 5, 3, 1

instructor_name = {
    "t": "`Term`",
    "e": "`Expr`",
    "f": "`Factor`"
}

def try_test(test_name: str, exe: str, filename: str, expected: str):
    bin_name = f"./hw08.{exe}.out" if exe else "./hw08.out"
    result = subprocess.run(f"{bin_name} {filename}", shell=True, capture_output=True, text=True)
    if result.returncode == 0 and result.stdout.strip() == expected:
        return {
            "name": test_name,
            "score": TEST_WEIGHT[len(exe)],
            "max_score": TEST_WEIGHT[0],
            "status": "passed",
            "output": f"Used instructor {' '.join(instructor_name[x] for x in exe)}" if exe else ""
        }
        
    return False

def run_testcase(filename: str, expected: str):
    test_name = filename
    score = 0

    return \
        try_test(test_name, "", filename, expected) or\
        try_test(test_name, "e", filename, expected) or\
        try_test(test_name, "t", filename, expected) or\
        try_test(test_name, "f", filename, expected) or\
        try_test(test_name, "et", filename, expected) or\
        try_test(test_name, "tf", filename, expected) or\
        try_test(test_name, "ef", filename, expected) or\
    {
        "name": test_name,
        "score": score,
        "status": "failed"
    }
    

def run_tests():
    # verify files submitted
    if check_submitted_files(['hw08.c'], base='.'):
        return {
            "name": "Files",
            "score": 0,
            "status": "failed",
            "output": "Missing file `hw08.c"
        }
        
    # check for compilation errors
    result = subprocess.run(
            "make hw08.out", 
            shell=True, capture_output=True, text=True)
    
    if result.returncode:
        return {
            "name": "Compilation test",
            "score": 0,
            "status": "failed",
            "output": f"Compilation error:\n\n{result.stderr}"
        }
        
        
    # generate all the partial credit binaries
    subprocess.run('rm *.o && make hw08.out ISOLS="E" && mv hw08.out hw08.e.out', shell=True)
    subprocess.run('rm *.o && make hw08.out ISOLS="T" && mv hw08.out hw08.t.out', shell=True)
    subprocess.run('rm *.o && make hw08.out ISOLS="F" && mv hw08.out hw08.f.out', shell=True)
    subprocess.run('rm *.o && make hw08.out ISOLS="E T" && mv hw08.out hw08.et.out', shell=True)
    subprocess.run('rm *.o && make hw08.out ISOLS="T F" && mv hw08.out hw08.tf.out', shell=True)
    subprocess.run('rm *.o && make hw08.out ISOLS="E F" && mv hw08.out hw08.ef.out', shell=True)
    subprocess.run('rm *.o && make hw08.out', shell=True)
    
    
    test_files = glob.glob("testcases/valid/*.txt") + glob.glob("testcases/invalid/*.txt")
    
    test_results = []
    for f in test_files:
        test_results.append(run_testcase(f, "Invalid" if "invalid" in f else "Valid"))
    
        
    return {"tests": test_results}
        
    
    
    

if __name__ == '__main__':
    import json
    json.dump(run_tests(), open("/autograder/results/results.json", "w"))
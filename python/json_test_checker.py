import pathlib
import os
import subprocess
import json

basedir="data/json"
inputdir="test_checker"
outputdir="test_checker_output"

c_binname="./json_valid.d"

if __name__ == "__main__":
    os.makedirs(f'{basedir}/{outputdir}',exist_ok=True)
    subprocess.run(["make", c_binname])
    ress = []
    for pfile in os.listdir(f'{basedir}/{inputdir}'):
        full_name = f'{basedir}/{inputdir}/{pfile}'
        res = subprocess.run([f"./{c_binname}", full_name],
            capture_output=True, text=True
        )
    
        should_res = 'y' if pfile[0]=='p' else 'n'

        py_res = ''
        try:
            json.loads(open(full_name).read())
            py_res = 'y'
        except Exception:
            py_res = 'n'
        
        if should_res!=res.stdout:
            ress.append({
                "src_file": full_name,
                "should_res": should_res,
                "python_res": py_res,
                "myc_res": res.stdout,
            })

    with open(f'{basedir}/{outputdir}/r.json','w') as ressf:
        ressf.write(json.dumps(ress))
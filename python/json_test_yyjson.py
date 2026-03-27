import pathlib
import os
import subprocess
import json

basedir="data/json"
inputdir="test_yyjson"
outputdir="test_yyjson_output"

c_binname="./json_valid.d"

if __name__ == "__main__":
    os.makedirs(f'{basedir}/{outputdir}',exist_ok=True)
    subprocess.run(["make", c_binname])
    ress = []
    for pfile in os.listdir(f'{basedir}/{inputdir}'):
        full_fname = f'{basedir}/{inputdir}/{pfile}'
        res = subprocess.run([f"./{c_binname}", full_fname],
            capture_output=True, text=True
        )

        py_res = ''
        try:
            json.loads(open(full_fname).read())
            py_res = 'y'
        except Exception:
            py_res = 'n'

        if py_res!=res.stdout:
            ress.append({
                "src_file": full_fname,
                "python_res": py_res,
                "myc_res": res.stdout, 
            })

    with open(f'{basedir}/{outputdir}/r.json','w') as ressf:
        ressf.write(json.dumps(ress))
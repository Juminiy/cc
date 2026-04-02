import pathlib
import os
import subprocess
import json

basedir="data/json"
inputdir="test_yyjson"
outputdir="test_yyjson_output"

c_binname="json_valid.d"
go_binname="json_valid.go.d"

def json_valid_py(filename:str):
    py_res = ''
    try:
        json.loads(open(filename).read())
        py_res = 'y'
    except Exception:
        try:
            json.loads(open(filename,'rb').read())
            py_res = 'y'
        except Exception:
            py_res = 'n'
    return py_res

def json_valid(filename:str):
    py_res = json_valid_py(filename)
    go_res = subprocess.run([f"./{go_binname}", filename],
        capture_output=True, text=True
    )
    myc_res = subprocess.run([f"./{c_binname}", filename],
        capture_output=True, text=True
    )
    return (py_res,go_res.stdout,myc_res.stdout)

if __name__ == "__main__":
    os.makedirs(f'{basedir}/{outputdir}',exist_ok=True)
    subprocess.run(["make", c_binname, go_binname])
    ress = []
    for pfile in os.listdir(f'{basedir}/{inputdir}'):
        full_fname = f'{basedir}/{inputdir}/{pfile}'

        py_res,go_res,myc_res = json_valid(full_fname)

        if not (go_res==myc_res):
            ress.append({
                "src_file": full_fname,
                "python_res": py_res,
                "go_res": go_res,
                "myc_res": myc_res,
            })

    with open(f'{basedir}/{outputdir}/r.json','w') as ressf:
        ressf.write(json.dumps(ress))
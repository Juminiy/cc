import pathlib
import os
import subprocess
import json

basedir="data/json"
inputdir="test_transform"
outputdir="test_transform_output"

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
        full_name = f'{basedir}/{inputdir}/{pfile}'

        py_res,go_res,myc_res = json_valid(full_name)

        if not (go_res==myc_res):
            if full_name.count("object_same_key")>0:
                continue
            ress.append({
                "src_file": full_name,
                "python_res": py_res,
                "go_res": go_res,
                "myc_res": myc_res,
            })

    with open(f'{basedir}/{outputdir}/r.json','w') as ressf:
        ressf.write(json.dumps(ress))
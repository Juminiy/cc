import pathlib
import os
import subprocess
import json

basedir="data/json"
inputdir="test_roundtrip"
outputdir="test_roundtrip_output"

c_binname="./json_valid.d"

def json_parse(filename:str):
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

if __name__ == "__main__":
    os.makedirs(f'{basedir}/{outputdir}',exist_ok=True)
    subprocess.run(["make", c_binname])
    ress = []
    for pfile in os.listdir(f'{basedir}/{inputdir}'):
        fullname = f'{basedir}/{inputdir}/{pfile}'
        res = subprocess.run([f"./{c_binname}", fullname],
            capture_output=True, text=True
        )

        py_res = json_parse(fullname)

        if res.stdout!=py_res:
            ress.append({
                "src_file": fullname,
                "python_res": py_res,
                "myc_res": res.stdout, 
            })

    with open(f'{basedir}/{outputdir}/r.json','w') as ressf:
        ressf.write(json.dumps(ress))
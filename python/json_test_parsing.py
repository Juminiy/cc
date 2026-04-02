import os 

import subprocess
import json

import pathlib

basedir="data/json"
input_dir="test_parsing"
output_dir="test_parsing_output"

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

def run_json_test_d():
    yfile,nfile,efile,ifile = [],[],[],{"n":[],"y":[]}
    subprocess.run(["make", c_binname, go_binname])
    tot,cnt = 0,0
    for pfile in os.listdir(f'{basedir}/{input_dir}'):
        tot+=1
        should_res = pfile[0]
        pfile_full = f'{basedir}/{input_dir}/{pfile}'

        py_res,go_res,myc_res = json_valid(pfile_full)

        if not (should_res==go_res==myc_res):
            if pfile.count("duplicated_key"): # my_c_program do not allow duplicated key
                continue
            if myc_res == "":
                efile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "python_res": py_res,
                    "go_res": go_res,
                    "myc_res": myc_res,
                })
                cnt+=1
            elif should_res=='y':
                yfile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "python_res": py_res,
                    "go_res": go_res,
                    "myc_res": myc_res,
                })
                cnt+=1
            elif should_res=='n':
                nfile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "python_res": py_res,
                    "go_res": go_res,
                    "myc_res": myc_res,
                })
                cnt+=1
            elif should_res=='i':
                if not (go_res==myc_res):
                    ifile[myc_res].append({
                        "src_file": pfile_full,
                        "should_res": should_res,
                        "python_res": py_res,
                        "go_res": go_res,
                        "myc_res": myc_res,
                    })
    
    with open(f'{basedir}/{output_dir}/y.json', 'w') as fp:
        fp.write(json.dumps(yfile))
    with open(f'{basedir}/{output_dir}/n.json', 'w') as fp:
        fp.write(json.dumps(nfile))
    with open(f'{basedir}/{output_dir}/e.json', 'w') as fp:
        fp.write(json.dumps(efile))
    with open(f'{basedir}/{output_dir}/i.json', 'w') as fp:
        fp.write(json.dumps(ifile))
    
    print(f'fail: {cnt}/{tot}, y: {len(yfile)}, n: {len(nfile)}, e: {len(efile)}')

def generate_pretest_file():
    numf = open(f'{basedir}/{output_dir}/num.txt','w')
    strf = open(f'{basedir}/{output_dir}/str.txt','w')
    for pfile in os.listdir(f'{basedir}/{input_dir}'):
        if pfile[:8]=='y_number' or pfile[:8]=='n_number' or \
            pfile[:8]=='y_string' or pfile[:8]=='n_string':
            try:
                bs = open(f'{basedir}/{input_dir}/{pfile}', 'r').readline()
                bsstr = str(bs).removesuffix('\n')
                target = bsstr
                if bsstr.startswith("[") and bsstr.endswith("]"):
                    target = bsstr.removeprefix("[").removesuffix("]")
                # elif bsstr.startswith("'") and bsstr.endswith("'"):
                #     target = bsstr.removeprefix("'").removesuffix("'")
                if pfile[:8]=='y_number' or pfile[:8]=='n_number':
                    numf.write(f'{target} {pfile[0]}\n')
                if pfile[:8]=='y_string' or pfile[:8]=='n_string':
                    strf.write(f'{target} {pfile[0]}\n')
            except Exception as e:
                print(f'file: {pfile}, read exception: {e}')

if __name__ == "__main__":
    if not pathlib.Path(f'{basedir}/{input_dir}').exists():
        print(f'{basedir}/{input_dir} not found, please copy test/data from yyjson')
        exit()
    os.makedirs(f'{basedir}/{output_dir}',exist_ok=True)
    # generate_pretest_file()
    run_json_test_d()
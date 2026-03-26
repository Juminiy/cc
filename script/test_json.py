import os 

import subprocess
import json

import pathlib

basedir="data/json"
test0="test_parsing"

c_binname="json_test.d"


def run_json_test_d():
    yfile,nfile,efile = [],[],[]
    subprocess.run(["make", c_binname])
    tot,cnt = 0,0
    for pfile in os.listdir(f'{basedir}/{test0}'):
        tot+=1
        should_res = pfile[0]
        pfile_full = f'{basedir}/{test0}/{pfile}'
        exec_res = subprocess.run([f"./{c_binname}", pfile_full], 
                    capture_output=True, text=True)
        if should_res != exec_res.stdout:
            if len(exec_res.stdout) == 0:
                efile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "exec_res": exec_res.stdout,
                })
                cnt+=1
            elif should_res=='y':
                yfile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "exec_res": exec_res.stdout,
                })
                cnt+=1
            elif should_res=='n':
                nfile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "exec_res": exec_res.stdout,
                })
                cnt+=1
            
    
    with open(f'{basedir}/y_{test0}.json', 'w') as fp:
        fp.write(json.dumps(yfile))
    with open(f'{basedir}/n_{test0}.json', 'w') as fp:
        fp.write(json.dumps(nfile))
    with open(f'{basedir}/e_{test0}.json', 'w') as fp:
        fp.write(json.dumps(efile))
    
    print(f'fail: {cnt}/{tot}, y: {len(yfile)}, n: {len(nfile)}, e: {len(efile)}')

def generate_pretest_file():
    numf = open(f'{basedir}/num_{test0}.txt','w')
    strf = open(f'{basedir}/str_{test0}.txt','w')
    for pfile in os.listdir(f'{basedir}/{test0}'):
        if pfile[:8]=='y_number' or pfile[:8]=='n_number' or \
            pfile[:8]=='y_string' or pfile[:8]=='n_string':
            try:
                bs = open(f'{basedir}/{test0}/{pfile}', 'r').readline()
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
                print(f'file: {pfile}')

if __name__ == "__main__":
    run_json_test_d()
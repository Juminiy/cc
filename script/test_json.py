import os 

import subprocess
import json

import pathlib

basedir="data/json/json"
test0="test_parsing"

c_binname="json_test.d"

if __name__ == "__main__":
    yfile,nfile,ifile = [],[],[]
    subprocess.run(["make", c_binname])
    tot,cnt = 0,0
    for pfile in os.listdir(f'{basedir}/{test0}'):
        tot+=1
        should_res = pfile[0]
        pfile_full = f'{basedir}/{test0}/{pfile}'
        exec_res = subprocess.run([f"./{c_binname}", pfile_full], 
                    capture_output=True, text=True)
        if should_res != exec_res.stdout:
            if should_res=='y':
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
            else:
                ifile.append({
                    "src_file": pfile_full,
                    "should_res": should_res,
                    "exec_res": exec_res.stdout,
                })
    
    with open(f'{basedir}/y_{test0}.json', 'w') as fp:
        fp.write(json.dumps(yfile))
    with open(f'{basedir}/n_{test0}.json', 'w') as fp:
        fp.write(json.dumps(nfile))
    with open(f'{basedir}/i_{test0}.json', 'w') as fp:
        fp.write(json.dumps(ifile))
    
    print(f'fail: {cnt}/{tot}, y: {len(yfile)}, n: {len(nfile)}')
import os 
from pathlib import Path

from typing import Dict,List,Optional,Any,Tuple
import json
import pandas as pd

if __name__ == "__main__":
    pids:List[int] = []
    ps_name:Dict[int,str] = {}
    ps_tree:Dict[int,List[int]] = {}
    for procdir in os.listdir("/proc"):
        procpath = Path(f'/proc/{procdir}')
        if procpath.is_dir() and \
            procdir.isdigit():
            proc_content = procpath.joinpath("status").open().readlines()
            proc_id = int(procdir)
            proc_parent_id = -1
            proc_name = ""
            for proc_line in proc_content:
                proc_line = proc_line.removesuffix("\n")
                if proc_line.startswith("Name"):
                    proc_name = proc_line.split(":")[1].lstrip()
                elif proc_line.startswith("PPid"):
                    proc_parent_id = int(proc_line.split(":")[1].lstrip())

            ps_name[proc_id] = proc_name
            if proc_parent_id>0:
                pids.append(proc_parent_id)
                if proc_parent_id not in ps_tree:
                    ps_tree[proc_parent_id] = []
                ps_tree[proc_parent_id].append(proc_id)
    
    with open('jyywiki/labs/data/proc.json','w') as proc_jfile:
        for ppid,chs in ps_tree.items():
            ps_tree[ppid] = sorted(chs)
        proc_jfile.write(json.dumps({
            "pids": pd.unique(pd.Series(sorted(pids))).tolist(),
            "ps_name": ps_name,
            "ps_tree": ps_tree,
        }))
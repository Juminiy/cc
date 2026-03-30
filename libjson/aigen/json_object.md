# JSON_OBJECT
1. 复用原有json.c中json_object_*函数签名, 底层结构保留原有`rb_tree *_tr`, 新增底层结构`barray _arr`
2. barray使用 libctr/ctr_barray.h中函数
3. 底层实现使用barray注意put,put_shallow_name,del,get无需检测是否存在
3. barray _arr 元素指针仍然是 `json_object_pair*`
4. 只允许在json_object.c生成代码,其他目录和文件只读勿改动
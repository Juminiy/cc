#include "json.h"
#include <stdio.h>

/*
 * JSON DECODE API
 */

// '\t'=11
// '\n'=12
// '\r'=15
// ' '=40
bool is_ws(char _ch) {
	return _ch==' '||_ch=='\t'||_ch=='\n'||_ch=='\r';
}

char next_token(ch_state *_stt) {
	for(int cur=_stt->rcur;cur<_stt->rsiz;cur++) {
		if(!is_ws(_stt->_raw[cur])){
			_stt->rcur = cur+1;
			return _stt->_raw[cur];
		}
	}
	_stt->rcur = _stt->rsiz;
	return EOF;
}

json_value* read_json_value(ch_state *_stt) {
	char ch = next_token(_stt);
	switch(ch) {
		case '"':
		return new_json_value_str(read_json_string(_stt));
		
		case '-':case'0':case'1':
		case'2':case'3':
		case'4':case'5':
		case'6':case'7':
		case'8':case'9':
		return new_json_value_num(read_json_num(_stt));

		case '{':
		return read_json_object(_stt);

		case '[':
		return read_json_array(_stt);

		case 't':case'f':case'n':
		return read_json_literal(ch,_stt);

		default:
		return NULL;
	}
}

// {}
// { "" : value}
// { "" : value, "" : value}
json_value* read_json_object(ch_state *_stt) {
	char ch = next_token(_stt);
	if (ch=='}'){
		return json_value_null_object;
	}
	json_object *obj = make_json_object();
	for(;;){
		if (ch!='"'){
			_stt->_err_msg = "json_object token '{' next is not '}' or '\"'";
			break;
		}
		char *obj_pair_name = read_json_string(_stt);
		if (obj_pair_name==NULL){
			_stt->_err_msg = "json_object pair name scanned invalid";
			break;
		}
		ch = next_token(_stt);
		if (ch !=':'){
			_stt->_err_msg = "json_object pair ':' colon not found";
			break;
		}
		json_value *obj_pair_val = read_json_value(_stt);
		if (obj_pair_val==NULL){
			break;
		}
		json_object_put(obj, obj_pair_name, obj_pair_val);
		ch = next_token(_stt);
		if (ch==',') {
			continue;
		} else if (ch=='}'){
			break;
		} else {
			_stt->_err_msg = "json_object pair next token is not ',' or '}'";
			break;
		}
		ch = next_token(_stt);
	}
	if (_stt->_err_msg){
		free_json_object(obj);
		return NULL;
	}
	return new_json_value_obj(obj);
}

// []
// [value]
// [value,value]
json_value* read_json_array(ch_state *_stt) {
	char ch = next_token(_stt);
	if (ch==']'){
		return json_value_null_array;
	}
	json_array *arr = make_json_array();

	for(;;){
		json_value *val = read_json_value(_stt);
		if(val==NULL){
			break;
		}
		json_array_append(arr,val);

		ch = next_token(_stt);
		if (ch==','){
			continue;
		} else if (ch==']'){
			break;
		} else {
			_stt->_err_msg = "json_array value next token is not ',' or ']'";
			break;
		}
	}

	if(_stt->_err_msg){
		free_json_array(arr);
		return NULL;
	}
	return new_json_value_arr(arr);
}

// ""
char* read_json_string(ch_state *_stt) {
	// find next \"
	char *endptr = strchr(_stt->_raw, '\"');
	size_t siz = endptr-_stt->_raw;
	char *_ss = __substr(_stt->_raw, _stt->rcur, siz);
	// todo: test and fix
	for(int i=0;i<__strlen(_ss);i++){
		if((_ss[i]>=0&&_ss[i]<=31)||(_ss[i]=='\\'||_ss[i]=='\"')){
			free(_ss);
			_ss = NULL;
			break;
		}
	}
	return _ss;
}

// todo:
// int64 border
int64_t read_json_int(ch_state *_stt) {
	return 0;
}

// todo:
// ieee754
double read_json_num(ch_state *_stt) {
	int cur = _stt->rcur-1, siz = 0;
	for(;cur<_stt->rsiz;cur++){
		if(is_ws(_stt->_raw[cur])){
			break;
		}
		siz++;
	}
	char *_ss = __substr(_stt->_raw, cur, siz);
	double val;
	sscanf(_ss, "%lf", &val);
	return val;
}

json_value* read_json_literal(char prv,ch_state *_stt) {
	switch (prv){
		case 't':
		{
			if(__strcmp("rue",__substr(_stt->_raw,_stt->rcur,3))==0) {
				return json_value_true;
			}
		}
		break;

		case 'f':
		{
			if(__strcmp("alse",__substr(_stt->_raw,_stt->rcur,4))==0) {
				return json_value_false;
			}
		}
		break;

		case 'n':
		{
			if(__strcmp("ull",__substr(_stt->_raw,_stt->rcur,3))==0) {
				return json_value_null;
			}
		}
		break;

		default:
		return NULL;
	}
	return NULL;
}

json_value* json_parse(const char *_str) {
	ch_state _stt = {
		.rcur=0,
		.rsiz=__strlen(_str),
		._raw=_str,
		._err_msg=NULL,
	};
	return read_json_value(&_stt);
}
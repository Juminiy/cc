#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * JSON DECODE API
 */
typedef struct ch_state {
	char    *_raw;
	int rcur,rsiz;
	// char *_clean;
	// int ccur,csiz;
	char *_err_msg;
} ch_state;

char next_token(ch_state *_stt);
json_value* read_json_value(ch_state *_stt);
json_value* read_json_object(ch_state *_stt);
json_value* read_json_array(ch_state *_stt);
char* read_json_string(ch_state *_stt);
json_value* read_json_num(ch_state *_stt);
json_value* read_json_literal(char prv,ch_state *_stt);

char next_token(ch_state *_stt) {
	for(int cur=_stt->rcur;cur<_stt->rsiz;cur++) {
		if(!isspace(_stt->_raw[cur])){
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
		return new_json_value_str_shallow(read_json_string(_stt));
		
		case '-':case'0':case'1':
		case'2':case'3':
		case'4':case'5':
		case'6':case'7':
		case'8':case'9':
		return read_json_num(_stt);

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
			_stt->_err_msg = "json_object pair name string scanned invalid";
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
		json_object_put_shallow_name(obj, obj_pair_name, obj_pair_val);
		ch = next_token(_stt);
		if (ch==',') {
			
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
	_stt->rcur--;
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

// name\"
char* read_json_string(ch_state *_stt) {
	// find next \"
	char *curptr = _stt->_raw+_stt->rcur;
	char *endptr = strchr(curptr, '\"');
	size_t siz = endptr-curptr;
	char *_ss = __substr(curptr, 0, siz);
	// todo: test and fix
	for(int i=0;i<__strlen(_ss);i++){
		if((_ss[i]>=0&&_ss[i]<=31)||(_ss[i]=='\\'||_ss[i]=='\"')){
			free(_ss);
			_ss = NULL;
			_stt->_err_msg = "json_string include control character 0~31 or '\' or '\"'";
			break;
		}
	}
	_stt->rcur += (siz+1);
	return _ss;
}

// todo:
// float/double: ieee754
// int64 border
json_value* read_json_num(ch_state *_stt) {
	int cur = _stt->rcur-1, siz = 0;
	bool only_digit = true;
	for(;cur<_stt->rsiz;cur++){
		if(is_num(_stt->_raw[cur])){
			siz++;
		} else {
			break;
		}
		if(!isdigit(_stt->_raw[cur])){
			only_digit=false;
		}
	}
	if (siz>JSON_NUM_MAX_SIZE){
		_stt->_err_msg = "json_number size overflow";
		return NULL;
	}
	
	char *_ss = __substr(_stt->_raw, _stt->rcur-1, siz);
	_stt->rcur = cur;
	double f64;
	sscanf(_ss, "%lf", &f64);

	json_value *val = NULL;
	if(only_digit){
		if(__strcmp(_ss,"0")==0) {
			val = new_json_value_int(0);
		} else if (siz>1&&_ss[0]=='0'){
			_stt->_err_msg = "json_number integer include leading zero";
		} else if (f64>=JSON_INT_MIN&&f64<=JSON_INT_MAX){
			int64_t i64;
			sscanf(_ss, "%ld", &i64);
			val = new_json_value_int(i64);
		} else if (f64>0.0&&f64<=JSON_UINT_MAX){
			uint64_t u64;
			sscanf(_ss, "%lu", &u64);
			val = new_json_value_uint(u64);
		}
	}
	free(_ss);
	if(val==NULL&&_stt->_err_msg==NULL){
		val = new_json_value_num(f64);
	}
	return val;
}

json_value* read_json_literal(char prv,ch_state *_stt) {
	switch (prv){
		case 't':
		{
			char buf[4];
			if(__strcmp("rue",__substr_2(buf,_stt->_raw,_stt->rcur,3))==0) {
				_stt->rcur+=3;
				return json_value_true;
			}
		}
		break;

		case 'f':
		{
			char buf[5];
			if(__strcmp("alse",__substr_2(buf,_stt->_raw,_stt->rcur,4))==0) {
				_stt->rcur+=4;
				return json_value_false;
			}
		}
		break;

		case 'n':
		{	
			char buf[4];
			if(__strcmp("ull",__substr_2(buf,_stt->_raw,_stt->rcur,3))==0) {
				_stt->rcur+=3;
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
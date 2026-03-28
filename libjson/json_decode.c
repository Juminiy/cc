#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CH_STATE_MAX_DEEPTH 100000
/*
 * JSON DECODE API
 */
typedef struct ch_state {
	char    *_raw;
	int rcur,rsiz;
	// char *_clean;
	// int ccur,csiz;
	char *_err_msg;
	int _d0; // [
	int _d1; // {
} ch_state;

char next_token(ch_state *_stt);
json_value* read_json_value(ch_state *_stt);
json_value* read_json_object(ch_state *_stt);
json_value* read_json_array(ch_state *_stt);
char* read_json_string(ch_state *_stt);
json_value* read_json_number(ch_state *_stt);
json_value* read_json_literal(char prv,ch_state *_stt);

char next_token(ch_state *_stt) {
	for(int cur=_stt->rcur;cur<_stt->rsiz;cur++) {
		if(!JSON_SPACE(_stt->_raw[cur])){
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
		char *_str = read_json_string(_stt);
		if(_str==NULL){
			return NULL;
		}
		return new_json_value_str_shallow(_str);
		
		case '-':case'0':case'1':
		case'2':case'3':
		case'4':case'5':
		case'6':case'7':
		case'8':case'9':
		return read_json_number(_stt);

		case '{':
		return read_json_object(_stt);

		case '[':
		return read_json_array(_stt);

		case 't':case'f':case'n':
		return read_json_literal(ch,_stt);

		default:
		_stt->_err_msg = "read illegal token, could not know what to do next";
		return NULL;
	}
}

// {}
// { "" : value}
// { "" : value, "" : value}
json_value* read_json_object(ch_state *_stt) {
	_stt->_d0++;
	if(_stt->_d0>=CH_STATE_MAX_DEEPTH){
		_stt->_err_msg = "'{' exceeded max depth";
		return NULL;
	}
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
		if(json_object_get(obj,obj_pair_name)){
			_stt->_err_msg = "json_object name duplicated";
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
	_stt->_d0--;
	return new_json_value_obj(obj);
}

// []
// [value]
// [value,value]
json_value* read_json_array(ch_state *_stt) {
	_stt->_d1++;
	if(_stt->_d1>=CH_STATE_MAX_DEEPTH){
		_stt->_err_msg = "'[' exceeded max depth";
		return NULL;
	}
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
	_stt->_d1--;
	return new_json_value_arr(arr);
}

// name\"
char* read_json_string(ch_state *_stt) {
	json_value _val;
	roSBuf _sbuf = {._p=_stt->_raw+_stt->rcur-1,._siz=_stt->rsiz-_stt->rcur+1,._init_type=ROSBUF_INIT_SHALLOW_COPY};
	int read_sz = parse_json_string_v2(_sbuf,&_val);
	if(read_sz<0){ // allow 0
		_stt->_err_msg = "json_string read error";
		return NULL;
	}

	_stt->rcur+=(read_sz+1);
	return (char*)(_val.val.ptr);
}

// float/double: ieee754
// int64/uint64: border
json_value* read_json_number(ch_state *_stt) {
	json_value _val;
	roSBuf _sbuf = {._p=_stt->_raw+_stt->rcur-1,._siz=_stt->rsiz-_stt->rcur+1,._init_type=ROSBUF_INIT_SHALLOW_COPY};
	int read_sz = parse_json_number_v2(_sbuf,&_val);
	if(read_sz<=0){
		_stt->_err_msg = "json_number read error";
		return NULL;
	}

	_stt->rcur += (read_sz-1);
	return new_json_value_copy(&_val);
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
	}
	_stt->_err_msg = "json_literal not in true,false,null";
	return NULL;
}

json_value* json_parse(const char *_str) {
	ch_state _stt = {
		.rcur=0,
		.rsiz=__strlen(_str),
		._raw=_str,
		._err_msg=NULL,
		._d0=0,
		._d1=0,
	};
	json_value *_val = read_json_value(&_stt);
	if(_val==NULL){
		return NULL;
	}
	
	char ch = next_token(&_stt);
	if(ch!=EOF){
		free_json_value(_val);
		return NULL;
	}
	return _val;
}

json_value* json_nparse(const char *_str, size_t _n) {
	ch_state _stt = {
		.rcur=0,
		.rsiz=_n,
		._raw=_str,
		._err_msg=NULL,
		._d0=0,
		._d1=0,
	};
	json_value *_val = read_json_value(&_stt);
	if(_stt._err_msg){
		fprintf(stderr, "[JSON parse_error]: %s\n", _stt._err_msg);
	}
	if(_val==NULL){
		return NULL;
	}
	
	char ch = next_token(&_stt);
	if(ch!=EOF){
		free_json_value(_val);
		return NULL;
	}
	return _val;
}
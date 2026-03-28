#include "json.h"
#include "../libctr/ctr_sbuf.h"

#include <ctype.h>

#define __can_endof_value(_ch) \
	(JSON_SPACE(_ch)||_ch==','||_ch==']'||_ch=='}') // json file in any position, character can not be '\0'

#define __can_endof_number(_ch) \
	(__can_endof_value(_ch))

#define __sptr_null(__s) (!(__s)||*(__s)=='\0')

// 1.7976931348623157 × 10³⁰⁸
#define IEEE754_INT_MAXVSIZ 308
// 2.2250738585072014 × 10⁻³⁰⁸
// 4.9406564584124654 × 10⁻³²⁴
#define IEEE754_EXP_MAXVAL 324

int parse_json_number(char *_s, json_value *_val) {
	while(_s&&JSON_SPACE(*_s)){
		_s++;
	}

	char *_ssave = _s;

	bool neg = false;
	if(_s&&*_s=='-'){
		_s++;
		neg = true;
	}

	if(!_s||!isdigit(*_s)){
		if(neg){
			// json_number char '-' next char is not [0-9]
		} else {
			// json_number first char is not [0-9]
		}
		return -1;
	}

	// integer section 
	size_t _isz = 0;
	if(*_s=='0'){
		_s++;
		_isz++;
		if(!(__can_endof_number(*_s)||*_s=='.'||*_s=='e'||*_s=='E')){
			// json_number char '0' next char is not space or '.' or 'e' or 'E'
			return -1;
		} 
	} else {
		while(_s&&isdigit(*_s)){
			_isz++;
			_s++;
			if(_isz>IEEE754_INT_MAXVSIZ){
				// json_number before '.' overflow IEEE754 double floating point
				return -1;
			}
		}
	}
	// if(_isz==0){
	// 	// json_number integer section not found
	// 	return -1;
	// }

	// fraction section
	size_t _fsz = 0;
	if(_s&&*_s=='.'){
		_s++;
		while(_s&&isdigit(*_s)){
			_s++;
			_fsz++;
		}
		if(_fsz==0){
			// json_number fraction char '.' next not [0-9]
			return -1;
		}
	}

	// exponent section
	size_t _esz = 0;
	bool _eneg = false;
	if(_s&&(*_s=='e'||*_s=='E')) {
		_s++;
		if(_s&&*_s=='-'){
			_s++;
			_eneg = true;
		}else if(_s&&*_s=='+'){
			_s++;
			_eneg = false;
		}
		int expv=0;
		while(_s&&isdigit(*_s)){
			expv = expv*10+(*_s-'0');
			_s++;
			_esz++;
			if (expv>IEEE754_EXP_MAXVAL) {
				// json_number exponent overflow IEEE754 double floating point
				return -1;
			}
		}
		if(_esz==0){
			// json_number exponent char 'e'/'E' next not [0-9]
			return -1;
		}
	}

	if(!(!_s||__can_endof_number(*_s))){
		// json_number next char invalid
		return -1;
	}

	// assert integer
	double f64;
	sscanf(_ssave, "%lf", &f64);
	_val->typ = JSON_NUMBER;
	_val->val.f64 = f64;
	if(_fsz==0&&_esz==0){
		if(f64>=JSON_INT_MIN&&f64<=JSON_INT_MAX){
			int64_t i64;
			sscanf(_ssave, "%ld", &i64);
			_val->typ = JSON_INTEGER;
			_val->val.i64 = i64;
		} else if(f64>0&&f64<=JSON_UINT_MAX){
			uint64_t u64;
			sscanf(_ssave, "%lu", &u64);
			_val->typ = JSON_INTEGER_UINT;
			_val->val.u64 = u64;
		}
	}
	
	return _s-_ssave;
}

int parse_json_string(char *_s, json_value *_val) {
	while(_s&&JSON_SPACE(*_s)){
		_s++;
	}

	if(!_s||*_s!='"'){
		return -1;
	}
	_s++;

	size_t siz = 0;
	for(size_t _i=0;;){
		if(_s[_i]=='"'){
			siz=_i;
			break;
		} else if(_s[_i]=='\\'){ // start escape
			_i++;
			if(__sptr_null(_s+_i)){
				return -1;
			}
			if(_s[_i]=='"'||_s[_i]=='\\'||_s[_i]=='/'||
				_s[_i]=='b'||_s[_i]=='f'||_s[_i]=='n'||
				_s[_i]=='r'||_s[_i]=='t'){
				_i++;
			} else if(_s[_i]=='u'){
				_i++;
				if(!__sptr_null(_s+_i+4)&&
					isxdigit(_s[_i+1])&&
					isxdigit(_s[_i+2])&&
					isxdigit(_s[_i+3])&&
					isxdigit(_s[_i+0])){
					_i+=4;
				} else {
					return -1;
				}
			} else {
				return -1;
			}
		} else if (_s[_i]==127) { // seem as escaped
			_i++;
		} else if(iscntrl(_s[_i])){
			return -1;
			// _i++;
		} else {
			_i++;
		}
	}

	_val->typ = JSON_STRING;
	_val->val.ptr = __substr(_s,0,siz);

	return siz;

}

// _p=123\0, _p_print=123, _siz=4
int parse_json_number_v2(roSBuf _buf, json_value *_val) {
	char *_s = _buf._p;
	#define _sok (_s-_buf._p<_buf._siz)
	while(_sok&&JSON_SPACE(*_s)){
		_s++;
	}

	char *_ssave = _s; // skip whitespace

	bool neg = false;
	if(_sok&&*_s=='-'){
		_s++;
		neg = true;
	}

	if(!_sok||!isdigit(*_s)){
		if(neg){
			// json_number char '-' next char is not [0-9]
		} else {
			// json_number first char is not [0-9]
		}
		return -1;
	}

	// integer section 
	size_t _isz = 0;
	if(*_s=='0'){
		_s++;
		_isz++;
		if(!(__can_endof_number(*_s)||*_s=='.'||*_s=='e'||*_s=='E')){
			// json_number char '0' next char is not space or ',' or ']' or '}' or '.' or 'e' or 'E'
			return -1;
		} 
	} else {
		while(_sok&&isdigit(*_s)){
			_s++; 
			_isz++;
			if(_isz>IEEE754_INT_MAXVSIZ){
				// json_number before '.' overflow IEEE754 double floating point
				return -1;
			}
		}
	}
	if(_isz==0){
		// json_number integer section not found
		return -1;
	}

	// fraction section
	size_t _fsz = 0;
	if(_sok&&*_s=='.'){
		_s++;
		while(_sok&&isdigit(*_s)){
			_s++;
			_fsz++;
		}
		if(_fsz==0){
			// json_number fraction char '.' next not [0-9]
			return -1;
		}
	}

	// exponent section
	size_t _esz = 0;
	bool _eneg = false;
	if(_sok&&(*_s=='e'||*_s=='E')) {
		_s++;
		if(_sok&&*_s=='-'){
			_s++;
			_eneg = true;
		}else if(_sok&&*_s=='+'){
			_s++;
			_eneg = false;
		}
		int expv=0;
		while(_sok&&isdigit(*_s)){
			expv = expv*10+(*_s-'0');
			_s++;
			_esz++;
			if (expv>IEEE754_EXP_MAXVAL) {
				// json_number exponent overflow IEEE754 double floating point
				return -1;
			}
		}
		if(_esz==0){
			// json_number exponent char 'e'/'E' next not [0-9]
			return -1;
		}
	}

	if(!(!_sok||__can_endof_number(*_s))){
		// json_number next char invalid
		return -1;
	}

	// assert integer
	double f64;
	sscanf(_ssave, "%lf", &f64);
	_val->typ = JSON_NUMBER;
	_val->val.f64 = f64;
	if(_fsz==0&&_esz==0){ // integer
		if(f64>=JSON_INT_MIN&&f64<=JSON_INT_MAX){ // int64
			int64_t i64;
			sscanf(_ssave, "%ld", &i64);
			_val->typ = JSON_INTEGER;
			_val->val.i64 = i64;
		} else if(f64>0&&f64<=JSON_UINT_MAX){ // uint64
			uint64_t u64;
			sscanf(_ssave, "%lu", &u64);
			_val->typ = JSON_INTEGER_UINT;
			_val->val.u64 = u64;
		}
	}
	
	return _s-_ssave;
}

// _p="xxx", _siz=3
int parse_json_string_v2(roSBuf _buf, json_value *_val) {
	char *_s = _buf._p;
	while((_s-_buf._p<_buf._siz)&&JSON_SPACE(*_s)){
		_s++;
	}

	if(!(_s-_buf._p<_buf._siz)||*_s!='"'){
		return -1;
	}
	_s++;

	#define _sok2 (_s+_i-_buf._p<_buf._siz)
	size_t _str_siz = 0;
	for(size_t _i=0;_sok2;){
		if(_s[_i]=='"'){ // end of string
			_str_siz=_i;
			break;
		} else if(_s[_i]=='\\'){ // start escape
			_i++;
			if(!_sok2){
				return -1;
			}
			if(_s[_i]=='"'||_s[_i]=='\\'||_s[_i]=='/'||
				_s[_i]=='b'||_s[_i]=='f'||_s[_i]=='n'||
				_s[_i]=='r'||_s[_i]=='t'){
				_i++;
			} else if(_s[_i]=='u'){
				_i++;
				for(int _tmpii=0;_tmpii<4;_tmpii++){
					if(_sok2&&isxdigit(_s[_i])){
						_i++;
					} else {
						return -1;
					}
				}
			} else {
				return -1;
			}
		} else if (_s[_i]==127) { // noescaped:DEL or 127 or \177
			_i++;
		} else if(iscntrl(_s[_i])){ // control char
			return -1;
		} else { // normal char
			_i++;
		}
	}

	_val->typ = JSON_STRING;
	_val->val.ptr = __substr(_s,0,_str_siz);

	return _str_siz;

}
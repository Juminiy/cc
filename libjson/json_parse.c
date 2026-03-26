#include "json.h"
#include <ctype.h>

bool __can_endof_value(char _ch) {
	return isspace(_ch)||_ch==','||_ch==']'||_ch=='}';
}

bool __can_endof_number(char _ch) {
	return __can_endof_value(_ch);
}

// 1.7976931348623157 × 10³⁰⁸
#define IEEE754_INT_MAXVSIZ 308
// 2.2250738585072014 × 10⁻³⁰⁸
// 4.9406564584124654 × 10⁻³²⁴
#define IEEE754_INT_MINVSIZ 324


size_t parse_json_number(char *_s, json_value *_val) {
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
		}
		while(_s&&isdigit(*_s)){
			_s++;
			_esz++;
			if (_esz>IEEE754_INT_MINVSIZ) {
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
	if(f64>=JSON_INT_MIN&&f64<=JSON_INT_MAX){
		int64_t i64;
		sscanf(_ssave, "%lld", &i64);
		_val->typ = JSON_INTEGER;
		_val->val.i64 = i64;
	} else if(f64>0&&f64<=JSON_UINT_MAX){
		uint64_t u64;
		sscanf(_ssave, "%llu", &u64);
		_val->typ = JSON_INTEGER_UINT;
		_val->val.u64 = u64;
	}

	return _s-_ssave;
}

size_t parse_json_string(char *_s, json_value *_val) {
	if(!_s||*_s!='\"'){
		return -1;
	}
	_s++;
	char *_endp = strchr(_s,'\"');
	if(_endp==NULL){
		return -1;
	}
	size_t siz = _endp-_s;

	for(size_t _i=0;_i<siz;_i++){
		
		if(_s[_i]=='\"'||_s[_i]=='\\'||iscntrl(_s[_i])){
			return -1;
		}

	}

	_val->typ = JSON_STRING;
	_val->val.ptr = __substr(_s,0,siz);

	return _s;

}
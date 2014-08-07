#ifndef JSONC_H
#define JSONC_H

struct json_object;

typedef union {
	char * string;
	float number;
	json_object * object;
	json_value * array;
	bool bit;
} json_value;

// typedef struct {
// 	char * key;
// 	json_value value;
// } json_pair;

// typedef struct {
// 	json_pair * members;
// } json_object;

struct json_object{
	char ** keys;
	unsigned int size;
	json_value * ;
	struct json_object *
};

#endif /* JSONC_H */

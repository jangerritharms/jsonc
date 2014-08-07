#ifndef JSONC_H
#define JSONC_H

#define LOG(x) fprintf(stdout, x);
#define ERR(x) fprintf(stderr, x), exit(EXIT_FAILURE);

struct json_object_;

typedef enum {false, true} bool;
typedef enum {STRING, NUMBER, OBJECT, ARRAY, BOOL} json_type;

union json_value {
	char * string;
	float number;
	struct json_object_ * object;
	union json_value * array;
	bool bit;
};
typedef union json_value json_value;

typedef struct {
	json_type type;
	char * key;
	json_value value;
} json_pair;

struct json_object_{
	json_pair member;
	struct json_object_ * next;
};
typedef struct json_object_ json_object;

typedef struct {
	json_object * root;
	unsigned int size;
} json_struct;

// struct json_object{
// 	char ** keys;
// 	unsigned int size;
// 	json_value * ;
// 	struct json_object *
// };

void init_json_struct(json_struct*);

typedef enum {SKIP, START_OBJECT, END_OBJECT, START_KEY, END_KEY, START_STRING_VALUE, END_STRING_VALUE, START_NUMBER_VALUE, END_NUMBER_VALUE, START_ARRAY_VALUE, END_ARRAY_VALUE} parse_state;

void parse_json(json_struct *, const char *);

#endif /* JSONC_H */

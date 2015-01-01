#ifndef JSONC_H
#define JSONC_H

#include <stdio.h>

typedef enum{false, true} bool;

#define LOG(...) fprintf(stdout, __VA_ARGS__);
#define ERR(...) fprintf(stderr, __VA_ARGS__), exit(EXIT_FAILURE);

typedef enum {JSON_BOOL, JSON_NONE, JSON_NUMBER, JSON_OBJECT, JSON_ARRAY, JSON_STRING} json_type;

struct json_object{
    struct json_object *next;
    char *key;
    size_t size;
    json_type type;
    char value[1];
};
typedef struct json_object json_object;

struct json_array{
    struct json_array* next;
    json_type type;
    size_t size;
    char value[1];
};
typedef struct json_array json_array;

json_object *json_parse_file(FILE *);
//json_object* json_find_node(const json_struct*, const char *);
//void json_get_object(const json_struct *js, const char* key, json_object *value);
//void json_parse_object(char *string, json_object *obj);

typedef enum {INITIAL, SKIP, OPEN_KEY, CLOSED_KEY, OPEN_STRING} parse_state;
typedef enum {A_INITIAL, A_SKIP, A_OPEN_STRING} array_parse_state;
#endif /* JSONC_H */

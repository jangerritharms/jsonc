#ifndef JSONC_H
#define JSONC_H

#define LOG(...) fprintf(stdout, __VA_ARGS__);
#define ERR(...) fprintf(stderr, __VA_ARGS__), exit(EXIT_FAILURE);

typedef enum {JSON_BOOL, JSON_NONE, JSON_NUMBER, JSON_OBJECT, JSON_ARRAY, JSON_STRING} json_type;

struct json_object;

struct json_node {
    int start;
    int length;
    int start_key;
    int key_length;
    json_type type;
    struct json_node *next;
};
typedef struct json_node json_node;

struct json_struct {
    char *string;
    int nelements;
    struct json_object *root;
};
typedef struct json_struct json_struct;

struct json_object{
    int start;
    int length;
    json_node *first;
    json_struct *file;
};
typedef struct json_object json_object;

void json_parse_file(const char *, json_struct*);
json_node* json_find_node(const json_struct*, const char *);
void json_get_object(const json_struct *js, const char* key, json_object *value);
void json_parse_object(char *string, json_object *obj);

typedef enum {INITIAL, SKIP, OPEN_KEY, CLOSED_KEY, OPEN_VALUE} parse_state;
#endif /* JSONC_H */

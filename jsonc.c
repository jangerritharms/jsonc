#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "jsonc.h"

void init_null(void*);

json_array *json_parse_array(FILE*);

json_object *json_parse_file(FILE *fp){
    json_object *root = NULL;
    json_object *current = NULL;
    json_object *next = NULL;
    char cursor;
    char *key;
    parse_state p = INITIAL;
    long temp;
    json_type type;
    int length;
    int size;

    if (!fp) ERR("Error opening file");

    while(true){
        cursor = fgetc(fp);
        switch (p){
            case INITIAL:
                if (cursor == '{'){
                    p = SKIP;
                }
                break;
            case SKIP:
                if (cursor == ','){
                    if(!root){
                        root = current = next;
                    }
                    else{
                        current->next = next;
                        current = current->next;
                    }
                }
                if (cursor == '"'){
                    temp = ftell(fp);            
                    p = OPEN_KEY;
                }
                else if (cursor == '}'){
                    if (!root)
                        root = current = next;
                    else{
                        current->next = next;
                    }
                    return root;
                }
                break;
            case OPEN_KEY:
                if (cursor == '"'){
                    length = ftell(fp)-temp;
                    key = malloc(length * sizeof(char));
                    fseek(fp,temp, SEEK_SET);
                    fread(key, 1, length-1, fp);
                    *(key+length) = '\0';
                    fgetc(fp);
                    p = CLOSED_KEY;
                }
                break;
            case CLOSED_KEY:
                if(cursor == '"'){
                    temp = ftell(fp);
                    type = JSON_STRING;
                    p = OPEN_STRING;
                }
                else if((cursor >= '0' && cursor <= '9') || cursor == '-'){
                    ungetc(cursor, fp);
                    size = sizeof(json_object)-1+sizeof(int);
                    next = malloc(size);
                    next->size = size;
                    fscanf(fp, "%d", (int*)&(next->value));
                    next->key = key;
                    next->type = JSON_NUMBER;
                    p = SKIP;
                }
                else if (cursor == '{'){
                    ungetc(cursor, fp);
                    json_object *obj_temp =  json_parse_file(fp);
                    size = sizeof(json_object)-1+obj_temp->size;
                    next = malloc(size);
                    next->size = size;
                    memmove(&(next->value), obj_temp, obj_temp->size);
                    next->key = key;
                    next->type = JSON_OBJECT;
                    p = SKIP;
                }
                else if(cursor == 'f'){
                    size = sizeof(json_object)-1+sizeof(bool);
                    next = malloc(size);
                    next->size = size;
                    bool *bool_temp = (bool*)&(next->value);
                    *bool_temp = false;
                    next->key = key;
                    next->type = JSON_OBJECT;
                    p = SKIP;
                }
                else if(cursor == 't'){
                    size = sizeof(json_object)-1+sizeof(bool);
                    next = malloc(size);
                    next->size = size;
                    bool *bool_temp = (bool*)&(next->value);
                    *bool_temp = true;
                    next->key = key;
                    next->type = JSON_OBJECT;
                    p = SKIP;
                }
                else if(cursor == 'n'){
                    size = sizeof(json_object)-1+sizeof(void*);
                    next = malloc(size);
                    next->size = size;
                    next->key = key;
                    next->type = JSON_NONE;
                    p = SKIP;
                }
                else if (cursor == '['){
                    ungetc(cursor, fp);
                    json_array* arr_temp = json_parse_array(fp);
                    size = sizeof(json_object)-1+arr_temp->size;
                    next = malloc(size); 
                    next->size = size;
                    memmove(&(next->value), arr_temp, arr_temp->size);
                    next->key = key;
                    next->type = JSON_ARRAY;
                    p = SKIP;
                }
                break;
            case OPEN_STRING:
                if (cursor == '"'){
                    length = ftell(fp)-temp;
                    size  = sizeof(json_object)-1+length*sizeof(char);
                    next = malloc(size);
                    next->size = size;
                    next->key = key;
                    next->type = type;
                    fseek(fp,temp, SEEK_SET);
                    fread(next->value, 1, length-1, fp);fgetc(fp);
                    *(next->value+length) = '\0';
                    p = SKIP;
                }
                break;
        }
    }
    return root;
}

json_array *json_parse_array(FILE *fp){
    json_array *root = NULL;
    json_array *current = NULL;
    json_array *next = NULL;
    char cursor;
    array_parse_state p = A_INITIAL;
    json_type type;
    long temp;
    int length;
    int size;

    if (!fp) ERR("Error opening file");

    while(true){
        cursor = fgetc(fp);
        switch (p){
            case A_INITIAL:
                if (cursor == '['){
                    p = A_SKIP;
                }
                break;
            case A_SKIP:
                if (cursor == ','){
                    if(!root){
                        root = current = next;
                    }
                    else{
                        current->next = next;
                        current = current->next;
                    }
                }
                else if(cursor == '"'){
                    temp = ftell(fp);
                    type = JSON_STRING;
                    p = A_OPEN_STRING;
                }
                else if((cursor >= '0' && cursor <= '9') || cursor == '-'){
                    ungetc(cursor, fp);
                    size = sizeof(json_array)-1+sizeof(int);
                    next = malloc(size);
                    next->size = size;
                    fscanf(fp, "%d", (int*)&(next->value));
                    next->type = JSON_NUMBER;
                    p = A_SKIP;
                }
                else if (cursor == '{'){
                    ungetc(cursor, fp);
                    json_object *obj_temp =  json_parse_file(fp);
                    size = sizeof(json_array)-1+obj_temp->size;
                    next = malloc(size);
                    next->size = size;
                    memmove(&(next->value), obj_temp, obj_temp->size);
                    next->type = JSON_OBJECT;
                    p = A_SKIP;
                }
                else if(cursor == 'f'){
                    size = sizeof(json_array)-1+sizeof(bool);
                    next = malloc(size);
                    next->size = size;
                    bool *bool_temp = (bool*)&(next->value);
                    *bool_temp = false;
                    next->type = JSON_OBJECT;
                    p = A_SKIP;

                }
                else if(cursor == 't'){
                    size = sizeof(json_array)-1+sizeof(bool);
                    next = malloc(size);
                    next->size = size;
                    bool *bool_temp = (bool*)&(next->value);
                    *bool_temp = true;
                    next->type = JSON_OBJECT;
                    p = A_SKIP;

                }
                else if(cursor == 'n'){
                    size = sizeof(json_array)-1+sizeof(void*);
                    next = malloc(size);
                    next->size = size;
                    next->type = JSON_NONE;
                    p = A_SKIP;
                }
                else if (cursor == '['){
                    ungetc(cursor, fp);
                    json_array* arr_temp = json_parse_array(fp);
                    size = sizeof(json_array)-1+arr_temp->size;
                    next = malloc(size); 
                    next->size = size;
                    memmove(&(next->value), arr_temp, arr_temp->size);
                    next->type = JSON_ARRAY;
                    p = A_SKIP;
                }
                else if (cursor == ']'){
                    if (!root)
                        root = current = next;
                    else{
                        current->next = next;
                    }
                    return root;
                }
                break;
            case A_OPEN_STRING:
                if (cursor == '"'){
                    length = ftell(fp)-temp;
                    size = sizeof(json_array)-1+length*sizeof(char);
                    next = malloc(size);
                    next->size = size;
                    next->type = type;
                    fseek(fp,temp, SEEK_SET);
                    fread(next->value, 1, length-1, fp);fgetc(fp);
                    *(next->value+length) = '\0';
                    p = A_SKIP;
                }
                break;
        }
    }
    return root;
}

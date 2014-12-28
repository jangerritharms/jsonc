#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsonc.h"

typedef enum{false, true} bool;

char* get_string_from_file(const char * file_path){
    FILE *fp;
    long lSize;
    char *string;

    fp = fopen(file_path, "r");
    if (!fp) ERR("Error opening file");

    fseek (fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    string = calloc(1, lSize+1);
    if (!string) fclose(fp), ERR("Memory allocation failed");
    if (1!=fread(string, lSize, 1, fp))
        fclose(fp), ERR("Reading of file failed");
    fclose(fp);

    return string;
}

void json_parse_object(char *string, json_object *obj){
    char * cursor;
    parse_state p = INITIAL;
    long ind=obj->start;
    json_node *current = NULL;

    for (cursor = string; *cursor != '\0'; cursor++,ind++){
        putc(*cursor, stdout);
        switch (p){
            case INITIAL:
                if (*cursor == '{'){
                    p = SKIP;
                }
                break;
            case SKIP:
                if (*cursor == '"'){
                    if (!current){
                        obj->first = calloc(1, sizeof(json_node));
                        current = obj->first;
                    }
                    else {
                        current->next = calloc(1, sizeof(json_node));
                        current = current->next;
                    }
                    current->start_key = ind+1;
                    p = OPEN_KEY;
                }
                else if (*cursor == '}'){
                    obj->length = ind - obj->start+1;
                    return;
                }
                break;
            case OPEN_KEY:
                if (*cursor == '"'){
                    current->key_length = ind - current->start_key;
                    p = CLOSED_KEY;
                }
                break;
            case CLOSED_KEY:
                if(*cursor == '"'){
                    current->start = ind+1;
                    current->type = JSON_STRING;
                    p = OPEN_VALUE;
                }
                else if((*cursor >= '0' && *cursor <= '9') || *cursor == '-'){
                    current->start = ind;
                    current->type = JSON_NUMBER;
                    p = OPEN_VALUE;
                }
                else if(*cursor == 'f' || *cursor == 't'){
                    current->start = ind;
                    current->type = JSON_BOOL;
                    p = OPEN_VALUE;
                }
                else if(*cursor == 'n'){
                    current->start = ind;
                    current->type = JSON_NONE;
                    p = OPEN_VALUE;
                }
                else if(*cursor == '['){
                    current->start = ind;
                    current->type = JSON_ARRAY;
                    p = OPEN_VALUE;
                }
                else if (*cursor == '{'){
                    current->start = ind;
                    current->type = JSON_OBJECT;
                    p = OPEN_VALUE;
                }
                break;
            case OPEN_VALUE:
                if (current->type == JSON_BOOL || current->type == JSON_NONE
                        || current->type == JSON_NUMBER){
                    if (*cursor == ' ' || *cursor == '\t' || *cursor == '\n'){
                        current->length = ind - current->start ;
                        p = SKIP;
                    }
                }
                else if (current->type == JSON_ARRAY){
                    if (*cursor == ']'){
                        current->length = ind - current->start +1;
                        p = SKIP;
                    }
                }
                else if (current->type == JSON_OBJECT){
                    if (*cursor == '}'){
                        current->length = ind - current->start+1;
                        p = SKIP;
                    }
                }
                else {
                    if (*cursor == '"'){
                        current->length = ind - current->start ;
                        p = SKIP;
                    }
                }
                break;
        }
    }
}

json_node *json_find_node(const json_struct *js, const char* key){
    json_node *j = js->root->first;
    while(strncmp(key, js->string+j->start_key, j->key_length) && j->next)
        j = j->next;

    if (strncmp(key, js->string+j->start_key, j->key_length)) ERR("Could not find key %s", key);

    return j;
}

void json_get_int(const json_struct *js, const char* key, int *value){
    json_node *j = json_find_node(js, key);
    if (j->type != JSON_NUMBER) ERR("Object %s is not of type NUMBER", key);

    *value = atol(js->string+j->start_key);
}

void json_get_object(const json_struct *js, const char* key, json_object *value){
    json_node *j = json_find_node(js, key);
    if (j->type != JSON_OBJECT) ERR("Object %s is not of type OBJECT", key);
    value->start = j->start;
    json_parse_object(js->string+j->start, value);
}

void json_parse_file(const char * file_path, json_struct *obj){
    obj->string = get_string_from_file(file_path);
    obj->nelements = 0;
    obj->root = calloc(1, sizeof(json_object));
    json_parse_object(obj->string, obj->root);
}

void json_list_keys(const json_struct *js, const json_object *obj){
    printf("Keys:\n");
    json_node *j = obj->first;
    do{
        printf("%.*s\n", j->key_length, js->string+j->start_key);
    }while((j=j->next));
}

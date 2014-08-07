#include <stdio.h>
#include <stdlib.h>
#include "jsonc.h"

static parse_state determine_parse_state(char);
static void assign_value(void * it, json_type t, json_struct *js);

void init_json_struct(json_struct * js){
	js->root = NULL;
	js->size = 0;
}

bool is_empty(json_struct *js){
	return (js->size == 0 && js->root == NULL);
}

static void assign_value(void * it, json_type t, json_struct *js){
	switch (t){
		case STRING: js->root->member.value.string = (char *) it;break;
		case NUMBER: js->root->member.value.number = *(float *)it;break;
		case OBJECT: js->root->member.value.object = NULL;break;
		case ARRAY: js->root->member.value.array = (json_value *)it;break;
		case BOOL: js->root->member.value.bit = *(bool *)it;break;
		default: ERR("Wrong data type encountered, exiting ...\n");
	}
}

void add_item(char * key, void * it, json_type t, json_struct *js){
	printf("Adding item with key %s and type %u\n", key, t);
	json_object *new;
	json_object *cursor = js->root;
	new = (json_object *) malloc(sizeof(json_object));

	if (!is_empty(js)) {
		while (cursor->next != NULL) cursor = cursor->next;
		cursor->next = new;
	}
	else
		js->root = new;
	new->next = NULL;
	new->member.type = t;
	new->member.key = key;
	assign_value(it, t, js);
	js->size++;
}

void parse_json(json_struct *js, const char * json){
	const char * cursor;
	init_json_struct(js);
	parse_state p;
	json_object *new;
	char * key = "root";
	char * start;
	char * end;

	cursor = json;

	for (; *cursor != '\0'; cursor++){
		putc(*cursor, stdout);

		p = determine_parse_state(*cursor);

		switch (p){
			case START_OBJECT:
				add_item(key, NULL, OBJECT, js);
				break;
			case SKIP:
				continue;
				break;
			default:
				continue;
				break;
		}

	}

	printf("The string is %d characters long\n", cursor-json);
}

static parse_state determine_parse_state(char c){
	static bool escape;
	static bool open_key;
	static bool open_value;
	static char previous = ':';
	static parse_state state = START_OBJECT;

	switch (c){
		case ',':
			if (!open_key && !open_value){
				previous = ',';
			}
		case ':': 
			if (!open_key && !open_value){
				previous = ':';
			}break;
		case '"':
			if (!escape){
				if (!open_key && !open_value && previous==','){ open_key=true; return START_KEY;}
				else if (open_key && !open_value && previous==','){ open_key=false; return END_KEY;}
				else if (!open_key && !open_value && previous==':'){ open_value=true;return START_STRING_VALUE;}
				else if (!open_key && open_value && previous==':'){ open_value=false;return END_STRING_VALUE;}
				else return SKIP;
			}
			break;
		case '{':
				return START_OBJECT;
		case '\\':
				escape = true;
		default:
				escape = false;
	}

	return SKIP;
}

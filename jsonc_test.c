#include "jsonc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t begin, end;
    double time_spent;
    begin = clock();
    FILE *fp = fopen("/home/jan/.config/colors/config.json", "r");
	json_object *root = json_parse_file(fp);
    end = clock();
    time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
    printf("Time spent: %f", time_spent);
    printf("Root: %s\n", root->key);
    json_object *menu = (json_object*)&root->value;
    json_object *popup = (json_object*)&menu->next->next->value;
    json_array *menuitem = (json_array*)&popup->value;
    json_object *first = (json_object*)&menuitem->value;
    printf("value first: %s\n", first->next->value);
	return 0;
}

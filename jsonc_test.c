#include "jsonc.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	json_struct js;
    json_parse_file("/home/jan/.config/colors/solarized_light.json", &js);

    json_object colors;
    json_get_object(&js, "colors", &colors);
    json_list_keys(&js, &colors);
	return 0;
}

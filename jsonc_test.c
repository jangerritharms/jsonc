#include "jsonc.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Creates a string from a file
 */
static char * file_to_string(const char * fn){
	/* Read file to string */
	FILE *fp;
	long lSize;
	char *js;

	fp = fopen(fn, "r");
	if (!fp) ERR("Error opening file");
	fseek (fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);
	js = calloc(1, lSize+1);
	if (!js) fclose(fp),fputs("Memory allocation failed", stderr), exit(1);
	if (1!=fread(js, lSize, 1, fp))
		fclose(fp), fputs("Reading of file failed", stderr), exit(1);
	fclose(fp);

	return js;
}
int main(int argc, const char *argv[])
{
	json_struct js;
	const char *json = file_to_string("/home/jan/.config/colors/solarized_light.json");
	/* const char *js = "{ \"Hello\" : \"World\" }"; */
	parse_json(&js, json);
	return 0;
}

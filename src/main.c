#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg_parser.h"
#include "app.h"

/**************************** DEFINES ****************************/

/**************************** STATIC VARIABLES ****************************/

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** MAIN FUNCTION ****************************/
int main(int argc, char* argv[]) {
    /* Parse command line arguments */
    arg_t arguments = {
        .input_type = INPUT_UNDEFINED,
        .input_val = NULL,
        .output_type = OUTPUT_UNDEFINED,
        .output_val = NULL,
        .operation = OPERATION_UNDEFINED,
    };

    printf("Start application...\n");

    parser_status_t status = parse_args(argc, argv, &arguments);

    /* Check if provided option is available */
    if (status == PARSE_ERROR_UNKNOWN_OPTION) {
        printf("Error! Status code %d: Unknown option!\n", status);
        return PARSE_ERROR_UNKNOWN_OPTION;
    }

    /* Check if provided input source type is available*/
    if (status == PARSE_ERROR_UNDEFINED_INPUT) {
      printf("Error! Status code %d: Undefined input!\n", status);
      return PARSE_ERROR_UNDEFINED_INPUT;
    }

    char* operation = (arguments.operation == COMPRESSION) ? "Compressing" : "Decompressing";
    printf("%s from \"%s\"...", operation, arguments.input_val);

    /* Start application */
    app_status_t app_status = app(arguments);

    if (app_status == APP_STATUS_ERROR) {
      printf("Error! Application status code: %d\n", app_status);
    }

    printf("Done.\n");
    printf("Result can be found in \"%s\".\n", arguments.output_val);
    printf("Exiting application.\n");

    return 0;
}

/**************************** STATIC FUNCTION DEFINITIONS ****************************/
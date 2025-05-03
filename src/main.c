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

    parser_status_t arg_status = parse_args(argc, argv, &arguments);

    /* Check if provided input source type is available */
    if (arg_status == PARSE_ERROR_UNDEFINED_INPUT) {
        printf("ERROR! Exit with status code %d: Undefined input!\n", arg_status);
        return PARSE_ERROR_UNDEFINED_INPUT;
    }

    /* Check if provided input source type is available */
    if (arg_status == PARSE_ERROR_UNDEFINED_OUTPUT) {
        printf("ERROR! Exit with status code %d: Undefined output!\n", arg_status);
        return PARSE_ERROR_UNDEFINED_OUTPUT;
    }

    /* Check if operation option is provided */
    if (arg_status == PARSE_ERROR_UNDEFINED_OPERATION) {
        printf("ERROR! Exit with status code %d: Operation option is required!\n", arg_status);
        return PARSE_ERROR_UNDEFINED_OPERATION;
    }

    /* Check if provided option is available */
    if (arg_status == PARSE_ERROR_UNKNOWN_OPTION) {
        printf("ERROR! Exit with status code %d: Unknown option!\n", arg_status);
        return PARSE_ERROR_UNKNOWN_OPTION;
    }

    char* operation = (arguments.operation == COMPRESSION) ? "Compressing" : "Decompressing";
    printf("%s from \"%s\"...", operation, arguments.input_val);

    /* Start application */
    app_status_t app_status = app(arguments);

    if (app_status == APP_STATUS_ERROR) {
      printf("ERROR! Application finished with status code: %d.\n", app_status);
      return APP_STATUS_ERROR;
    }

    printf("[Done]\nResult can be found in \"%s\".\n", arguments.output_val);
    printf("Exiting application.\n");

    return EXIT_SUCCESS;
}

/**************************** STATIC FUNCTION DEFINITIONS ****************************/
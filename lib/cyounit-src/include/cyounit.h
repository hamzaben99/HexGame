#ifndef CYOUNIT_LIBRARY_H
#define CYOUNIT_LIBRARY_H

#include <stdio.h>

#ifndef COLOR_SUCCESS
#define COLOR_SUCCESS "\033[1;32m"
#endif

#ifndef COLOR_FAILURE
#define COLOR_FAILURE "\033[1;31m"
#endif

#ifndef COLOR_RESET
#define COLOR_RESET "\033[0m"
#endif

/**
 * Checks the value of given expression, prints colored result if message
 * is not an empty string, and update testing variables.
 * @param EXPR The evaluation
 * @param MSG The message to display
 */
#define cy_assert(EXPR, MSG) __cy_assert(EXPR, MSG, current_test);


/**
 * Initializes child testing variables and asserts given function name
 * @param NAME The testing function to call
 */
#define cy_run_test(NAME)                                           \
    {                                                               \
        struct cyounit *child_test = __cy_init_child(current_test); \
        cy_assert(NAME(child_test) == 0, "");                      \
    }

/**
 * Writes the NAME function signature with the testing structure as argument
 * @param NAME The function to declare
 */
#define cy_new_test(NAME) int NAME(struct cyounit *current_test)

/**
 * Returns the number of fails
 */
#define cy_return_result()                       \
    {                                            \
        return __cy_return_result(current_test); \
    }

/**
 * Initializes main testing structure
 */
#define cy_init_testing() struct cyounit *current_test = __cy_init_testing();

/**
 * Indents the current line and displays the title of the current test
 * @param TITLE The tile of the current test
 */
#define cy_announce_test(TITLE)    \
    {                              \
        __cy_indent(current_test); \
        printf(TITLE"\n");         \
    }

/**
 * Displays the success rate of the current test
 */
#define cy_display_result() __cy_display_result(current_test);

/**
 * Stores the number of attempts and fails, and the indent level for display
 * purposes
 */
struct cyounit;

extern struct cyounit *__cy_init_testing();

extern struct cyounit *__cy_init_child(struct cyounit *parent_test);

extern void __cy_display_result(struct cyounit *current_test);

extern int __cy_return_result(struct cyounit *current_test);

extern void __cy_indent(struct cyounit *current_test);

extern void __cy_assert(int expression, char *message, struct cyounit *current_test);

extern void __cy_assert_success();

extern void __cy_assert_failed();

#endif //CYOUNIT_LIBRARY_H

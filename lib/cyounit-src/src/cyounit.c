#include "cyounit.h"
#include "cyounit_private.h"
#include <stdio.h>
#include <stdlib.h>

struct cyounit *__cy_init_testing(void) {
	struct cyounit *test = (struct cyounit *) malloc(sizeof(*test));
	test->indent = 0;
	test->attempts = 0;
	test->fails = 0;
	return test;
}

struct cyounit *__cy_init_child(struct cyounit *parent_test) {
	struct cyounit *child = __cy_init_testing();
	child->indent = parent_test->indent + 1;
	return child;
}

void __cy_assert(int expression, char *message, struct cyounit *current_test) {
	current_test->attempts += 1;
	int result = expression;
	if (message[0] != 0) {
		__cy_indent(current_test);
		printf("%s\t", message);
		if (result)
			__cy_assert_success();
		else {
			__cy_assert_failed();
			current_test->fails += 1;
		}
	} else {
		if (!result)
			current_test->fails += 1;
	}
}

void __cy_assert_success(void) {
	printf(COLOR_SUCCESS "OK" COLOR_RESET"\n");
}

void __cy_assert_failed(void) {
	printf(COLOR_FAILURE "FAIL" COLOR_RESET"\n");
}

void __cy_display_result(struct cyounit *current_test) {
	if (current_test->fails == 0)
		printf(COLOR_SUCCESS);
	else
		printf(COLOR_FAILURE);
	__cy_indent(current_test);
	printf("Success rate: %d/%d"COLOR_RESET"\n",
	       current_test->attempts - current_test->fails,
	       current_test->attempts);
}

int __cy_return_result(struct cyounit *current_test) {
	int fails = current_test->fails;
	free(current_test);
	return fails;
}

void __cy_indent(struct cyounit *current_test) {
	int tabs = current_test->indent;
	while (tabs-- > 0)
		putchar('\t');
}

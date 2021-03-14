#include <stdio.h>

#include "../include/cyounit.h"
#include "../src/cyounit_private.h"
#include "test-main.h"

int main(void) {
	cy_init_testing();
	cy_announce_test("Testing Cyounit library");
	cy_run_test(test_success);
	cy_display_result();
	cy_return_result();
}

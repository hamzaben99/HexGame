#include "../include/cyounit.h"
#include "../src/cyounit_private.h"

#include <string.h>

cy_new_test(one_is_different_from_two) {
	cy_assert(1 != 2, "1 is not equal to 2")
	cy_return_result();
}

cy_new_test(hello_is_different_from_world) {
	cy_assert(strcmp("hello", "world"), "'Hello' is not equal to 'world'")
	cy_return_result();
}

// Test suite
cy_new_test(test_success) {
	cy_announce_test("Testing assert successful cases")
	cy_run_test(one_is_different_from_two);
	cy_run_test(hello_is_different_from_world);
	cy_display_result();
	cy_return_result();
}

#include "cyounit.h"
#include "../lib/set-src/src/set_dynamic.h"

cy_new_test(check_value_with_non_empty_set) {
	struct set *s = set__empty();

	set__add(s, 0);
	int res = set__random_value(s);
	cy_assert(res == 0, "With only one value, the returned value is this value...");

	set__add(s, 1);
	set__add(s, 2);
	set__add(s, 3);
	set__add(s, 4);
	res = set__random_value(s);
	cy_assert((res == 0 || res == 1|| res == 2 || res == 3 || res == 4), "With multiple values, the returned value is one of this value...");

	set__remove(s, 0);
	set__remove(s, 1);
	set__remove(s, 2);
	set__remove(s, 4);
	res = set__random_value(s);
	cy_assert(res == 3, "After removing values and it stills only one...");

	set__free(s);
	cy_return_result();
}

cy_new_test(check_value_with_empty_set) {
	struct set *s = set__empty();
	int res = set__random_value(s);
	cy_assert(res == -1, "With empty set, the returned value is -1...");
	set__free(s);
	cy_return_result();
}

cy_new_test(f_set__random_value) {
	cy_announce_test("Testing function set__random_value...");
	cy_run_test(check_value_with_non_empty_set);
	cy_run_test(check_value_with_empty_set);
	cy_display_result();
	cy_return_result();
}

cy_new_test(test_set) {
	cy_announce_test("Testing library set.so");
	cy_run_test(f_set__random_value);
	cy_display_result();
	cy_return_result();
}

#include <stdio.h>
#include <string.h>
#include "cyounit.h"
#include "test_connex_player.h"
#include "player.h"
#include "test_methods.h"
#include "test_WTree.h"

cy_new_test(test_connex_player) {
	cy_announce_test("Testing file connex_player");
	cy_run_test(test_WTree);
	cy_run_test(test_methods);
	cy_display_result();
	cy_return_result();
}

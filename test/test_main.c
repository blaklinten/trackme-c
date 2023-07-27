#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

#include "../src/timer.h"

static void timer_start_test() {
  char *name = "test name", *client = "test client", *project = "test project",
       *description = "This is a test";

  struct StartInfo si = {name, client, project, description};
  struct Timer *t_ptr = create_timer();
  time_t started = start_timer(t_ptr, &si);

  assert_string_equal(name, t_ptr->name);
  assert_string_equal(client, t_ptr->client);
  assert_string_equal(project, t_ptr->project);
  assert_string_equal(description, t_ptr->description);
  assert_true(started <= t_ptr->start_time);
  assert_int_equal(TIMER_NOT_STARTED, t_ptr->end_time);
}

int main(void) {

  const struct CMUnitTest tests[] = {
      cmocka_unit_test(timer_start_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

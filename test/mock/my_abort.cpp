#include "my_abort.hpp"

/**
 * Custom abort function so we can mock it for unit tests
 */
void my_abort() {
  throw 1;
}

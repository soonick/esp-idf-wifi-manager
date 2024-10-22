#include "my_abort.hpp"

#include <stdlib.h>

/**
 * Custom abort function so we can mock it for unit tests
 */
void my_abort() {
  abort();
}

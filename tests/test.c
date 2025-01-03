#include "../include/server.h"
#include <criterion/criterion.h>

Test(server_test, check_ip_valid)
{
    cr_assert_eq(1, check_ip("0.0.0.0"), "Expected True (1)");
}

Test(server_test, check_ip_invalid)
{
    cr_assert_eq(0, check_ip("999.999.999.999"), "Expected False (0)");
}

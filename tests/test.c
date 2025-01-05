#include "../include/checker.h"
#include <criterion/criterion.h>

Test(checker_test, check_ip_valid)
{
    cr_assert_eq(1, check_ip("0.0.0.0"), "Expected True: 0.0.0.0");
    cr_assert_eq(1, check_ip("1.1.1.1"), "Expected True: 1.1.1.1");
    cr_assert_eq(1, check_ip("127.0.0.1"), "Expected True: 127.0.0.1");
    cr_assert_eq(1, check_ip("255.255.255.255"),
                 "Expected True: 255.255.255.255");
    cr_assert_eq(1, check_ip("192.168.1.1-192.168.1.10"),
                 "Expected True: 192.168.1.1-192.168.1.10");
    cr_assert_eq(1, check_ip("192.168.1.1-192.168.1.1"),
                 "Expected True: 192.168.1.1-192.168.1.1");
    cr_assert_eq(1, check_ip("0.0.0.0-255.255.255.255"),
                 "Expected True: 0.0.0.0-255.255.255.255");
}

Test(checker_test, check_ip_invalid)
{
    cr_assert_eq(0, check_ip(".0.0.1"), "Expected False: .0.0.1");
    cr_assert_eq(0, check_ip("0..0.1"), "Expected False: 0..0.1");
    cr_assert_eq(0, check_ip("0.0.0."), "Expected False: 0.0.0.");
    cr_assert_eq(0, check_ip("abc.def.ghi.bac"),
                 "Expected False: abc.def.ghi.bac");
    cr_assert_eq(0, check_ip("256.0.0.0"), "Expected False: 256.0.0.0");
    cr_assert_eq(0, check_ip("1.123.12"), "Expected False: 1.123.12");
    cr_assert_eq(0, check_ip("1.123.-1.2"), "Expected False: 1.123.-1.2");
    cr_assert_eq(0, check_ip("1.0001.123.1"), "Expected False: 1.0001.123.1");
    cr_assert_eq(0, check_ip(" 1.1.123.1"), "Expected False:  1.1.123.1");
    cr_assert_eq(0, check_ip("1.1. 123.1"), "Expected False: 1.1. 123.1");
    cr_assert_eq(0, check_ip("192.1.1.10-"), "Expected False: 192.1.1.10-");
    cr_assert_eq(0, check_ip("192.168.1.1-192.168.1.256"),
                 "Expected False: 192.168.1.1-192.168.1.256");
    cr_assert_eq(0, check_ip("192.168.1.1-abc.def.gha.bcd"),
                 "Expected False: 192.168.1.1-abc.def.gha.bcd");
    cr_assert_eq(0, check_ip("192.168.1.1.1-192.168.1.10"),
                 "Expected False: 192.168.1.1.1-192.168.1.10");
    cr_assert_eq(0, check_ip("192.168.1.1- 192.168.1.10"),
                 "Expected False: 192.168.1.1- 192.168.1.10");
    cr_assert_eq(0, check_ip("192.168.1.1--192.168.1.10"),
                 "Expected False: 192.168.1.1--192.168.1.10");
}

Test(checker_test, check_port_valid)
{
    cr_assert_eq(1, check_port("0"), "Expected True: 0");
    cr_assert_eq(1, check_port("65535"), "Expected True: 65535");
    cr_assert_eq(1, check_port("1"), "Expected True: 1");
    cr_assert_eq(1, check_port("8080"), "Expected True: 8080");
    cr_assert_eq(1, check_port("0-65535"), "Expected True: 0-65535");
    cr_assert_eq(1, check_port("1024-2048"), "Expected True: 1024-2048");
    cr_assert_eq(1, check_port("22-22"), "Expected True: 22-22");
}

Test(checker_test, check_port_invalid)
{
    cr_assert_eq(0, check_port("65536"), "Expected False: 65536");
    cr_assert_eq(0, check_port("-1"), "Expected False: -1");
    cr_assert_eq(0, check_port("123456"), "Expected False: 123456");
    cr_assert_eq(0, check_port("22a"), "Expected False: 22a");
    cr_assert_eq(0, check_port("080"), "Expected False: 080");
    cr_assert_eq(0, check_port(" 8"), "Expected False:  8");
    cr_assert_eq(0, check_port("0-"), "Expected False: 0-");
    cr_assert_eq(0, check_port("-1-65535"), "Expected False: -1-65535");
    cr_assert_eq(0, check_port("22-123456"), "Expected False: 22-123456");
    cr_assert_eq(0, check_port("8080-808a"), "Expected False: 8080-808a");
    cr_assert_eq(0, check_port("22- 80"), "Expected False: 22- 80");
}

Test(checker_test, check_ip_range_invalid)
{
    cr_assert_eq(0, check_ip_range("10.0.0.1", "10.0.0.0"),
                 "Expected False: 10.0.0.1-10.0.0.0");
    cr_assert_eq(0, check_ip_range("192.168.1.10", "192.168.1.1"),
                 "Expected False: 192.168.1.10-192.168.1.1");
    cr_assert_eq(0, check_ip_range("192.168.2.0", "192.168.1.255"),
                 "Expected False: 192.168.2.0-192.168.1.255");
    cr_assert_eq(0, check_ip_range("255.255.255.255", "192.168.1.1"),
                 "Expected False: 255.255.255.255-192.168.1.1");
    cr_assert_eq(0, check_ip_range("192.169.0.1", "192.168.255.255"),
                 "Expected False: 192.169.0.1-192.168.255.255");
    cr_assert_eq(0, check_ip_range("10.1.1.1", "10.0.255.255"),
                 "Expected False: 10.1.1.1-10.0.255.255");
}

Test(checker_test, check_port_range_invalid)
{
    cr_assert_eq(0, check_port_range("8081", "8080"),
                 "Expected False: 8081-8080");
    cr_assert_eq(0, check_port_range("65535", "1024"),
                 "Expected False: 65535-1024");
    cr_assert_eq(0, check_port_range("5000", "4000"),
                 "Expected False: 5000-4000");
    cr_assert_eq(0, check_port_range("12345", "123"),
                 "Expected False: 12345-123");
    cr_assert_eq(0, check_port_range("80", "0"), "Expected False: 80-0");
}

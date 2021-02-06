#include <Parser.h>
#include <unity.h>

ConfiguredRingbuffer buff;
Parser p(buff);

void test_in_out()
{
    auto l1 = ">m0.0;10.111111;200;";
    for (int i = 0; i < sizeof(l1); i++)
    {
        p.push(l1[i]);
        if (i == sizeof(l1) - 1)
        {
            TEST_ASSERT(!buff.isEmpty());
        }
        else
        {
            TEST_ASSERT(buff.isEmpty());
        }
    }
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_in_out);
    UNITY_END();
    return 0;
}
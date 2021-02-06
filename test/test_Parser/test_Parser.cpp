#include <Parser.h>
#include <unity.h>

ConfiguredRingbuffer buff;
Parser p(buff);

void test_in_out()
{
    char l1[] = ">m0.0;10.111111;200;";
    for (int i = 0; i < sizeof(l1)-1; i++)
    {
        p.push(l1[i]);
        if (i == sizeof(l1) - 2)
        {
            TEST_ASSERT(!buff.isEmpty());
        }
        else
        {
            TEST_ASSERT(buff.isEmpty());
        }
    }
    auto b = buff.popBack();
    TEST_ASSERT_EQUAL(b.command, 'm');
    TEST_ASSERT_EQUAL(b.var0, 0.0);
    TEST_ASSERT_EQUAL(b.var1, 10.111111);
    TEST_ASSERT_EQUAL(b.var3, 200.0);    
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_in_out);
    UNITY_END();
    return 0;
}
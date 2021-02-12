#include <unity.h>
#include <CommandBuffer.h>

void test_in_out()
{
    CommandBuffer<20> cb;
    const char *commands[21]{
        ">m0.3;0.0;50.0;",
        ">m3.1;2.6;50.0;",
        ">m0.2;0.2;50.0;",
        ">m3.2;2.5;50.0;",
        ">m0.2;0.1;50.0;",
        ">m3.3;2.5;50.0;",
        ">m4.8;3.7;50.0;",
        ">m7.2;6.6;50.0;",
        ">m4.6;3.8;50.0;",
        ">m7.2;6.5;50.0;",
        ">m4.7;3.7;50.0;",
        ">m7.3;6.5;50.0;",
        ">m8.7;7.9;50.0;",
        ">m10.6;11.2;50.0;",
        ">m8.5;8.0;50.0;",
        ">m10.7;11.1;50.0;",
        ">m8.6;7.9;50.0;",
        ">m10.8;11.1;50.0;",
        ">m11.9;12.6;50.0;",
        ">m13.2;16.2;50.0;",
        ">m11.7;12.7;50.0;"};

    for (size_t i = 0; i < 20; i++)
    {
        auto current = commands[i];
        for (size_t j = 0; current[j] != 0; j++)
        {
            TEST_ASSERT(cb.push(current[j]));
        }
    }
    TEST_ASSERT(cb.isFull());

    for (size_t i = 0; i < 19; i++)
    {
        cb.pop();
        TEST_ASSERT(!cb.isFull());
        TEST_ASSERT(!cb.isEmpty());
        TEST_ASSERT(cb.size() == 19 - i);
    }
    cb.pop();
    TEST_ASSERT(cb.isEmpty());
    Serial.println("");
    for(size_t i = 0; commands[20][i] != 0; i++)
    {
        cb.push(commands[20][i]);
    }
    TEST_ASSERT(!cb.isEmpty());
    TEST_ASSERT(!cb.isFull());
    TEST_ASSERT(cb.size() == 1);
    Serial.println("");
    for(size_t i = 0; commands[20][i] != 0; i++)
    {
        cb.push(commands[1][i]);
    }
    TEST_ASSERT(!cb.isEmpty());
    TEST_ASSERT(!cb.isFull());
    Serial.println(cb.size());
    TEST_ASSERT(cb.size() == 2);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_in_out);
    UNITY_END();
    return 0;
}
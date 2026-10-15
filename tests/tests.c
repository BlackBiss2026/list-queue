/*
 * Automatic tests for list.h / queue.h.
 * Usage:  ./build/tests <test-name>     run one test
 *         ./build/tests all             run every test
 *         ./build/tests list            print the test names
 *
 * Each test runs in its own process in CI, so a crash fails only that test.
 */
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "queue.h"

static int failures = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);         \
            failures++;                                                      \
        }                                                                    \
    } while (0)

#define REQUIRE(cond)                                                        \
    do {                                                                     \
        if (!(cond)) {                                                       \
            printf("  FAIL %s:%d: %s (stopping this test)\n", __FILE__,      \
                   __LINE__, #cond);                                         \
            failures++;                                                      \
            return;                                                          \
        }                                                                    \
    } while (0)

#define CHECK_DOUBLE(actual, expected)                                       \
    do {                                                                     \
        double a_ = (actual), e_ = (expected);                               \
        if (fabs(a_ - e_) > 1e-6 * (fabs(e_) > 1 ? fabs(e_) : 1)) {          \
            printf("  FAIL %s:%d: %s == %.6f, expected %.6f\n", __FILE__,    \
                   __LINE__, #actual, a_, e_);                               \
            failures++;                                                      \
        }                                                                    \
    } while (0)

/* ------------------------------------------------------------------ */
/* List tests                                                          */
/* ------------------------------------------------------------------ */

static void test_list_create_empty(void)
{
    List *l = list_create();
    REQUIRE(l != NULL);
    CHECK(list_length(l) == 0);
    CHECK(list_first(l) == NULL);
    CHECK(list_last(l) == NULL);
    CHECK_DOUBLE(list_average(l), 0.0);
    list_destroy(l);
    list_destroy(NULL); /* must be safe */
}

static void test_list_add_order(void)
{
    List *l = list_create();
    REQUIRE(l != NULL);
    ListNode *a = list_add(l, 10);
    ListNode *b = list_add(l, 20);
    ListNode *c = list_add(l, 30);
    REQUIRE(a && b && c);
    CHECK(list_value(a) == 10);
    CHECK(list_value(b) == 20);
    CHECK(list_value(c) == 30);

    /* forward */
    CHECK(list_first(l) == a);
    CHECK(list_next(a) == b);
    CHECK(list_next(b) == c);
    CHECK(list_next(c) == NULL);
    /* backward */
    CHECK(list_last(l) == c);
    CHECK(list_prev(c) == b);
    CHECK(list_prev(b) == a);
    CHECK(list_prev(a) == NULL);
    list_destroy(l);
}

static void test_list_length(void)
{
    List *l = list_create();
    REQUIRE(l != NULL);
    int i;
    for (i = 0; i < 1000; i++) {
        REQUIRE(list_add(l, i) != NULL);
        CHECK(list_length(l) == (size_t)(i + 1));
    }
    /* walk the whole list forward and check every value */
    ListNode *n = list_first(l);
    for (i = 0; i < 1000 && n != NULL; i++, n = list_next(n))
        CHECK(list_value(n) == i);
    CHECK(i == 1000);
    CHECK(n == NULL);
    list_destroy(l);
}

static void test_list_remove(void)
{
    List *l = list_create();
    REQUIRE(l != NULL);
    ListNode *n[5];
    int i;
    for (i = 0; i < 5; i++)
        REQUIRE((n[i] = list_add(l, i + 1)) != NULL); /* 1 2 3 4 5 */

    list_remove(l, n[2]); /* middle -> 1 2 4 5 */
    CHECK(list_length(l) == 4);
    CHECK(list_next(n[1]) == n[3]);
    CHECK(list_prev(n[3]) == n[1]);

    list_remove(l, n[0]); /* first -> 2 4 5 */
    CHECK(list_length(l) == 3);
    CHECK(list_first(l) == n[1]);
    CHECK(list_prev(n[1]) == NULL);

    list_remove(l, n[4]); /* last -> 2 4 */
    CHECK(list_length(l) == 2);
    CHECK(list_last(l) == n[3]);
    CHECK(list_next(n[3]) == NULL);

    list_remove(l, n[1]); /* -> 4 */
    CHECK(list_first(l) == n[3]);
    CHECK(list_last(l) == n[3]);

    list_remove(l, n[3]); /* only element -> empty */
    CHECK(list_length(l) == 0);
    CHECK(list_first(l) == NULL);
    CHECK(list_last(l) == NULL);

    /* list must still be usable after becoming empty */
    ListNode *x = list_add(l, 42);
    REQUIRE(x != NULL);
    CHECK(list_first(l) == x && list_last(l) == x);
    CHECK(list_length(l) == 1);
    list_destroy(l);
}

static void test_list_average(void)
{
    List *l = list_create();
    REQUIRE(l != NULL);
    list_add(l, 1);
    list_add(l, 2);
    CHECK_DOUBLE(list_average(l), 1.5);
    ListNode *neg = list_add(l, -9);
    CHECK_DOUBLE(list_average(l), -2.0);
    list_remove(l, neg);
    CHECK_DOUBLE(list_average(l), 1.5);
    list_destroy(l);

    /* the sum does not fit in an int */
    l = list_create();
    REQUIRE(l != NULL);
    list_add(l, INT_MAX);
    list_add(l, INT_MAX);
    list_add(l, INT_MAX);
    CHECK_DOUBLE(list_average(l), (double)INT_MAX);
    list_destroy(l);

    l = list_create();
    REQUIRE(l != NULL);
    list_add(l, INT_MIN);
    list_add(l, INT_MIN);
    CHECK_DOUBLE(list_average(l), (double)INT_MIN);
    list_destroy(l);
}

/* ------------------------------------------------------------------ */
/* Queue tests                                                         */
/* ------------------------------------------------------------------ */

static void test_queue_empty(void)
{
    Queue *q = queue_create();
    REQUIRE(q != NULL);
    int out = 1234;
    CHECK(queue_length(q) == 0);
    CHECK(queue_dequeue(q, &out) == -1);
    CHECK(out == 1234);
    CHECK(queue_peek(q, &out) == -1);
    CHECK(out == 1234);
    queue_destroy(q);
    queue_destroy(NULL); /* must be safe */
}

static void test_queue_fifo(void)
{
    Queue *q = queue_create();
    REQUIRE(q != NULL);
    int i, out;
    for (i = 1; i <= 100; i++)
        CHECK(queue_enqueue(q, i * 7) == 0);
    CHECK(queue_length(q) == 100);
    for (i = 1; i <= 100; i++) {
        out = -1;
        CHECK(queue_dequeue(q, &out) == 0);
        CHECK(out == i * 7);
    }
    CHECK(queue_length(q) == 0);
    CHECK(queue_dequeue(q, &out) == -1);
    queue_destroy(q);
}

static void test_queue_peek(void)
{
    Queue *q = queue_create();
    REQUIRE(q != NULL);
    int out = 0;
    queue_enqueue(q, 5);
    queue_enqueue(q, 6);
    CHECK(queue_peek(q, &out) == 0);
    CHECK(out == 5);
    CHECK(queue_peek(q, &out) == 0);
    CHECK(out == 5);
    CHECK(queue_length(q) == 2); /* peek does not remove */
    queue_dequeue(q, &out);
    CHECK(queue_peek(q, &out) == 0);
    CHECK(out == 6);
    queue_destroy(q);
}

static void test_queue_mixed(void)
{
    Queue *q = queue_create();
    REQUIRE(q != NULL);
    int out = 0;
    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    CHECK(queue_dequeue(q, &out) == 0 && out == 1);
    queue_enqueue(q, 3);
    CHECK(queue_length(q) == 2);
    CHECK(queue_dequeue(q, &out) == 0 && out == 2);
    CHECK(queue_dequeue(q, &out) == 0 && out == 3);
    CHECK(queue_length(q) == 0);
    /* empty again, then reuse */
    queue_enqueue(q, 4);
    CHECK(queue_peek(q, &out) == 0 && out == 4);
    CHECK(queue_length(q) == 1);
    /* destroy while not empty - must not leak */
    queue_enqueue(q, 5);
    queue_enqueue(q, 6);
    queue_destroy(q);
}

/* ------------------------------------------------------------------ */

struct test {
    const char *name;
    void (*fn)(void);
};

static const struct test TESTS[] = {
    {"list_create_empty", test_list_create_empty},
    {"list_add_order", test_list_add_order},
    {"list_length", test_list_length},
    {"list_remove", test_list_remove},
    {"list_average", test_list_average},
    {"queue_empty", test_queue_empty},
    {"queue_fifo", test_queue_fifo},
    {"queue_peek", test_queue_peek},
    {"queue_mixed", test_queue_mixed},
};
#define NTESTS (sizeof TESTS / sizeof TESTS[0])

int main(int argc, char *argv[])
{
    size_t i;
    int ran = 0;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <test-name> | all | list\n", argv[0]);
        return 2;
    }
    if (strcmp(argv[1], "list") == 0) {
        for (i = 0; i < NTESTS; i++)
            puts(TESTS[i].name);
        return 0;
    }
    for (i = 0; i < NTESTS; i++) {
        if (strcmp(argv[1], "all") == 0 || strcmp(argv[1], TESTS[i].name) == 0) {
            int before = failures;
            printf("%s\n", TESTS[i].name);
            fflush(stdout);
            TESTS[i].fn();
            printf("  %s\n", failures == before ? "PASS" : "FAILED");
            ran++;
        }
    }
    if (!ran) {
        fprintf(stderr, "unknown test '%s'\n", argv[1]);
        return 2;
    }
    return failures ? 1 : 0;
}

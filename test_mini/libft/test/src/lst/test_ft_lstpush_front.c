#include "libft_test.h"

TEST_GROUP(ft_lstpush_front);

TEST_SETUP(ft_lstpush_front)
{}

TEST_TEAR_DOWN(ft_lstpush_front)
{}

TEST(ft_lstpush_front, basic)
{
	t_ftlst *lst;
	int a = 1;
	int b = 2;
	int c = 2;

	lst = ft_lstnew(&a);
	TEST_ASSERT_NOT_NULL(lst);

	ft_lstpush_front(&lst, ft_lstnew(&b));
	TEST_ASSERT_NOT_NULL(lst);
	TEST_ASSERT_EQUAL_PTR(&b, lst->data);
	TEST_ASSERT_EQUAL(b, *(int*)lst->data);
	TEST_ASSERT_NOT_NULL(lst->next);
	TEST_ASSERT_EQUAL_PTR(&a, lst->next->data);
	TEST_ASSERT_EQUAL(a, *(int*)lst->next->data);

	ft_lstpush_front(&lst, ft_lstnew(&c));
	TEST_ASSERT_NOT_NULL(lst);
	TEST_ASSERT_EQUAL_PTR(&c, lst->data);
	TEST_ASSERT_EQUAL(c, *(int*)lst->data);
	TEST_ASSERT_NOT_NULL(lst->next);
	TEST_ASSERT_EQUAL_PTR(&b, lst->next->data);
	TEST_ASSERT_EQUAL(b, *(int*)lst->next->data);
	TEST_ASSERT_NOT_NULL(lst->next->next);
	TEST_ASSERT_EQUAL_PTR(&a, lst->next->next->data);
	TEST_ASSERT_EQUAL(a, *(int*)lst->next->next->data);

	ft_lstdestroy(&lst, NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:49:02 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 01:26:38 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	print_test_header(const char *test_name)
{
	printf("\n%s%s%s\n", TXT_YELLOW, test_name, TXT_RST);
	printf("-------------------------------------------------\n");
}

void	check_test(int test_no, int expected, int actual)
{
	if (expected != actual)
		printf("%sTest failed: test %d (expected %d, got %d)\n%s", \
					TXT_RED, test_no, expected, actual, TXT_RST);
	else
		printf("%sTest passed: test %d (expected %d, got %d)\n%s", \
					TXT_GREEN, test_no, expected, actual, TXT_RST);
}

void	test_contains_only_digits(void)
{
	int	i;

	print_test_header(__func__);
	i = 1;
	check_test(i++, 1, contains_only_digits("1"));
	check_test(i++, 1, contains_only_digits("123"));
	check_test(i++, 1, contains_only_digits("0"));
	check_test(i++, 0, contains_only_digits("0 0"));
	check_test(i++, 0, contains_only_digits(""));
	check_test(i++, 0, contains_only_digits("0_"));
	check_test(i++, 0, contains_only_digits("-1"));
	check_test(i++, 0, contains_only_digits("2a"));
	check_test(i++, 0, contains_only_digits("a"));
	check_test(i++, 0, contains_only_digits("-a"));
	check_test(i++, 0, contains_only_digits(" "));
}

void test_is_valid_single_argument(void)
{
	int	i;

	print_test_header(__func__);
	i = 1;
	check_test(i++, 1, is_valid_single_argument("1"));
	check_test(i++, 0, is_valid_single_argument("11232132134512323"));
	check_test(i++, 1, is_valid_single_argument("123"));
	check_test(i++, 1, is_valid_single_argument("0"));
	check_test(i++, 0, is_valid_single_argument("0 0"));
	check_test(i++, 0, is_valid_single_argument(""));
	check_test(i++, 0, is_valid_single_argument("0_"));
	check_test(i++, 0, is_valid_single_argument("-1"));
	check_test(i++, 0, is_valid_single_argument("2a"));
	check_test(i++, 0, is_valid_single_argument("a"));
	check_test(i++, 0, is_valid_single_argument("-a"));
}

void test_is_valid_input(void)
{
	int	i;

	print_test_header(__func__);
	i = 1;
	check_test(i++, 1, is_valid_input((char *[]){"1", "2", "3", "4", NULL}, 4));
	check_test(i++, 1, is_valid_input((char *[]){"1", "2", \
								"3", "4", "5", NULL}, 5));
	check_test(i++, 0, is_valid_input((char *[]){"1", "2a", \
								"3", "4", NULL}, 4));
	check_test(i++, 0, is_valid_input((char *[]){"1", "2", \
								"-3", "4", "5", NULL}, 5));
	check_test(i++, 0, is_valid_input((char *[]){"1", "2", \
								"3", "4", "5", "6", NULL}, 6));
	check_test(i++, 0, is_valid_input((char *[]){"1", "2", NULL}, 2));
	check_test(i++, 0, is_valid_input((char *[]){"1", "2", "3", NULL}, 3));
	check_test(i++, 0, is_valid_input((char *[]){"1", "2", "a3a", NULL}, 3));
}

void	test_int_atoi(void)
{
	int	i;

	print_test_header(__func__);
	i = 1;
	check_test(i++, 123, int_atoi("123"));
	check_test(i++, 999, int_atoi("999"));
	check_test(i++, 0, int_atoi("0"));
}

void	test_ft_strlen(void)
{
	int	i;

	print_test_header(__func__);
	i = 1;
	check_test(i++, 3, ft_strlen("123"));
	check_test(i++, 0, ft_strlen(""));
	check_test(i++, 5, ft_strlen("hello"));
	check_test(i++, 8, ft_strlen("hello123"));
	check_test(i++, 11, ft_strlen("hello123!!!"));
}

int main(void)
{
	test_contains_only_digits();
	test_is_valid_single_argument();
	test_is_valid_input();
	test_int_atoi();
	test_ft_strlen();
}

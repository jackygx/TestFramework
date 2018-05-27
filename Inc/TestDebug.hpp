/*
 * Copyright (c) 2018 Guo Xiang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TEST_DEBUG_HPP__
#define __TEST_DEBUG_HPP__

#include <Common/Color.hpp>

#define TEST_PRINT(color, ...)	\
	TRACE_INFO(color, __VA_ARGS__, COLOR_NONE)

#ifdef DEBUG_TEST
#define TEST_DEBUG(...) \
	TRACE_INFO(__VA_ARGS__, EOS)
#else
#define TEST_DEBUG(color, ...)
#endif

#define TEST_INFO(color, ...) \
	TRACE_INFO(color, __VA_ARGS__, COLOR_NONE, EOS)

#define TEST_ERROR(...) \
	TEST_INFO(COLOR_RED, __func__, "[", DEC(__LINE__), "]", __VA_ARGS__)

#define TEST_PASSED(...) \
	TEST_INFO(COLOR_GREEN, __VA_ARGS__);

#define TEST_FAILED(...) \
	TEST_INFO(COLOR_RED, __VA_ARGS__);

#ifdef SHOW_TEST_CASE
#define DEFINE_TEST_GROUP(name) static auto TestCase##name(int &iStep, const char *topic = #name)
#define TEST_CASE(desc)			TEST_INFO(COLOR_CYAN, topic, "[", DEC(++iStep, 2, '0'), "]: ", desc);
#else /* !SHOW_TEST_CASE */
#define DEFINE_TEST_GROUP(name) static auto TestCase##name(int &iStep)
#define TEST_CASE(desc)			++iStep;
#endif /* SHOW_TEST_CASE */

#define START_TEST() int iTestCases = 0;
#define RUN_TEST_GROUP(name) \
	do { \
		int i = 0; \
		TestCase##name(i); \
		iTestCases += i; \
	} while (0)
#define STOP_TEST() \
	TEST_PASSED(DEC(iTestCases), " test cases passed") \

#define RUN_TEST(func, ...) \
	do { \
		TEST_INFO(COLOR_YELLOW, "================= Start testing " #func " ================="); \
		func(##__VA_ARGS__); \
	} while (0)

#define CHECK(cond, ...) \
	do { \
		if (!(cond)) { \
			TEST_ERROR(">>> " #cond " <<< failed. ", __VA_ARGS__); \
			throw ES("fail"); \
		} \
	} while (0)

#define CHECK_THROW(func, ...) \
	do { \
		try { \
			func; \
			__VA_ARGS__; \
			TEST_ERROR("\"" #func "\" does not throw error"); \
			throw ES("fail"); \
		} catch (...) { \
		} \
	} while (0)

#endif /* __TEST_DEBUG_HPP__ */


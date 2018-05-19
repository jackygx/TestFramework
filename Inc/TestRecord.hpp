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

#ifndef __TEST_RECORD_HPP__
#define __TEST_RECORD_HPP__

#include "TestDebug.hpp"

/* Used to record each test step */
class CTestRecord
{
private:
	uint32_t mTestNum;

public:
	inline CTestRecord(void);

	/* Save the test step [i].
	 * If the step [i] has already been tested, throw error.
	 * If the step ([i] - 1) has not been tested, throw error.
	 */
	inline void Save(uint32_t i);

	/* Check if all [nTest] tests are done */
	inline void Check(uint32_t nTest);
};

inline CTestRecord::CTestRecord(void) :
	mTestNum(0)
{
	/* Does nothing */
}

inline void CTestRecord::Save(uint32_t i)
{
	CHECK(i >= mTestNum, "%d already tested", i);
	CHECK(i <= mTestNum, "%d is not tested", mTestNum);
	++mTestNum;
}

inline void CTestRecord::Check(uint32_t nTest)
{
	CHECK(mTestNum == nTest,
		  "Not all tests run. Expect %d tests but run %d",
		  nTest, mTestNum);
}

#endif /* __TEST_RECORD_HPP__ */


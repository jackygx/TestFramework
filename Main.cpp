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

//#define DEBUG_TEST

#include <EasyCpp.hpp>
#include <CmdlineParser.hpp>
#include <Io.hpp>

#include "Inc/TestFramework.hpp"

DEFINE_FUNC(Test, void(void));

static void Start(const CConstStringPtr &path)
{
	TEST_DEBUG("Path: ", path);

	CreateDirent(path)->ToFolder()->ForEach()->Then([&](const IDirentPtr &dir) {
		auto name(dir->GetBaseName());
		name->Split(".")->First()->Then([&](const CConstStringPtr &prename) {
			auto size(prename->GetSize());
			return If(size > 4 &&
				prename[size - 4] == 'T' &&
				prename[size - 3] == 'e' &&
				prename[size - 2] == 's' &&
				prename[size - 1] == 't');
		})->Then([&](void) {
			CConstStringPtr fullName(path, "/", name);
			TEST_DEBUG("loading: ", fullName);
			return Platform::CreateDynamicLib(fullName);
		})->Then([&](const CDynamicLibraryPtr &dlib) {
			return dlib->GetFunc<TestFn>("TestCaseEntry");
		})->Then([&](const TestFn &fn) {
			try {
				fn();
				TEST_PASSED(name, " test passed!!!");
			} catch (const IException *e) {
				TEST_FAILED(name, " test failed!!!");
				e->Show();
				delete e;
			}
		})->Catch([&](void) {
			TEST_DEBUG(name, " is not a test lib\n");
		});
	});
}

int main(int argc, char *argv[])
{
	/* To show the pretty call stack */
	InitSymbolPath(argv[0]);

	try {
		CCmdlineParser parser(argc, argv);
		parser.AddKey('p', "Path of the test case");
		parser.AddKey('h', "Show this help message");
		parser.Parse();

		if (parser.IsKeySet('h')) {
			parser.PrintUsage(0);
		}

		if (!parser.IsKeySet('p')) {
			Start("./");
		} else {
			Start(parser.GetKeyArg('p', 0));
		}
	} catch (const IException *e) {
		e->Show();
		delete e;
	} catch (...) {
		TRACE_ERROR("Unknown error!!!");
	}

	return 0;
}


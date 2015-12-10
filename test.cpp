/***
    This file is part of popl (program options parser lib)
    Copyright (C) 2015  Johannes Pohl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "popl.h"
#include <string>

using namespace std;
using namespace popl;


int main (int argc, char **argv)
{
	OptionParser op("Allowed options");
	std::string version, list;
	int help, i;
	bool b;

	Value<int> v1("h", "help", "produce help message", 4, help);
	Value<string> v2("l", "", "list pcm devices");
	Value<string> v3("", "version", "show version number");
//	Value<int> v4("", "12345678", "test of a long option name");
	Value<int> v4("", "1234567890123456789012345678", "test of a long option name");
//	Value<int> v4("", "1234567890123456789012345678901234567890", "test of a long option name");
	Value<float> v5("f", "float", "transport codec [flac|ogg|pcm][:options]\nType codec:? to get codec specific options");
	Switch s1("s", "switch", "switch test", b);
	Implicit<int> i1("i", "implicit", "implicit test", 5, i);

	op.add(v1)
	  .add(v2.assignTo(list))
	  .add(v3.setDefault("xxx").assignTo(version))
	  .add(v4)
	  .add(v5)
	  .add(s1)
	  .add(i1);

	op.parse(argc, argv);
	cout << "\n\nop:\n" << op << "\n\n";

	cout << "Help: " << help << ", value: " << v1.getValue() << ", count: " << v1.count() << "\n"
		<< "Version: " << version << ", value: " << v3.getValue() << "\n"
		<< "Float: " << v5.getValue() << "\n"
		<< "List: " << list << "\n"
		<< "switch: " << s1.getValue() << ", b: " << b << "\n"
		<< "implicit: " << i1.getValue() << ", count: " << i1.count() << ", i: " << i << "\n";
}


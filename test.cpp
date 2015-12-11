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

#include "popl.hpp"
#include <string>

using namespace std;
using namespace popl;


int main (int argc, char **argv)
{
	OptionParser op("Allowed options");
	std::string s;
	int i, m;
	bool version;

	Switch helpSwitchOption("h", "help", "produce help message");
	Switch versionSwitchOption("v", "version", "show version number", &version);

	Value<int> longOption("", "1234567890123456789012345678901234567890", "test of a long option name");

	Value<float> floatOption("f", "float", "test for float values");
	Value<float> floatOptionDefault("", "floatDefault", "test for float values with default", 0.1);
	Value<int> intOption("i", "int", "test for int values");
	Value<int> intOptionDefault("", "intDefault", "test for int values with default", 42);
	Value<int> intOptionDefaultAssign("", "intDefaultAssign", "test for int values with default\nAssign result to variable i", 42, &i);
	Value<string> stringOption("s", "string", "test for string values");
	Value<string> stringOptionAssign("", "stringAssign", "test for string values\nAssign result to variable s", "", &s);

	Implicit<int> implicitIntOption("m", "implicit", "implicit test", 5, &m);

	op.add(helpSwitchOption)
	  .add(versionSwitchOption)
	  .add(longOption)
	  .add(floatOption)
	  .add(floatOptionDefault)
	  .add(intOption)
	  .add(intOptionDefault)
	  .add(intOptionDefaultAssign)
	  .add(stringOption)
	  .add(stringOptionAssign)
	  .add(implicitIntOption);

	op.parse(argc, argv);

	if (helpSwitchOption.isSet())
	{
		cout << op << "\n";
		return 0;
	}

	if (version)
	{
		cout << VERSION << "\n";
		return 0;
	}

	cout
		<< "floatOption        isSet: " << floatOption.isSet() << ", value: " << floatOption.getValue() << "\n"
		<< "floatOptionDefault isSet: " << floatOptionDefault.isSet() << ", value: " << floatOptionDefault.getValue() << "\n"
		<< "intOption          isSet: " << intOption.isSet() << ", value: " << intOption.getValue() << "\n"
		<< "intOptionDefault   isSet: " << intOptionDefault.isSet() << ", value: " << intOptionDefault.getValue() << "\n"
		<< "intOptionDefaultA  isSet: " << intOptionDefaultAssign.isSet() << ", value: " << intOptionDefaultAssign.getValue() << ", var: " << i << "\n"
		<< "stringOption       isSet: " << stringOption.isSet() << ", value: " << stringOption.getValue() << "\n"
		<< "stringOptionAssign isSet: " << stringOptionAssign.isSet() << ", value: " << stringOptionAssign.getValue() << ", var: " << s << "\n"
		<< "implicitIntOption  isSet: " << implicitIntOption.isSet() << ", value: " << implicitIntOption.getValue() << ", var: " << m << "\n";

	for (size_t n=0; n<op.nonOptionArgs().size(); ++n)
		cout << "NonOptionArg: " << op.nonOptionArgs()[n] << "\n";

	for (size_t n=0; n<op.unknownOptions().size(); ++n)
		cout << "UnknownOptions: " << op.unknownOptions()[n] << "\n";
}



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

using namespace std;
using namespace popl;


int main(int argc, char **argv)
{
	float f;
	int m;

	Switch helpOption("h", "help", "produce help message");
	Value<float> floatOption("f", "float", "test for float values", 1.23f, &f);
	Value<double> doubleOption("d", "double", "test for double values", 3.14159265359);
	Value<string> stringOption("s", "string", "test for string values");
	Implicit<int> implicitIntOption("m", "implicit", "implicit test", 42, &m);

	OptionParser op("Allowed options");
	op.add(helpOption)
	.add(floatOption)
	.add(doubleOption)
	.add(stringOption)
	.add(implicitIntOption);

	op.parse(argc, argv);

	// print auto-generated help message
	if (helpOption.isSet())
		cout << op << "\n";

	// show all non option arguments (those without "-o" or "--option")
	for (size_t n=0; n<op.nonOptionArgs().size(); ++n)
		cout << "NonOptionArg: " << op.nonOptionArgs()[n] << "\n";

	// show unknown options (undefined ones, like "-u" or "--undefined")
	for (size_t n=0; n<op.unknownOptions().size(); ++n)
		cout << "UnknownOptions: " << op.unknownOptions()[n] << "\n";

	// print all the configured values
	cout << "floatOption  - isSet: " << floatOption.isSet() << ", value: " << floatOption.getValue() << ", reference: " << f << "\n";
	cout << "doubleOption - isSet: " << doubleOption.isSet() << ", value: " << doubleOption.getValue() << "\n";
	if (stringOption.isSet())
	{
	  	for (size_t n=0; n<stringOption.count(); ++n)
			cout << "stringOption #" << n << " - value: " << stringOption.getValue(n) << "\n";
	}
	cout << "implicitIntOption - value: " << implicitIntOption.getValue() << ", isSet: " << implicitIntOption.isSet() << ", reference: " << m << "\n";
}



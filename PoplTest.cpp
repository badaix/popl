/***
    This file is part of popl (program options parser lib)
    Copyright (C) 2015-2017 Johannes Pohl
    
    This software may be modified and distributed under the terms
    of the MIT license.  See the LICENSE file for details.
***/

#include "popl.hpp"

using namespace std;
using namespace popl;


int main(int argc, char **argv)
{
	float f;
	int m;

	Switch help_option("h", "help", "produce help message");
	Switch verbose_option("v", "", "verbose");
	Switch hidden_option("x", "", "hidden option");
	Value<float> float_option("f", "float", "test for float values", 1.23f, &f);
	Value<double> double_option("d", "double", "test for double values", 3.14159265359);
	Value<string> string_option("s", "string", "test for string values");
	Implicit<int> implicit_int_option("m", "implicit", "implicit test", 42, &m);

	OptionParser op("Allowed options");
	op.add(help_option)
	.add(verbose_option)
	.add(hidden_option, hidden)
	.add(float_option)
	.add(double_option)
	.add(string_option)
	.add(implicit_int_option);

	op.parse(argc, argv);

	// print auto-generated help message
	if (help_option.is_set())
		cout << op << "\n";

	// show all non option arguments (those without "-o" or "--option")
	for (size_t n=0; n<op.non_option_args().size(); ++n)
		cout << "non_option_args: " << op.non_option_args()[n] << "\n";

	// show unknown options (undefined ones, like "-u" or "--undefined")
	for (size_t n=0; n<op.unknown_options().size(); ++n)
		cout << "unknown_options: " << op.unknown_options()[n] << "\n";

	// print all the configured values
	cout << "verbose_option  - count: " << verbose_option.count() << "\n";
	cout << "hidden_option  - count: " << hidden_option.count() << "\n";
	cout << "float_option  - is_set: " << float_option.is_set() << ", value: " << float_option.value() << ", reference: " << f << "\n";
	cout << "double_option - is_set: " << double_option.is_set() << ", value: " << double_option.value() << "\n";
	if (string_option.is_set())
	{
	  	for (size_t n=0; n<string_option.count(); ++n)
			cout << "string_option #" << n << " - value: " << string_option.value(n) << "\n";
	}
	cout << "implicit_int_option - value: " << implicit_int_option.value() << ", is_set: " << implicit_int_option.is_set() << ", reference: " << m << "\n";
}



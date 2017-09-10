/***
    This file is part of popl (program options parser lib)
    Copyright (C) 2015-2017 Johannes Pohl
    
    This software may be modified and distributed under the terms
    of the MIT license.  See the LICENSE file for details.
***/

#include "popl.hpp"
#include <iostream>

using namespace std;
using namespace popl;


int main(int argc, char **argv)
{
	float f;
	int m, i;
	bool v;

	OptionParser op("Allowed options");
	auto help_option    = op.add<Switch>("h", "help", "produce help message");
	auto verbose_option = op.add<Switch>("v", "verbose", "be verbose", &v);
	auto hidden_option  = op.add<Switch>("x", "", "hidden option");
	auto double_option  = op.add<Value<double>>("d", "double", "test for double values", 3.14159265359);
	auto float_option   = op.add<Value<float>>("f", "float", "test for float values", 2.71828182845f, &f);
	                      op.add<Value<int>>("i", "int", "test for int value w/o option", 23, &i);
	auto string_option  = op.add<Value<string>>("s", "string", "test for string values");
	auto implicit_int_option = op.add<Implicit<int>>("m", "implicit", "implicit test", 42);
	implicit_int_option->assign_to(&m);

	op.parse(argc, argv);

	TerminalHelpPrinter hp;
	cout << hp.print_help(op) << "\n";

	// print auto-generated help message
	if (help_option->is_set())
		cout << op << "\n";

	// show all non option arguments (those without "-o" or "--option")
	for (size_t n=0; n<op.non_option_args().size(); ++n)
		cout << "non_option_args: " << op.non_option_args()[n] << "\n";

	// show unknown options (undefined ones, like "-u" or "--undefined")
	for (size_t n=0; n<op.unknown_options().size(); ++n)
		cout << "unknown_options: " << op.unknown_options()[n] << "\n";

	// print all the configured values
	cout << "verbose_option - is_set: " << verbose_option->is_set() << ", count: " << verbose_option->count() << ", reference: " << v << "\n";
	cout << "hidden_option  - is_set: " << hidden_option->is_set() << ", count: " << hidden_option->count() << "\n";
	cout << "double_option  - is_set: " << double_option->is_set() << ", count: " << double_option->count() << ", value: " << double_option->value() << "\n";
	cout << "string_option  - is_set: " << string_option->is_set() << ", count: " << string_option->count() << "\n";
	if (string_option->is_set())
	{
	  	for (size_t n=0; n<string_option->count(); ++n)
			cout << "string_option #" << n << " - value: " << string_option->value(n) << "\n";
	}
	cout << "float_option   - is_set: " << float_option->is_set() << ", value: " << float_option->value() << ", reference: " << f << "\n";
	cout << "int w/o option - reference: " << i << "\n";
	auto int_option = op.get_option<Value<int>>('i');
	cout << "int_option     - is_set: " << int_option->is_set() << ", value: " << int_option->value() << ", reference: " << i << "\n";
	cout << "imp_int_option - is_set: " << implicit_int_option->is_set() << ", value: " << implicit_int_option->value() << ", reference: " << m << "\n";
}



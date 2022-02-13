/***
    This file is part of popl (program options parser lib)
    Copyright (C) 2015-2021 Johannes Pohl
    
    This software may be modified and distributed under the terms
    of the MIT license.  See the LICENSE file for details.
***/

#include "popl.hpp"
#include <iostream>
#include <utility>

using namespace std;
using namespace popl;


int main(int argc, char **argv)
{
	float f;
	int m, i;
	bool v;

	OptionParser op("Allowed options");
	auto help_option     = op.add<Switch>("h", "help", "produce help message");
	auto groff_option    = op.add<Switch>("", "groff", "produce groff formatted help message");
	auto bash_option     = op.add<Switch>("", "bash", "produce bash completion script");
	auto verbose_option  = op.add<Switch, Attribute::optional>("v", "verbose", "be verbose", &v);
	auto hidden_option   = op.add<Switch, Attribute::hidden>("x", "", "hidden option");
	auto double_option   = op.add<Value<double>>("d", "double", "test for double values", 3.14159265359);
	auto float_option    = op.add<Value<float>>("f", "float", "test for float values", 2.71828182845f, &f);
	                       op.add<Value<int>>("i", "int", "test for int value w/o option", 23, &i);
	auto string_option   = op.add<Value<string>>("s", "string", "test for string values");
	auto implicit_int_option = op.add<Implicit<int>>("m", "implicit", "implicit test", 42);
	auto advanced_option = op.add<Switch, Attribute::advanced>("", "advanced", "advanced option");
	auto expert_option   = op.add<Switch, Attribute::expert>("", "expert", "expert option");
	auto inactive_option = op.add<Switch>("", "inactive", "inactive option");
	inactive_option->set_attribute(Attribute::inactive);
	implicit_int_option->assign_to(&m);

	try
	{
		op.parse(argc, argv);

		// print auto-generated help message
		if (help_option->count() == 1)
			cout << op << "\n";
		else if (help_option->count() == 2)
			cout << op.help(Attribute::advanced) << "\n";
		else if (help_option->count() > 2)
			cout << op.help(Attribute::expert) << "\n";

		if (groff_option->is_set())
		{
			GroffOptionPrinter option_printer(&op);
			cout << option_printer.print();
		}

		if (bash_option->is_set())
		{
			BashCompletionOptionPrinter option_printer(&op, "popl_example");
			cout << option_printer.print();
		}

		// show all non option arguments (those without "-o" or "--option")
		for (const auto& non_option_arg: op.non_option_args())
			cout << "non_option_args: " << non_option_arg << "\n";

		// show unknown options (undefined ones, like "-u" or "--undefined")
		for (const auto& unknown_option: op.unknown_options())
			cout << "unknown_options: " << unknown_option << "\n";

		// print all the configured values
		cout << "verbose_option  - is_set: " << verbose_option->is_set() << ", count: " << verbose_option->count() << ", reference: " << v << "\n";
		cout << "hidden_option   - is_set: " << hidden_option->is_set() << ", count: " << hidden_option->count() << "\n";
		cout << "double_option   - is_set: " << double_option->is_set() << ", count: " << double_option->count() << ", value: " << double_option->value() << "\n";
		cout << "string_option   - is_set: " << string_option->is_set() << ", count: " << string_option->count() << "\n";
		if (string_option->is_set())
		{
			for (size_t n=0; n<string_option->count(); ++n)
				cout << "string_option #" << n << " - value: " << string_option->value(n) << "\n";
		}
		cout << "float_option    - is_set: " << float_option->is_set() << ", value: " << float_option->value() << ", reference: " << f << "\n";
		cout << "int w/o option  - reference: " << i << "\n";
		auto int_option = op.get_option<Value<int>>('i');
		cout << "int_option      - is_set: " << int_option->is_set() << ", value: " << int_option->value() << ", reference: " << i << "\n";
		cout << "imp_int_option  - is_set: " << implicit_int_option->is_set() << ", value: " << implicit_int_option->value() << ", reference: " << m << "\n";
		cout << "advanced_option - is_set: " << advanced_option->is_set() << ", count: " << advanced_option->count() << "\n";
		cout << "expert_option   - is_set: " << expert_option->is_set() << ", count: " << expert_option->count() << "\n";


		const char *ptr_array[3] = { "xxx", "-h", "-f 0.5" };
		op.parse(3, ptr_array);
		cout << "\n\nSecond parse:\n";
		cout << "verbose_option  - is_set: " << verbose_option->is_set() << ", count: " << verbose_option->count() << ", reference: " << v << "\n";
		cout << "float_option    - is_set: " << float_option->is_set() << ", value: " << float_option->value() << ", reference: " << f << "\n";
		cout << "int_option      - is_set: " << int_option->is_set() << ", value: " << int_option->value() << ", reference: " << i << "\n";
	}
	catch (const popl::invalid_option& e)
	{
		cerr << "Invalid Option Exception: " << e.what() << "\n";
		cerr << "error:  ";
		if (e.error() == invalid_option::Error::missing_argument)
			cerr << "missing_argument\n";
		else if (e.error() == invalid_option::Error::invalid_argument)
			cerr << "invalid_argument\n";
		else if (e.error() == invalid_option::Error::too_many_arguments)
			cerr << "too_many_arguments\n";
		else if (e.error() == invalid_option::Error::missing_option)
			cerr << "missing_option\n";

		if (e.error() == invalid_option::Error::missing_option)
		{
			string option_name(e.option()->name(OptionName::short_name, true));
			if (option_name.empty())
				option_name = e.option()->name(OptionName::long_name, true);
			cerr << "option: " << option_name << "\n";
		}
		else
		{
			cerr << "option: " << e.option()->name(e.what_name()) << "\n";
			cerr << "value:  " << e.value() << "\n";
		}
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}



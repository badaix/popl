# popl

Program Options Parser Library

[![Github Releases](https://img.shields.io/github/release/badaix/popl.svg)](https://github.com/badaix/popl/releases)
[![Build Status](https://travis-ci.org/badaix/popl.svg?branch=master)](https://travis-ci.org/badaix/popl)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/badaix/popl.svg)](https://lgtm.com/projects/g/badaix/popl/context:cpp)  

popl is a C++ command line arguments parser that supports the same set of options as GNU's `getopt` and thus closely follows the POSIX guidelines for the command-line options of a program.

## Features

* Single header file implementation. Simply include and use it!
* No external dependencies, just C++11
* Platform independent
* Supports the same set of options as GNU's `getopt`: short options, long options, non-option arguments, ...
* Supports parsing of `ini` files
* Templatized option parsing: arguments are directly casted into the desired target type
* Automatic creation of a usage message
  * Console help message
  * [Groff](https://www.gnu.org/software/groff/) formatted help message for use in man pages
  * Script snippets for use in [bash completion](https://debian-administration.org/article/316/An_introduction_to_bash_completion_part_1) scripts
* Easy to use: no strange braces syntax, but for each command line option one typesafe object

## Howto

Key object is `OptionParser`, which is populated with different option types:

* `Value<T>` Option with argument  
* `Switch` Option without argument  
* `Implicit<T>` Option with optional argument (using an implicit value if no argument is given)  

Next, OptionParser will parse the command line (by passing `argc` and `argv`) and fill the option objects.  
Each option type is initialized with a short option, long option and a help message.  

### Basic usage example

```C++
OptionParser op("Allowed options");
auto help_option   = op.add<Switch>("h", "help", "produce help message");
auto string_option = op.add<Value<std::string>>("s", "string", "some string value");
auto implicit_int  = op.add<Implicit<int>>("m", "implicit", "implicit value", 42);
op.parse(argc, argv);

// print auto-generated help message
if (help_option->is_set())
	cout << op << "\n";
cout << "string_option - is_set: " << string_option->is_set() << ", value: " << string_option->value() << "\n";
cout << "implicit_int  - is_set: " << implicit_int->is_set() << ", value: " << implicit_int->value() << "\n";
```

### Multiple definition

Options can be set multiple times on command line. Use `count()` and `value(n)` to access them:

```C++
cout << "string_option - count: " << string_option->count() << "\n";
if (string_option->is_set())
{
	for (size_t n=0; n<string_option->count(); ++n)
		cout << "string_option #" << n << " - value: " << string_option->value(n) << "\n";
}
```

### Default values

Every option type can have a default value:

```C++
auto string_option = op.add<Value<std::string>>("s", "string", "some string value", "default value");
```

if not set on command line, `string_option->is_set()` will be `false` and `string_option->value()` will be `default value` 
  
### Assigning to a variable

The argument of an option can be directly assigned to a variable:

```C++
std::string s;
/*auto string_option =*/ op.add<Value<std::string>>("s", "string", "some string value", "default value", &s);
```

The variable `s` will carry the same value as `string_option.value()`, and thus the declaration of `string_option` can be omitted.  
  
### Attributes of an option

Options have an `Attribute`: they can be hidden in the auto-created help message, or classified as "advanced", or "expert":

```C++
auto string_option = op.add<Value<std::string>>("s", "string", "some string value");
auto advanced_int  = op.add<Value<int>, Attribute::advanced>("i", "integer", "advanced integer value");
auto hidden_bool   = op.add<Swtich, Attribute::hidden>("", "hidden", "hidden flag");
```

Now `cout << op.help()` (same as `cout << op`) will not show the hidden or advanced option, while `cout << op.help(Attribute::advanced)` will show the advanced option. The hidden one is never shown to the user.  
Also an option can be flagged as mandatory by assigning `Attribute::required`

## Example

```C++
#include "popl.hpp"

using namespace std;
using namespace popl;

int main(int argc, char **argv)
{
	float f;
	int m, i;
	bool v;

	OptionParser op("Allowed options");
	auto help_option     = op.add<Switch>("h", "help", "produce help message");
	auto verbose_option  = op.add<Switch>("v", "verbose", "be verbose", &v);
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

	op.parse(argc, argv);

	// print auto-generated help message
	if (help_option->count() == 1)
		cout << op << "\n";
	else if (help_option->count() == 2)
		cout << op.help(Attribute::advanced) << "\n";
	else if (help_option->count() > 2)
		cout << op.help(Attribute::expert) << "\n";

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
}
```

A call to `popl -s hello -h -m23 test` will produce an output like this:

```shell
Allowed options:
  -h, --help                   produce help message
  -v, --verbose                be verbose
  -d, --double arg (=3.14159)  test for double values
  -f, --float arg (=2.71828)   test for float values
  -i, --int arg (=23)          test for int value w/o option
  -s, --string arg             test for string values
  -m, --implicit [=arg(=42)]   implicit test

non_option_args: test
verbose_option  - is_set: 0, count: 0, reference: 0
hidden_option   - is_set: 0, count: 0
double_option   - is_set: 0, count: 0, value: 3.14159
string_option   - is_set: 1, count: 1
string_option #0 - value: hello
float_option    - is_set: 0, value: 2.71828, reference: 2.71828
int w/o option  - reference: 23
int_option      - is_set: 0, value: 23, reference: 23
imp_int_option  - is_set: 1, value: 23, reference: 23
advanced_option - is_set: 0, count: 0
expert_option   - is_set: 0, count: 0
```

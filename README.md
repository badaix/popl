# popl

Program Options Parser Lib

popl is a c++ option parser that supports the same options as GNU's `getopt` and thus closely follows the POSIX guidelines for the command-line options of a program.

## Features
* Single header file implementation. Simply include and use it!
* Supports the same set of options as GNU's `getopt`: short options, long options, non-option arguments, ...
* Templatized option parsing: arguments are directly casted into the desired target type
* Automatic creation of a usage message

Key objects are:
* `Value<T>` Option with argument  
  `Value<int> intOption("i", "int", "configures an integer value", 42 /*optional: default*/, &i /*optional: assign value to i*/);`
* `Swtich` Option without argument  
  `Switch helpOption("h", "help", "produce help message", &h /*optinal: assign bool to b*/);`
* `Implicit<T>` Option with optional argument (using an implicit value if no argument is given)  
  `Implicit<int> implicitIntOption("v", "verbose", "verbosity level", 2);`

## And why?
There are a lot of option parsers around. My favorite one is `boost program options`. But thanks to the new C++11 language features, I sometimes end up in just using the `program options` part of boost. Since it's not a header only library, even a small tool has to be linked against `boost system` and `boost program options`.  
I tried out two other libs that were recommended on stackoverflow. Both had their small flaws (from my point of view), and so I switched to `getopt`, with it's main drawbacks: 
* Short and long argument of a single option are defined on two places
* Short and long option are parsed on two different places
* The result is a string and has to be casted to the target type
* The obligatory help option has to implemented manually

I started to work around these drawbacks and ended up with this `popl` library.

## Example
```C++
#include "popl.hpp"

using namespace std;
using namespace popl;

int main(int argc, char **argv)
{
	float f;
	int m;
	bool t;

	Switch helpOption("h", "help", "produce help message");
	Switch testOption("t", "test", "execute another test", &t);
	Value<float> floatOption("f", "float", "test for float values", 1.23, &f);
	Value<string> stringOption("s", "string", "test for string values");
	Implicit<int> implicitIntOption("m", "implicit", "implicit test", 42, &m);

	OptionParser op("Allowed options");
	op.add(helpOption)
	.add(testOption)
	.add(floatOption)
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
	cout << "testOption - value: " << testOption.getValue() << ", isSet: " << testOption.isSet() << ", count: " << testOption.count() << ", reference: " << t << "\n";
	cout << "floatOption - value: " << floatOption.getValue() << ", reference: " << f << "\n";
	cout << "stringOption - value: " << stringOption.getValue() << "\n";
	cout << "implicitIntOption - value: " << implicitIntOption.getValue() << ", isSet: " << implicitIntOption.isSet() << ", reference: " << m << "\n";
}
```

A call to `./popl -s hello -h -m23 test` will produce an output like this:

```
Allowed options:
  -h, --help                  produce help message
  -t, --test                  execute another test
  -f, --float arg (=1.23)     test for float values
  -s, --string arg            test for string values
  -m, --implicit [=arg(=42)]  implicit test

NonOptionArg: test
testOption - value: 0, isSet: 0, count: 0, reference: 0
floatOption - value: 1.23, reference: 1.23
stringOption - value: hello
implicitIntOption - value: 23, isSet: 1, reference: 23
```

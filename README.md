# popl

program options parser lib

popl is a c++ wrapper around GNU's `getopt`

## Features
* `Value<T>` Option with argument
* `Swtich` Option without argument
* `Implicit<T>` Option with optional argument (using an implicit value if no argument is given)

## Example
```C++
Switch helpOption("h", "help", "produce help message");
Value<float> floatOption("f", "float", "test for float values", 0.5, &f);
Value<string> stringOption("s", "string", "test for string values");
Implicit<int> implicitIntOption("m", "implicit", "implicit test", 42, &m);

OptionParser op("Allowed options");
op.add(helpOption)
  .add(floatOption)
  .add(stringOption)
  .add(implicitIntOption);

op.parse(argc, argv);

for (size_t n=0; n<op.nonOptionArgs().size(); ++n)
	cout << "NonOptionArg: " << op.nonOptionArgs()[n] << "\n";

for (size_t n=0; n<op.unknownOptions().size(); ++n)
	cout << "UnknownOptions: " << op.unknownOptions()[n] << "\n";

if (helpOption.isSet())
{
	cout << op << "\n";
	return 0;
}
```

a call to `./popl -h -w` will produce an output like this:

```
UnknownOptions: -w
Allowed options:
  -h, --help                  produce help message
  -f, --float arg (=0.5)      test for float values
  -s, --string arg            test for string values
  -m, --implicit [=arg(=42)]  implicit test
```

/***
    This file is part of popl (program options parser lib)
    Copyright (C) 2015-2017 Johannes Pohl
    
    This software may be modified and distributed under the terms
    of the MIT license.  See the LICENSE file for details.
***/


#ifndef POPL_H
#define POPL_H

#define NOMINMAX

#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdexcept>


namespace popl
{

#define POPL_VERSION "0.6.0"


enum // permitted values for its `has_arg' field...
{
	no_argument = 0,    // option never takes an argument
	required_argument,  // option always requires an argument
	optional_argument   // option may take an argument
};


enum Attribute
{
	null,
	hidden
	//advanced // not used for now
};


class Option
{
friend class OptionParser;
public:
	Option(const std::string& short_option, const std::string& long_option, const std::string& description);

	char short_option() const;
	std::string long_option() const;
	std::string description() const;
	unsigned int count() const;
	bool is_set() const;
	void set_attribute(const Attribute& attribute);
	Attribute attribute() const;

protected:
	virtual void parse(const std::string& what_option, const char* value) = 0;
	virtual void update_reference();
	virtual std::string to_string() const;
	virtual int has_arg() const = 0;

	std::string short_option_;
	std::string long_option_;
	std::string description_;
	unsigned int count_;
	Attribute attribute_;
};




template<class T>
class Value : public Option
{
public:
	Value(const std::string& short_option, const std::string& long_option, const std::string& description);
	Value(const std::string& short_option, const std::string& long_option, const std::string& description, const T& default_val, T* assign_to = NULL);

	Value<T>& assign_to(T* var);
	Value<T>& set_default(const T& value);
	T value(size_t idx = 0) const;
	void set_value(const T& value);

protected:
	virtual void parse(const std::string& what_option, const char* value);
	virtual std::string to_string() const;
	virtual int has_arg() const;
	virtual void add_value(const T& value);
	virtual void update_reference();
	T* assign_to_;
	std::vector<T> values_;
	T default_;
	bool has_default_;
};




template<class T>
class Implicit : public Value<T>
{
public:
	Implicit(const std::string& short_option, const std::string& long_option, const std::string& description, const T& implicitVal);
	Implicit(const std::string& short_option, const std::string& long_option, const std::string& description, const T& implicitVal, T* assign_to = NULL);

	Value<T>& assign_to(T* var);

protected:
	virtual void parse(const std::string& what_option, const char* value);
	virtual std::string to_string() const;
	virtual int has_arg() const;
	Value<T>& set_default(const T& value);
};




class Switch : public Value<bool>
{
public:
	Switch(const std::string& short_option, const std::string& long_option, const std::string& description);
	Switch(const std::string& short_option, const std::string& long_option, const std::string& description, bool* assign_to);

protected:
	virtual void parse(const std::string& what_option, const char* value);
	virtual std::string to_string() const;
	virtual int has_arg() const;
	Switch& set_default(const bool& value);
};




class OptionParser
{
public:
	OptionParser(const std::string& description = "");
	virtual ~OptionParser();

	OptionParser& add(Option& option, const Attribute& attribute = null);
	void parse(int argc, char **argv);
	std::string help() const;
	const std::vector<Option*>& options() const;
	const std::vector<std::string>& non_option_args() const;
	const std::vector<std::string>& unknown_options() const;

protected:
	std::vector<Option*> options_;
	std::string description_;
	std::vector<std::string> non_option_args_;
	std::vector<std::string> unknown_options_;

	Option* get_long_opt(const std::string& opt) const;
	Option* get_short_opt(char opt) const;
};





/// Option implementation /////////////////////////////////

Option::Option(const std::string& short_option, const std::string& long_option, const std::string& description) :
	short_option_(short_option),
	long_option_(long_option),
	description_(description),
	count_(0),
	attribute_(null)
{
	if (short_option.size() > 1)
		throw std::invalid_argument("length of short option must be <= 1: '" + short_option + "'");

	if (short_option.empty() && long_option.empty())
		throw std::invalid_argument("short and long option are empty");
}


void Option::update_reference()
{
}


char Option::short_option() const
{
	if (!short_option_.empty())
		return short_option_[0];
	return 0;
}


std::string Option::long_option() const
{
	return long_option_;
}


std::string Option::description() const
{
	return description_;
}


unsigned int Option::count() const
{
	return count_;
}


bool Option::is_set() const
{
	return (count() > 0);
}


std::string Option::to_string() const
{
	std::stringstream line;
	if (short_option() != 0)
	{
		line << "  -" << short_option();
		if (!long_option().empty())
			line << ", ";
	}
	else
		line << "  ";

	if (!long_option().empty())
		line << "--" << long_option();

	return line.str();
}


void Option::set_attribute(const Attribute& attribute)
{
	this->attribute_ = attribute;
}


Attribute Option::attribute() const
{
	return attribute_;
}





/// Value implementation /////////////////////////////////

template<class T>
Value<T>::Value(const std::string& short_option, const std::string& long_option, const std::string& description) :
	Option(short_option, long_option, description),
	assign_to_(NULL),
	has_default_(false)
{
}


template<class T>
Value<T>::Value(const std::string& short_option, const std::string& long_option, const std::string& description, const T& default_val, T* assign_to) :
	Option(short_option, long_option, description),
	assign_to_(assign_to),
	default_(default_val),
	has_default_(true)
{
	update_reference();
}


template<class T>
Value<T>& Value<T>::assign_to(T* var)
{
	assign_to_ = var;
	return *this;
}


template<class T>
Value<T>& Value<T>::set_default(const T& value)
{
	default_ = value;
	has_default_ = true;
	return *this;
}


template<class T>
void Value<T>::update_reference()
{
	if (assign_to_ != NULL)
	{
		if (is_set() || has_default_)
			*assign_to_ = value();
	}
}


template<class T>
void Value<T>::add_value(const T& value)
{
	values_.push_back(value);
	++count_;
	update_reference();
}


template<class T>
void Value<T>::set_value(const T& value)
{
	values_.clear();
	add_value(value);
}


template<class T>
T Value<T>::value(size_t idx) const
{
	if (!is_set())
	{
		if (has_default_)
			return default_;
		else
		{
			std::stringstream optionStr;
			if (short_option() != 0)
				optionStr << "-" << short_option();
			else
				optionStr << "--" << long_option();

			throw std::out_of_range("option not set: \"" + optionStr.str() + "\"");
		}
	}

	if (idx >= count_)
	{
		std::stringstream optionStr;
		optionStr << "index out of range (" << idx << ") for \"";
		if (short_option() != 0)
			optionStr << "-" << short_option();
		else
			optionStr << "--" << long_option();
		optionStr << "\"";
		throw std::out_of_range(optionStr.str());
	}

	return values_[idx];
}


template<class T>
int Value<T>::has_arg() const
{
	return required_argument;
}


template<>
void Value<std::string>::parse(const std::string& what_option, const char* value)
{
	if (strlen(value) == 0)
		throw std::invalid_argument("missing argument for " + what_option);

	add_value(value);
}


template<class T>
void Value<T>::parse(const std::string& what_option, const char* value)
{
	T parsedValue;
	std::string strValue;
	if (value != NULL)
		strValue = value;

	std::istringstream is(strValue);
	int valuesRead = 0;
	while (is.good())
	{
		if (is.peek() != EOF)
			is >> parsedValue;
		else
			break;

		valuesRead++;
	}

	if (is.fail())
		throw std::invalid_argument("invalid argument for " + what_option + ": '" + strValue + "'");

	if (valuesRead > 1)
		throw std::invalid_argument("too many arguments for " + what_option + ": '" + strValue + "'");

	if (strValue.empty())
		throw std::invalid_argument("missing argument for " + what_option);

	add_value(parsedValue);
}


template<class T>
std::string Value<T>::to_string() const
{
	std::stringstream ss;
	ss << Option::to_string() << " arg";
	if (has_default_)
	{
		std::stringstream defaultStr;
		defaultStr << default_;
		if (!defaultStr.str().empty())
			ss << " (=" << default_ << ")";
	}
	return ss.str();
}





/// Implicit implementation /////////////////////////////////

template<class T>
Implicit<T>::Implicit(const std::string& short_option, const std::string& long_option, const std::string& description, const T& implicitVal) :
	Value<T>(short_option, long_option, description, implicitVal)
{
}


template<class T>
Implicit<T>::Implicit(const std::string& short_option, const std::string& long_option, const std::string& description, const T& implicitVal, T* assign_to) :
	Value<T>(short_option, long_option, description, implicitVal, assign_to)
{
}


template<class T>
int Implicit<T>::has_arg() const
{
	return optional_argument;
}


template<class T>
void Implicit<T>::parse(const std::string& what_option, const char* value)
{
	if ((value != NULL) && (strlen(value) > 0))
		Value<T>::parse(what_option, value);
	else
		this->add_value(this->default_);
}


template<class T>
std::string Implicit<T>::to_string() const
{
	std::stringstream ss;
	ss << Option::to_string() << " [=arg(=" << this->default_ << ")]";
	return ss.str();
}





/// Switch implementation /////////////////////////////////

Switch::Switch(const std::string& short_option, const std::string& long_option, const std::string& description) :
	Value<bool>(short_option, long_option, description, false)
{
}


Switch::Switch(const std::string& short_option, const std::string& long_option, const std::string& description, bool* assign_to) :
	Value<bool>(short_option, long_option, description, false, assign_to)
{
}


void Switch::parse(const std::string& what_option, const char* value)
{
	add_value(true);
}


int Switch::has_arg() const
{
	return no_argument;
}


std::string Switch::to_string() const
{
	return Option::to_string();
}





/// OptionParser implementation /////////////////////////////////

OptionParser::OptionParser(const std::string& description) : description_(description)
{
}


OptionParser::~OptionParser()
{
}


OptionParser& OptionParser::add(Option& option, const Attribute& attribute)
{
	for (size_t n=0; n<options_.size(); ++n)
	{
		if ((option.short_option() != 0) && (option.short_option() == options_[n]->short_option()))
			throw std::invalid_argument("dublicate short option '-" + std::string(1, option.short_option()) + "'");
		if (!option.long_option().empty() && (option.long_option() == (options_[n]->long_option())))
			throw std::invalid_argument("dublicate long option '--" + option.long_option() + "'");
	}
	if (attribute != null)
		option.set_attribute(attribute);
	options_.push_back(&option);
	return *this;
}


const std::vector<Option*>& OptionParser::options() const
{
	return options_;
}


const std::vector<std::string>& OptionParser::non_option_args() const
{
	return non_option_args_;
}


const std::vector<std::string>& OptionParser::unknown_options() const
{
	return unknown_options_;
}


std::string OptionParser::help() const
{
	std::stringstream s;
	if (!description_.empty())
		s << description_ << ":\n";

	size_t optionRightMargin(20);
	const size_t maxDescriptionLeftMargin(40);
//	const size_t descriptionRightMargin(80);

	for (size_t opt = 0; opt < options_.size(); ++opt)
		optionRightMargin = std::max(optionRightMargin, options_[opt]->to_string().size() + 2);
	optionRightMargin = std::min(maxDescriptionLeftMargin - 2, optionRightMargin);

	for (size_t opt = 0; opt < options_.size(); ++opt)
	{
		if (options_[opt]->attribute() == hidden)
			continue;
		std::string optionStr = options_[opt]->to_string();
		if (optionStr.size() < optionRightMargin)
			optionStr.resize(optionRightMargin, ' ');
		else
			optionStr += "\n" + std::string(optionRightMargin, ' ');
		s << optionStr;

		std::stringstream description(options_[opt]->description());
		std::string line;
		std::vector<std::string> lines;
		while (std::getline(description, line, '\n'))
			lines.push_back(line);

		std::string empty(optionRightMargin, ' ');
		for (size_t n=0; n<lines.size(); ++n)
		{
			if (n > 0)
				s << "\n" << empty;
			s << lines[n];
		}
		s << "\n";
	}

	return s.str();
}


Option* OptionParser::get_long_opt(const std::string& opt) const
{
	for (size_t n = 0; n < options_.size(); ++n)
	{
		if (options_[n]->long_option() == opt)
			return options_[n];
	}
	return NULL;
}


Option* OptionParser::get_short_opt(char opt) const
{
	for (size_t n = 0; n < options_.size(); ++n)
		if (options_[n]->short_option() == opt)
			return options_[n];
	return NULL;
}


void OptionParser::parse(int argc, char **argv)
{
	unknown_options_.clear();
	non_option_args_.clear();
	for (int n=1; n<argc; ++n)
	{
		const std::string arg(argv[n]);
		if (arg == "--")
		{
			///from here on only non opt args
			for (int m=n+1; m<argc; ++m)
				non_option_args_.push_back(argv[m]);

			break;
		}
		else if (arg.find("--") == 0)
		{
			/// long option arg
			std::string opt = arg.substr(2);
			std::string optarg;
			size_t equalIdx = opt.find('='); 
			if (equalIdx != std::string::npos)
			{
				optarg = opt.substr(equalIdx + 1);
				opt.resize(equalIdx);
			}

			Option* option = NULL;
			if ((option = get_long_opt(opt)) != NULL)
			{
				if (option->has_arg() == no_argument) 
				{
					if (!optarg.empty())
						option = NULL;
				}
				else if (option->has_arg() == required_argument)
				{
					if (optarg.empty() && n < argc-1)
						optarg = argv[++n];
				}
			}

			if (option != NULL)
				option->parse(opt, optarg.c_str());
			else
				unknown_options_.push_back(arg);
		}
		else if (arg.find("-") == 0)
		{
			/// short option arg
			std::string opt = arg.substr(1);
			bool unknown = false;
			for (size_t m=0; m<opt.size(); ++m)
			{
				char c = opt[m];
				Option* option = NULL;
				std::string optarg;

				if ((option = get_short_opt(c)) != NULL)
				{
					if (option->has_arg() == required_argument)
					{
						/// use the rest of the current argument as optarg
						optarg = opt.substr(m + 1);
						/// or the next arg
						if (optarg.empty() && n < argc-1)
							optarg = argv[++n];
						m = opt.size();
					}
					else if (option->has_arg() == optional_argument)
					{
						/// use the rest of the current argument as optarg
						optarg = opt.substr(m + 1);
						m = opt.size();
					}
				}

				if (option != NULL)
					option->parse(std::string(1, c), optarg.c_str());
				else
					unknown = true;
			}
			if (unknown)
				unknown_options_.push_back(arg);
		}
		else
		{
			non_option_args_.push_back(arg);
		}
	}
}


std::ostream& operator<<(std::ostream& out, const OptionParser& op)
{
	out << op.help();
	return out;
}


} // namespace popl

#endif



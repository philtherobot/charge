#include "compiler.hpp"

#include "process.hpp"
#include "tools.hpp"

#include <iostream>

#include <boost/range/algorithm/find.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>


using namespace std::string_literals;


namespace charge
{


namespace
{

boost::optional<std::string> consume_line(std::string & buf_inout)
{
	auto eol_iterator = boost::range::find(buf_inout, '\n');
	if (eol_iterator == buf_inout.end())
	{
		return boost::optional<std::string>();
	}

	auto eol_pos = std::distance(buf_inout.begin(), eol_iterator);
	auto after_eol_pos = eol_pos + 1;

	std::string line = buf_inout.substr(0, after_eol_pos);

	buf_inout.erase(0, after_eol_pos);

	return line;
}


class InclusionNotesSniffer : public ReadableStream
{
public:

	explicit InclusionNotesSniffer(ReadableStream & source);

	FileList inclusions_;

	virtual boost::optional<std::string> read();

private:
	std::string extract_output_from_accumulator();
	void note(std::string const & line);
		
	ReadableStream & source_;

	std::string const note_inclusion_;
	std::string::size_type const note_inclusion_end_pos_;
	std::string accumulator_;
};


InclusionNotesSniffer::InclusionNotesSniffer(ReadableStream & source)
:
	source_(source),
	note_inclusion_("Note: including file:"),
	note_inclusion_end_pos_(note_inclusion_.size())
{}


boost::optional<std::string> InclusionNotesSniffer::read()
{
	for (;;)
	{

		auto new_block = source_.read();

		if (!new_block)
		{
			if (accumulator_.empty())
			{
				return boost::optional<std::string>();
			}

			std::string output;
			swap(output, accumulator_);
			return output;
		}

		// We are guaranteed to receive at least one character.
	    assert(! (*new_block).empty());
		accumulator_ += *new_block;

		auto output = extract_output_from_accumulator();

		if (!output.empty()) return output;
		// else, wait for more data.

	}
}


std::string InclusionNotesSniffer::extract_output_from_accumulator()
{
	std::string output;

	for (;;)
	{
		auto line = consume_line(accumulator_);
		if (!line) break;

		using boost::algorithm::starts_with;

		if (starts_with(*line, note_inclusion_))
		{
			note(*line);
		}
		else
		{
			output += *line;
		}
	}

	return output;
}


void InclusionNotesSniffer::note(std::string const & line)
{
	auto remaining_size = line.size() - note_inclusion_end_pos_;

	auto raw_header_fn = line.substr(note_inclusion_end_pos_, remaining_size);

	using boost::algorithm::trim_copy;
	auto header_fn = trim_copy(raw_header_fn);

	inclusions_.push_back(header_fn);
}


class FirstLineFilter : public ReadableStream
{
public:
	explicit FirstLineFilter(ReadableStream & source);

	virtual boost::optional<std::string> read();

private:
	ReadableStream & source_;
	std::string accumulator_;
	bool first_line_skipped_;
};


FirstLineFilter::FirstLineFilter(ReadableStream & source)
: source_(source), first_line_skipped_(false)
{}


boost::optional<std::string> FirstLineFilter::read()
{
	for (;;)
	{
		auto new_block = source_.read();

		if (first_line_skipped_)
		{
			return new_block;
		}

		accumulator_ += *new_block;

		auto line = consume_line(accumulator_);

		if (line)
		{
			first_line_skipped_ = true;

			if (!accumulator_.empty())
			{
				std::string output;
				swap(output, accumulator_);
				return output;
			}
		}
	}
}


} // anonymous


YAML::Node configure()
{
    // TODO
    YAML::Node conf;

    conf["compiler"] = "/usr/bin/g++";
    conf["version"] = "7.2.0";
    conf["family"] = "g++";

    return conf;
}


Compiler::Compiler(YAML::Node const & conf)
    : config_(conf)
{}


FileList Compiler::compile(Arguments const & args)
{
    auto family = config_["family"].as<std::string>();

	std::string cmd;

    if (family == "msvc")
    {
		return compile_with_msvc(args);
    }
    else
    {
        throw UnsupportedFamilyError(family);
    }

	ShellProcess proc;

	proc.start(cmd);

	while (auto res = proc.child_stdout_->read())
	{
		std::cout << *res;
	}

	return FileList();
}


std::string Compiler::msvc_command_line(Arguments const & args) const
{
	auto s = args.executable_output_fn_.stem(); // "fn.exe" -> "fn"
	s += ".obj";
	auto p = args.executable_output_fn_.parent_path() / s;

	auto cl_cmd = config_["command"].as<std::string>();

	StringList words{
		quote_if_needed(cl_cmd),
		"/nologo",
		"/TP",
		"/MT",
		"/showIncludes",
		"/EHsc",
		quote_if_needed("/Fe:" + args.executable_output_fn_.string()),
		quote_if_needed("/Fo:" + p.string()),
		quote_if_needed(args.source_.string())
	};

	return boost::algorithm::join(words, " ");
}


FileList Compiler::compile_with_msvc(Arguments const & args) const
{
	auto cmd = msvc_command_line(args);

	ShellProcess proc;

	proc.start(cmd);

	InclusionNotesSniffer sniffer( *proc.child_stdout_ );

	FirstLineFilter filter(sniffer);

	while (auto res = filter.read())
	{
		std::cout << *res;
	}

	return sniffer.inclusions_;
}


UnsupportedFamilyError::UnsupportedFamilyError(std::string const & family)
:
    Exception("unsupported compiler family "s + family),
    family_(family)
{}

std::string const & UnsupportedFamilyError::family() const
{
    return family_;
}

}

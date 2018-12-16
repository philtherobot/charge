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

class InclusionNotesSniffer
{
public:

	InclusionNotesSniffer();

	FileList inclusions_;

	void operator() (std::string const & new_block);

private:
	std::string consume_line(std::string::iterator eol_iterator);
	void note(std::string const & line);
		
	std::string const note_inclusion_;
	std::string::size_type const note_inclusion_end_pos_;
	std::string left_over_;
};


InclusionNotesSniffer::InclusionNotesSniffer()
:
	note_inclusion_("Note: including file:"),
	note_inclusion_end_pos_(note_inclusion_.size())
{}


void InclusionNotesSniffer::operator() (std::string const & new_block)
{
	left_over_ += new_block;

	for (;;)
	{
		auto eol_iterator = boost::range::find(left_over_, '\n');
		if (eol_iterator == left_over_.end()) return;

		auto line = consume_line(eol_iterator);

		using boost::algorithm::starts_with;

		if (starts_with(line, note_inclusion_))
		{
			note(line);
		}
		else
		{
			std::cout << line;
		}
	}
}


std::string InclusionNotesSniffer::consume_line(std::string::iterator eol_iterator)
{
	auto eol_pos = std::distance(left_over_.begin(), eol_iterator);
	auto after_eol_pos = eol_pos + 1;

	std::string line = left_over_.substr(0, after_eol_pos);

	left_over_.erase(0, after_eol_pos);

	return line;
}


void InclusionNotesSniffer::note(std::string const & line)
{
	auto remaining_size = line.size() - note_inclusion_end_pos_;

	auto raw_header_fn = line.substr(note_inclusion_end_pos_, remaining_size);

	using boost::algorithm::trim_copy;
	auto header_fn = trim_copy(raw_header_fn);

	inclusions_.push_back(header_fn);
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

	InclusionNotesSniffer sniffer;

	while (auto res = proc.child_stdout_->read())
	{
		sniffer(*res);
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

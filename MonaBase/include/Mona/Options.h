/*
Copyright 2013 Mona - mathieu.poux[a]gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License received along this program for more
details (or else see http://www.gnu.org/licenses/).

This file is a part of Mona.
*/

#pragma once

#include "Mona/Mona.h"
#include "Mona/Option.h"
#include "Mona/Exceptions.h"
#include <set>

namespace Mona {

class Options : virtual Object {
public:
	typedef std::set<Option>::const_iterator Iterator;

	Options();
	virtual ~Options();

	template <typename ...Args>
	Option& add(Exception& ex, const char* fullName, const char* shortName, const Args&... args) {
		if (std::strlen(fullName)==0) {
			ex.set(Exception::OPTION, "Invalid option (fullName is empty)");
			return _OptionEmpty;
		}
		if (std::strlen(shortName) == 0) {
			ex.set(Exception::OPTION, "Invalid option (shortName is empty)");
			return _OptionEmpty;
		}
		auto result = _options.emplace(fullName, shortName, args ...);
		if (!result.second) {
			ex.set(Exception::OPTION, "Option ", fullName, " (", shortName, ") duplicated");
			return _OptionEmpty;
		}
		return const_cast<Option&>(*result.first);
	}

	void			remove(const std::string& name) { _options.erase(Option(name.c_str(), "")); }

	const Option&	get(Exception& ex, const std::string& name) const;

	Iterator		begin() const { return _options.begin(); }
	Iterator		end() const { return _options.end(); }


	bool			process(Exception& ex, int argc, char* argv[], const std::function<void(Exception& ex, const std::string&, const std::string&)>& handler = nullptr);

private:
	bool			process(Exception& ex, const std::string& argument, std::string& name, std::string& value, std::set<std::string>& alreadyReaden);
	void			handleOption(const std::string& name,const std::string& value) {}
	
	std::set<Option>	_options;
	const Option* 		_pOption;

	static Option		_OptionEmpty;
};



} // namespace Mona
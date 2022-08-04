#include "Stringifier.h"
#include <stack>


namespace eon
{
	std::vector<string> Stringifier::generateLines() const
	{
		std::vector<string> output;

		// We have the hard lines in out 'Lines' attribute.
		// We need to identify lines that are too long and split these.
		for( auto& element_line : Lines )
		{
			auto cur = element_line.Elements.begin();
			auto indentation = element_line.Indentation;
			stringify::Line line( indentation );
			bool have_split_block = false;

			// Keep going until we have split the line enough to not overflow
			while( cur != element_line.Elements.end() )
			{
				auto& elm = **cur;

				// Find the element that overflows the line
				auto end = _fillLine( element_line.Elements, cur, indentation );

				// If we have something ...
				bool have_something = false;
				if( end != cur )
				{
					for( auto i = cur; !have_something && i != end; ++i )
					{
						if( !( *i )->Value.empty() || elm.doubleq() || elm.singleq() )
							have_something = true;
					}
				}
				if( have_something )
				{
					if( end != element_line.Elements.end() && end != cur )
						end = _findSplit( cur, end );
					_output( cur, end, output, indentation );
					if( end != element_line.Elements.end() )
					{
						auto& e = **end;
						if( e.isSplitPlain() || e.isSplitBlock() )
						{
							cur = end;
							++cur;
							if( e.isSplitBlock() && !have_split_block )
							{
								have_split_block = true;
								++indentation;
							}
							continue;
						}
					}
					if( !have_split_block && cur == element_line.Elements.begin() )
					{
						have_split_block = true;
						++indentation;
					}
					cur = end;
				}

				// If we have nothing before but a single or double quoted element now, do a hard line split
				else if( end != element_line.Elements.end() && ( (*end)->doubleq() || (*end)->singleq() ) )
				{
					_splitHard( **end, output, indentation );
					cur = end;
					++cur;
				}
				else
					break;
			}
		}

		return output;
	}



	void Stringifier::_add( stringify::Type type, string&& value, stringify::Type before )
	{
		if( before != stringify::Type::none )
		{
			for( index_t lnum = CurLine + 1; lnum > 0; --lnum )
			{
				if( lnum - 1 < Lines.size() )
				{
					auto& elements = Lines[ lnum - 1 ].Elements;
					index_t i = 0;
					for( i = elements.size(); i > 0; --i )
					{
						if( elements[ i - 1 ]->ElmType == before )
							continue;
						elements.insert( elements.begin() + i,
							stringify::ElementPtr( new stringify::Element( type, std::move( value ) ) ) );
						return;
					}
				}
			}
		}
		if( Lines.size() == CurLine )
			Lines.push_back( stringify::Line( Indentation ) );
		Lines[ CurLine ].push_back( stringify::Element( type, std::move( value ) ) );
	}

	std::vector<stringify::ElementPtr>::const_iterator Stringifier::_fillLine(
		const std::vector<stringify::ElementPtr>& line,
		std::vector<stringify::ElementPtr>::const_iterator probe, index_t indentation ) const noexcept
	{
		index_t linew{ 0 };
		stringify::Type prev_type{ stringify::Type::none };
		for( ; probe != line.end(); ++probe )
		{
			auto& elm = **probe;
			index_t new_w{ elm.Value.numChars() };
			if( prev_type == stringify::Type::none && indentation > 0 )
				new_w += indentation * 2;
			if( Configuration.spaceBetween( prev_type, elm.ElmType ) )
				++new_w;
			if( elm.doubleq() || elm.singleq() )
				new_w += 2;
			if( linew + new_w > Configuration.SoftLineWidth )
				break;
			else
				linew += new_w;
			if( !( elm.isSplitBlock() || elm.isSplitPlain() ) )
				prev_type = elm.ElmType;
		}
		return probe;
	}
	std::vector<stringify::ElementPtr>::const_iterator Stringifier::_findSplit(
		std::vector<stringify::ElementPtr>::const_iterator begin,
		std::vector<stringify::ElementPtr>::const_iterator end ) const noexcept
	{
		// Check primary first, then secondary, then tertiary
		auto found = _findSplit( stringify::Type::prim_split, stringify::Type::prim_split_block, begin, end );
		if( found != end )
			return found;
		found = _findSplit( stringify::Type::sec_split, stringify::Type::sec_split_block, begin, end );
		if( found != end )
			return found;
		found = _findSplit( stringify::Type::tert_split, stringify::Type::tert_split_block, begin, end );
		if( found != end )
			return found;
		return end;
	}
	std::vector<stringify::ElementPtr>::const_iterator Stringifier::_findSplit(
		stringify::Type type, stringify::Type type_block,
		std::vector<stringify::ElementPtr>::const_iterator begin,
		std::vector<stringify::ElementPtr>::const_iterator end ) const noexcept
	{
		auto cur = end;
		for( --cur; cur != begin; --cur )
		{
			auto& elm = **cur;
			if( elm.ElmType == type || elm.ElmType == type_block )
				return cur;
		}
		return end;
	}

	void Stringifier::_splitHard( const stringify::Element& value, std::vector<string>& output, index_t indentation ) const
	{
		index_t total = 0;
		auto beg = value.Value.begin();
		while( beg != value.Value.end() )
		{
			string line( indentation * 2, ' ' );
			index_t elm_w = indentation * 2;
			if( beg == value.Value.begin() )
			{
				line += value.doubleq() ? "\"" : "'";
				++elm_w;
			}
			index_t remaining = value.Value.numChars() - total;
			if( remaining < Configuration.HardLineWidth )
			{
				line << value.Value.substr( beg );
				line += value.doubleq() ? "\"" : "'";
				output.push_back( std::move( line ) );
				break;
			}
			auto num_chars = Configuration.HardLineWidth - 1 - elm_w;
			auto end = beg + num_chars;
			auto split_point = _findStringSplitPoint( beg, end );
			line << value.Value.substr( beg, split_point );
			line += "\\";
			output.push_back( std::move( line ) );
			if( beg == value.Value.begin() )
				++indentation;
			total += split_point - beg;
			beg = split_point;
		}
	}
	eon::string::iterator Stringifier::_findStringSplitPoint( eon::string::iterator beg, eon::string::iterator end ) const
	{
		static string split_after{ " .,;:)]}>-/" };
		static string split_before{ "\t([{<" };
		for( auto sp = end - 1; sp != beg; --sp )
		{
			if( split_after.contains( *sp ) )
				return sp + 1;
			else if( sp.numChar() > beg.numChar() + 1 && split_before.contains( *sp ) )
				return sp;
		}
		return end;
	}
	void Stringifier::_output( std::vector<stringify::ElementPtr>::const_iterator begin,
		std::vector<stringify::ElementPtr>::const_iterator end, std::vector<string>& output, index_t indentation ) const
	{
		string line( indentation * 2, ' ' );
		stringify::Type prev_type{ stringify::Type::none };
		for( auto cur = begin; cur != end; ++cur )
		{
			auto& elm = **cur;
			if( Configuration.spaceBetween( prev_type, elm.ElmType ) )
				line += " ";
			if( elm.doubleq() )
				line += "\"" + elm.Value.escape() + "\"";
			else if( elm.singleq() )
				line += "\'" + elm.Value.escapeAll() + "\"";
			else
				line += elm.Value;
			if( !elm.isSplitPlain() && !elm.isSplitBlock() )
				prev_type = elm.ElmType;
		}
		if( line.numChars() > indentation * 2 )
			output.push_back( std::move( line ) );
	}
}

#include "Stringifier.h"
#include <stack>


namespace eon
{
	std::vector<string> Stringifier::generateLines() const
	{
		if( Blocks > 0 )
			throw MismatchedStartBlock();
		GeneratorData data( Lines );
		for( data.startLineIteration(); !data.atEndLine(); data.nextLine() )
			_processLine( data );
		return data.Output;
	}




	void Stringifier::GeneratorData::startLineIteration() noexcept
	{
		CurLine = Lines->begin();
		if( CurLine != Lines->end() )
		{
			CurElm = CurLine->Elements.begin();
			Indentation = CurLine->Indentation;
			Line = stringify::Line( Indentation );
			LongLineSplit = false;
		}
	}

	void Stringifier::GeneratorData::nextLine()
	{
		++CurLine;
		if( CurLine != Lines->end() )
		{
			CurElm = CurLine->Elements.begin();
			Indentation = CurLine->Indentation;
			Line = stringify::Line( Indentation );
			LongLineSplit = false;
		}
	}

	void Stringifier::_add( stringify::Type type, string&& value, stringify::Type before )
	{
		if( before != stringify::Type::none && _addBefore( type, value, before ) )
			return;
		if( Lines.size() == CurLine )
			Lines.emplace_back( Indentation );
		Lines[ CurLine ].push_back( stringify::Element( type, std::move( value ) ) );
	}

	bool Stringifier::_addBefore( stringify::Type type, string& value, stringify::Type before = stringify::Type::none )
	{
		for( index_t lnum = CurLine + 1; lnum > 0; --lnum )
		{
			if( lnum - 1 < Lines.size() && _addBefore( type, value, before, lnum - 1 ) )
				return true;
		}
		return false;
	}
	bool Stringifier::_addBefore( stringify::Type type, string& value, stringify::Type before, index_t pos )
	{
		auto& elements = Lines[ pos ].Elements;
		for( index_t i = elements.size(); i > 0; --i )
		{
			if( elements[ i - 1 ]->ElmType == before )
				continue;
			elements.insert( elements.begin() + i, std::make_shared<stringify::Element>( type, std::move( value ) ) );
			return true;
		}
		return false;
	}


	void Stringifier::_processLine( GeneratorData& data ) const
	{
		while( !data.atEndElement() )
		{
			auto end = _fillLine( data );
			if( _haveSomething( data, end ) )
				_processLineElements( data, end );

			// If we have nothing before but a single or double quoted element now, do a hard line split
			else if( end != data.endElement() && ( ( *end )->doubleq() || ( *end )->singleq() ) )
				_doHardLineSplit( data, end );
			else
				break;
		}
	}

	bool Stringifier::_haveSomething( GeneratorData& data, element_iterator end ) const
	{
		bool have_something = false;
		if( end != data.curElement() )
		{
			for( auto i = data.curElement(); !have_something && i != end; ++i )
			{
				if( !( *i )->Value.empty() || data.element().doubleq() || data.element().singleq() )
					have_something = true;
			}
		}
		return have_something;
	}

	void Stringifier::_doHardLineSplit( GeneratorData& data, element_iterator end ) const
	{
		_splitHard( **end, data );
		data.curElement( end + 1 );
	}

	void Stringifier::_processLineElements( GeneratorData& data, element_iterator end ) const
	{
		if( end != data.endElement() && end != data.curElement() )
			end = _findSplit( data.curElement(), end );
		_output( data, end );
		if( !data.isEndElement( end ) && _processLineSplit( data, end ) )
			return;
		if( !data.LongLineSplit && end != data.endElement() && data.curElement() == data.firstElement() )
		{
			data.LongLineSplit = true;
			++data.Indentation;
		}
		data.curElement( end );
	}

	bool Stringifier::_processLineSplit( GeneratorData& data, element_iterator end ) const
	{
		if( const auto& e = **end;
			e.isSplitPlain() || e.isSplitBlock() )
		{
			data.curElement( end );
			++data.curElement();
			if( e.isSplitBlock() && !data.LongLineSplit )
			{
				data.LongLineSplit = true;
				++data.Indentation;
			}
			return true;
		}
		return false;
	}

	Stringifier::element_iterator Stringifier::_fillLine( GeneratorData& data ) const noexcept
	{
		index_t linew{ 0 };
		stringify::Type prev_type{ stringify::Type::none };
		auto probe = data.curElement();
		for( ; probe != data.endElement(); ++probe )
		{
			const auto& elm = **probe;
			index_t new_w{ elm.Value.numChars() };
			if( prev_type == stringify::Type::none && data.Indentation > 0 )
				new_w += data.Indentation * 2;
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

	Stringifier::element_iterator Stringifier::_findSplit( element_iterator begin, element_iterator end ) const noexcept
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

	Stringifier::element_iterator Stringifier::_findSplit(
		stringify::Type type, stringify::Type type_block, element_iterator begin, element_iterator end ) const noexcept
	{
		auto cur = end;
		for( --cur; cur != begin; --cur )
		{
			const auto& elm = **cur;
			if( elm.ElmType == type || elm.ElmType == type_block )
				return cur;
		}
		return end;
	}

	void Stringifier::_splitHard( const stringify::Element& value, GeneratorData& data ) const
	{
		index_t total = 0;
		auto beg = value.Value.begin();
		while( beg != value.Value.end() )
		{
			string line( data.Indentation * 2, ' ' );
			index_t elm_w = data.Indentation * 2;
			if( beg == value.Value.begin() )
			{
				line += value.doubleq() ? "\"" : "'";
				++elm_w;
			}
			if( index_t remaining = value.Value.numChars() - total;
				remaining < Configuration.HardLineWidth )
			{
				line << value.Value.substr( beg );
				line += value.doubleq() ? "\"" : "'";
				data.Output.push_back( std::move( line ) );
				break;
			}
			auto num_chars = Configuration.HardLineWidth - 1 - elm_w;
			auto end = beg + num_chars;
			auto split_point = _findStringSplitPoint( beg, end );
			line << value.Value.substr( beg, split_point );
			line += "\\";
			data.Output.push_back( std::move( line ) );
			if( beg == value.Value.begin() )
				++data.Indentation;
			total += static_cast<index_t>( split_point - beg );
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

	void Stringifier::_output( GeneratorData& data, element_iterator end ) const
	{
		string line( data.Indentation * 2, ' ' );
		stringify::Type prev_type{ stringify::Type::none };
		for( auto cur = data.curElement(); cur != end; ++cur )
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
		if( line.numChars() > data.Indentation * 2 )
			data.Output.push_back( std::move( line ) );
	}
}

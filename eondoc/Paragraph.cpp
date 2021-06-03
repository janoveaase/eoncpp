#include "Paragraph.h"


namespace eon
{
	namespace doc
	{
		void paragraph::trimTrailing()
		{
			while( Sections.rbegin()->type() == sectiontype::newline
				|| Sections.rbegin()->type() == sectiontype::space )
				Sections.erase( Sections.end() - 1 );
		}




		string paragraph::str( size_t max_line_len, size_t indent ) const
		{
			string indt( indent * 2, SpaceChr );
			std::vector<string> lines{ string() };
			for( size_t i = 0; i < Sections.size(); ++i )
			{
				auto& sect = Sections[ i ];
				switch( sect.type() )
				{
					case sectiontype::text:
						if( lines[ lines.size() - 1 ].numChars() + sect.text().numChars() > max_line_len )
							lines.push_back( indt + sect.text() );
						else
							lines[ lines.size() - 1 ] += sect.text();
						break;
					case sectiontype::emphasized:
						if( lines[ lines.size() - 1 ].numChars() + sect.text().numChars() + 2 > max_line_len )
							lines.push_back( indt + "*" + sect.text() + "*" );
						else
							lines[ lines.size() - 1 ] += "*" + sect.text() + "*";
						break;
					case sectiontype::link:
						if( lines[ lines.size() - 1 ].numChars() + sect.text().numChars() + 3 > max_line_len )
							lines.push_back( indt + "@[" + sect.text() + "]" );
						else
							lines[ lines.size() - 1 ] += "@[" + sect.text() + "]";
						break;
					case sectiontype::space:
					{
						size_t size = 1;
						switch( Sections[ i + 1 ].type() )
						{
							case sectiontype::text:
								size += Sections[ i + 1 ].text().numChars();
								break;
							case sectiontype::emphasized:
								size += Sections[ i + 1 ].text().numChars() + 2;
								break;
							case sectiontype::link:
								size += Sections[ i + 1 ].text().numChars() + 3;
								break;
							default:
								size = 0;
						}
						if( size > 0
							&& lines[ lines.size() - 1 ].numChars()
							+ size <= max_line_len )
							lines[ lines.size() - 1 ] += " ";
						else
							lines.push_back( indt );
						break;
					}
					case sectiontype::newline:
						lines.push_back( string( indt + "  " ) );
						break;
				}
			}
			return anchor( "", "\n" ) + string( "\n" ).join( lines ) + "\n";
		}
	}
}

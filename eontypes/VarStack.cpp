#include "VarStack.h"


namespace eon
{
	namespace tup
	{
		Frame::Frame( const FrameDecl& decl )
		{
			Decl = &decl;
			Data = new byte_t[ decl.stackSize() ];
			auto addr = Data;
			for( size_t i = 0; i < decl.types().size(); ++i )
			{
				decl.types()[ i ]->instantiate( nullptr, addr );
				Names[ decl.names()[ i ] ] = (Object*)addr;
				addr += decl.types()[ i ]->stackSize();
			}
		}
		Frame::~Frame()
		{
			if( Data )
			{
				// Need to call destructors on all attributes
				for( auto& elm : Names )
					elm.second->callDestructor();

				// Now we can free the memory used
				delete[] Data;
			}
		}
	}
}

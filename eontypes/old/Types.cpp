#include "Types.h"


namespace eon
{
	namespace tup
	{
		Tuple::Tuple( std::vector<Attribute>& attributes, byte_t* memory_start )
		{
			// Allocate stack space for all the attributes
			pos_t stack_memory{ 0 };
			for( auto& attribute : attributes )
				stack_memory += attribute.Type->stackSize();
			HeapMemory = memory_start == nullptr;
			if( HeapMemory )
				Memory = new byte_t[ stack_memory ];
			else
				Memory = new( memory_start ) byte_t[ stack_memory ];

			// Create each attribute in the allocated stack space
			auto mem_start = Memory;
			for( auto& attribute : attributes )
			{
				auto instance = attribute.Type->instantiate( attribute.Value, mem_start );
				if( attribute.Name != no_name )
					NamedAttribs[ attribute.Name ] = std::make_pair( Attributes.size(), instance );
				mem_start += attribute.Type->stackSize();
				Attributes.push_back( instance );
			}
		}
		Tuple::Tuple( const Tuple& other, byte_t* memory_start )
		{
			// Allocate stack space for all the attributes
			pos_t stack_memory{ 0 };
			for( auto& attribute : other.Attributes )
				stack_memory += attribute->type()->stackSize();
			HeapMemory = memory_start == nullptr;
			if( HeapMemory )
				Memory = new byte_t[ stack_memory ];
			else
				Memory = new( memory_start ) byte_t[ stack_memory ];

			// Create each attribute in the allocated stack space
			auto mem_start = Memory;
			for( auto& attribute : other.Attributes )
			{
				auto instance = attribute->type()->instantiate( attribute, mem_start );
				mem_start += attribute->type()->stackSize();
				Attributes.push_back( instance );
			}
			for( auto& named : other.NamedAttribs )
				NamedAttribs[ named.first ] = named.second;
		}
		Tuple::~Tuple()
		{
			// Call the destructor of each attribute
			for( auto instance : Attributes )
				instance->callDestructor();

			// Delete memory if on the heap
			if( HeapMemory )
				delete[] Memory;
		}

		int Tuple::compare( const Tuple& other ) const noexcept
		{
			auto i = Attributes.begin(), j = other.Attributes.begin();
			for( ; i != Attributes.end() && j != other.Attributes.end(); ++i, ++j )
			{
				auto cmp = ( *i )->compare( **j );
				if( cmp != 0 )
					return cmp;
			}
			return j != other.Attributes.end() ? -1 : i != Attributes.end() ? 1 : 0;
		}

		string Tuple::str() const
		{
			string s;
			for( auto instance : Attributes )
			{
				if( !s.empty() )
					s += ", ";
				s += instance->str();
			}
			return s;
		}
		pos_t Tuple::stackSize() const noexcept
		{
			pos_t stack_memory{ 0 };
			for( auto instance : Attributes )
				stack_memory += instance->type()->stackSize();
			return stack_memory;
		}
	}
}

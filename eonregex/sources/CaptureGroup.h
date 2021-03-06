#pragma once

#include "../NodeGroup.h"


///////////////////////////////////////////////////////////////////////////////
//
// The 'eon' namespace encloses all public functionality
//
namespace eon
{
	///////////////////////////////////////////////////////////////////////////
	//
	// The 'eon::rx' namespace enclosed special elements for Eon regular
	// expressions
	//
	namespace rx
	{
		// Capture group node
		// Captures matched group
		class CaptureGroup : public NodeGroup
		{
		public:
			inline CaptureGroup( const substring& source ) : NodeGroup( source ) {
				Type = NodeType::capt_group; };
			inline CaptureGroup( const CaptureGroup& other ) { *this = other; }
			inline CaptureGroup( CaptureGroup&& other ) noexcept { *this = std::move( other ); }
			inline CaptureGroup( name_t name, NodeGroup* group, const substring& source )
				noexcept : NodeGroup( std::move( *group ) ) {
				Name = name; Source = source; Type = NodeType::capt_group; delete group; }
			virtual ~CaptureGroup() = default;

			inline Node* copy() const override { return new CaptureGroup( *this ); }

			inline CaptureGroup& operator=( const CaptureGroup& other ) {
				*static_cast<NodeGroup*>( this ) = other; Name = other.Name; return *this; }
			inline CaptureGroup& operator=( CaptureGroup&& other ) noexcept {
				*static_cast<NodeGroup*>( this ) = std::move( other ); Name = std::move( other.Name ); return *this; }

		private:
			bool _match( RxData& data, index_t steps ) override;

			inline string _strStruct() const override { return "@<" + str( Name ) + ">" + NodeGroup::_strStruct(); }

			inline bool _equal( const Node& other, cmpflag flags ) const noexcept override {
				return NodeGroup::_equal( other, flags ) && Name == dynamic_cast<const CaptureGroup*>( &other )->Name; }

			inline Node* _removeSuperfluousGroups() noexcept override {
				if( Next ) Next = Next->_removeSuperfluousGroups(); return this; }
			inline void _capture( RxData& data ) override {
				data.registerCapture( Name, substring( Start, data.pos() ) ); Captured = true; }

		private:
			name_t Name{ no_name };
			string::iterator Start;
			bool Captured{ false };
		};
	}
}

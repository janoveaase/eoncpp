#pragma once

#include "NodeGroup.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::rx' namespace enclosed special elements for Eon regular
	  expressions
	**************************************************************************/
	namespace rx
	{
		/*
		* Capture group node
		* Captures matched group
		*/
		class CaptureGroup : public NodeGroup
		{
		public:
			CaptureGroup( const substring& source ) : NodeGroup( source ) {
				Type = NodeType::capt_group; };
			CaptureGroup( const CaptureGroup& other ) { *this = other; }
			CaptureGroup( CaptureGroup&& other ) noexcept {
				*this = std::move( other ); }
			CaptureGroup( name_t name, NodeGroup* group,
				const substring& source ) noexcept
				: NodeGroup( std::move( *group ) ) { Name = name;
				Source = source; Type = NodeType::capt_group; delete group; }
			virtual ~CaptureGroup() = default;

			inline Node* copy() const override {
				return new NodeGroup( *this ); }

			inline CaptureGroup& operator=( const CaptureGroup& other ) {
				*static_cast<Node*>( this ) = other; Name = other.Name;
				return *this; }
			inline CaptureGroup& operator=( CaptureGroup&& other ) noexcept {
				*static_cast<Node*>( this ) = std::move( other );
				Name = std::move( other.Name ); return *this; }

		private:
			bool _match( RxData& data, size_t steps ) override;

		private:
			name_t Name{ no_name };
		};
	}
}

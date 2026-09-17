// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Cast.h>
#include <string>
#include <memory>

namespace qxml
{
	class Attribute
	{
	public:
		typedef std::shared_ptr< Attribute > shared_ptr;

		Attribute();
		explicit Attribute( std::string attribute );
		Attribute( std::string name, std::string value );
		~Attribute();
		std::string GetName() const; 
		std::string GetString() const;

		template< typename T >
		T Get() const
		{
			auto value = unify::FromString< T >( m_value );
			return !value ? T{} : *value;
		}

	protected:
		std::string m_name;
		std::string m_value;
	};
} // namespace qxml
// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <qxml/Attribute.h>
#include <unify/String.h>

using namespace qxml;

Attribute::Attribute()
{
}

Attribute::Attribute( std::string attribute )
{
	m_name = unify::String::ListPart( attribute, {'='}, 0 );
	m_value = unify::String::ListPart( attribute, {'='}, 1 );
	
	if( unify::String::LeftString( m_value, 1 ) == "\"" )
	{
		m_value = m_value.substr( 1, m_value.length() - 1 );
	}

	if( unify::String::RightString( m_value, 1 ) == "\"" )
	{
		m_value = m_value.substr( 0, m_value.length() - 1 );
	}
}

Attribute::Attribute( std::string name, std::string value )
: m_name( name )
, m_value( value )
{
}

Attribute::~Attribute()
{
	m_name.clear();
	m_value.clear();
}

std::string Attribute::GetName() const
{
	return m_name;
}

std::string Attribute::GetString() const
{
	return m_value;
}

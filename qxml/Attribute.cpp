// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <qxml/Attribute.h>
#include <unify/String.h>

using namespace qxml;

Attribute::Attribute()
{
}

Attribute::Attribute( std::string attribute )
{
	m_name = unify::string::ListPart( attribute, {'='}, 0 );
	m_value = unify::string::ListPart( attribute, {'='}, 1 );
	
	if( unify::string::LeftString( m_value, 1 ) == "\"" )
	{
		m_value = m_value.substr( 1, m_value.length() - 1 );
	}

	if( unify::string::RightString( m_value, 1 ) == "\"" )
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

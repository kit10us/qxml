// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

template< typename T >
T Element::GetAttribute( std::string attribute ) const
{
	return GetAttribute( attribute ).get()->Get< T >();
}

template< typename T >
T Element::GetAttributeElse( std::string attribute, T value ) const
{
	if( HasAttributes( attribute ) )
	{
		T out = GetAttribute( attribute ).get()->Get< T >();
		return out;
	}
	else
	{
		return value;
	}
}

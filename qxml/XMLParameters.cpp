// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <qxml/XMLParameters.h>

using namespace qxml;

bool XMLParameters::CaseInsensitiveLessThanTest::operator() ( std::string string1, std::string string2 ) const
{
    return _stricmp( string1.c_str(), string2.c_str() ) < 0;
}

XMLParameters::XMLParameters()
{
}

XMLParameters::~XMLParameters()
{
}

void XMLParameters::AddConvertor( const std::string name, INodeConvertor::shared_ptr convertor )
{
	m_convertors[ name ] = convertor;
}

bool XMLParameters::Convert( const qxml::Element * element, unify::Parameters & parameters )
{
	ConvertorMap::const_iterator itr = m_convertors.find( element->GetName() );

	if ( itr == m_convertors.end() )
	{
		return false;
	}

	return (*itr->second)( element, parameters );
}

size_t XMLParameters::NumberOfConvertors() const
{
	return m_convertors.size();
}

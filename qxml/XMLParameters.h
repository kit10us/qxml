// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Parameters.h>
#include <qxml/Element.h>
#include <memory>
#include <map>
#include <string>

namespace qxml
{
	class XMLParameters
	{
	public:
		struct INodeConvertor
		{
			typedef std::shared_ptr< INodeConvertor > shared_ptr;
			virtual ~INodeConvertor() {}

			// Converts an element node into a parameters entry.
			// Returns true on success.
			virtual bool operator()( const qxml::Element * element, unify::Parameters & parameters ) = 0;
		};

		XMLParameters();
		~XMLParameters();

		void AddConvertor( const std::string name, INodeConvertor::shared_ptr convertor );
		
		// Convert a single XML node to a parameters entry.
		bool Convert( const qxml::Element * element, unify::Parameters & parameters );

		size_t NumberOfConvertors() const;

	private:
        class CaseInsensitiveLessThanTest
        {
        public:
            bool operator() (std::string string1, std::string string2) const;
        };  
        typedef std::map< std::string, INodeConvertor::shared_ptr, CaseInsensitiveLessThanTest > ConvertorMap;
		ConvertorMap m_convertors;
	};
}
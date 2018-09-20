// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#pragma comment(lib,"qxml.lib")

#include <qxml/Attribute.h>
#include <qxml/Element.h>
#include <unify/Path.h>
#include <unify/FStream.h>
#include <unify/Exception.h>
#include <unify/unify.h>
#include <vector>
#include <string>

namespace qxml
{
	class Document
	{
	public:
		Document();
		Document( unify::Path filePath );
		virtual ~Document();
		virtual void Load( unify::Path filePath );
		void Destroy();
		Element * GetRoot();
		const Element * GetRoot() const;

		Element * GetElement( unsigned int element );
		Element * FindElement( std::string element );
		Element * FindElement( std::string element, std::string attribute, std::string value );

        const unify::Path & GetPath() const;

		Element * AddElement( Element * element );

	protected:

		 unify::Path m_filePath;

		 // Elements stored as a list internally, however they keep linkage hierarchicly.
		ElementList m_elementList;
		Element * m_xml; // The XML header node.
		Element * m_root;
	};
}


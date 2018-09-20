// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <qxml/Element.h>

namespace qxml
{
	class XPath
	{
	public:
		XPath();
		XPath( std::string path );
		~XPath();

		std::string GetPath() const;

		bool Test( const qxml::Element & element ) const;

	private:
		std::string m_path;		
	};
}
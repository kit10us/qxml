// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <qxml/Document.h>
#include <unify/FStream.h>
#include <unify/LinkList.h>
#include <unify/Path.h>
#include <string>

namespace qxml 
{
	#define MAX_ELEMENT_DEPTH		32

	class ExportDocument
	{
	public:
		ExportDocument( unify::Path filename, std::string indent = "   " );
		~ExportDocument();

		void Close();

		bool IsOpen() const;

		void Indent( unsigned int depth );

		void ExportDocument::WriteDoc( Document & doc );

	protected:
		void WriteElement( Element * element, int depth );
		
		unify::FileStream m_stream;
		unsigned int m_uDepth;
		std::string m_indent;
		std::string m_attributeIndent;
		std::string m_afterElement;
		unify::Path m_filename;

		bool m_bInElement;
		unsigned int m_uWritenAttributes;

		std::string m_elementNameTree[ MAX_ELEMENT_DEPTH ];
	};

}

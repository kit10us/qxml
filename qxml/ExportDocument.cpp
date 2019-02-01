// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <qxml/ExportDocument.h>
#include <unify/Exception.h>

using namespace qxml;

ExportDocument::ExportDocument( unify::Path filename, std::string indent )
	: m_indent{ indent }
	, m_attributeIndent{ " " }
	, m_afterElement{ "\n" }
	, m_uDepth( 0 )
	, m_bInElement( false )
	, m_uWritenAttributes( 0 )
{
	m_stream.Open( unify::StreamAccessType::STREAMACCESS_WRITETEXT, filename );

	// Leave xml version tag
	m_stream.Write( "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" );
}

ExportDocument::~ExportDocument()
{
	Close();
}

void ExportDocument::Close()
{
	m_stream.Close();
}

bool ExportDocument::IsOpen() const
{
	return m_stream.IsOpen();
}

void ExportDocument::Indent( unsigned int depth )
{
	for( depth; depth; depth-- )
	{
		m_stream.Write( m_indent );
	}
}

void ExportDocument::WriteDoc( Document & doc )
{
	WriteElement( doc.GetRoot(), 0 );
}

void ExportDocument::WriteElement( Element * element, int depth )
{
	Indent( depth );
	
	switch( element->GetType() )
	{
	case Element::NodeType::CDATA:
	{
		m_stream.Write( "<![CDATA[" );
		m_stream.Write( element->GetText() );
		m_stream.Write( "]]> " );
		m_stream.Write( m_afterElement );
	}
	break;
	{
	}
	break;
	case Element::NodeType::Comment:
	{
		m_stream.Write( "<!-- " );
		m_stream.Write( element->GetText() );
		m_stream.Write( " --> " );
		m_stream.Write( m_afterElement );
	}
	break;
	case Element::NodeType::ProcessingInstruction:
	case Element::NodeType::Element:
	{
		m_stream.Write( "<" );
		if( element->GetType() == Element::NodeType::ProcessingInstruction )
		{
			m_stream.Write( "?" );
		}

		m_stream.Write( element->GetName() );

		// Attributes...
		if( element->NumAttributes() )
		{
			for( auto attribute : element->Attributes() )
			{
				m_stream.Write( m_attributeIndent );
				m_stream.Write( attribute.GetName() + "=\"" + attribute.GetString() + "\"" );
			}
		}

		// Child nodes...
		if( element->NumChildren() )
		{
			m_stream.Write( ">" );
			m_stream.Write( m_afterElement );

			for( auto node : element->Children() )
			{
				WriteElement( &node, depth + 1 );
			}

			// Close node...
			Indent( depth );
			m_stream.Write( "</" + element->GetName() );
			if( element->GetType() == Element::NodeType::ProcessingInstruction )
			{
				m_stream.Write( "?" );
			}
			m_stream.Write( ">" );
		}
		else
		{
			// Close node...
			if( element->GetType() == Element::NodeType::ProcessingInstruction )
			{
				m_stream.Write( "?>" );
			}
			else
			{
				m_stream.Write( "/>" );
			}
		}

		m_stream.Write( m_afterElement );
	}
	break;
	}
}

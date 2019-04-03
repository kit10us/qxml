// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <qxml/Document.h>
#include <unify/String.h>

using namespace qxml;

Document::Document()
: m_xml( 0 )
, m_root( 0 )
{
}

Document::Document( unify::Path filePath )
: m_xml( 0 )
, m_root( 0 )
{
	Load( filePath );
}


Document::~Document()
{
	Destroy();
}

void Document::Load( unify::Path filePath )
{
	if( ! filePath.Exists() )
	{
		throw unify::Exception( filePath.ToString() + ": XML File not found!" );
	}

	unify::FileStream stream;
	char cChar;	
	bool bInElement = false;
	bool bInComment = false;
	std::string sElementName;
	unsigned int uAttribute = 0;
	Attribute * pAttribute = 0;
	Element * pElement = 0;

	// Destroy existing document
	Destroy();

	stream.Open( unify::StreamAccessType::STREAMACCESS_READ, filePath );

	m_filePath = filePath;

	Element * pParent {};
	size_t parent_line {};

	// Loop until we find an element, all else is an error (except cr, nl, spaces, tabs)
	std::string data;
	size_t data_line {};

	char cQuotes = ' ';
	bool bInCData = false;
	size_t line = 0;
	while( ! stream.EndOfStream() )
	{
		stream.Read( &cChar, 1 );
		if ( cChar == '\n' )
		{
			line++;
		}

		if ( bInCData ) {
			data += cChar;
			// End CDATA
			if ( unify::string::RightString( data, 3 ) == "]]>" ) {
				data = unify::string::LeftString( data, (unsigned int)data.length() - 3 );
				pParent->m_text += data;
				data = "";
				bInCData = false;
				continue;
			}
		} 
		else if( bInElement )
		{
			if ( cQuotes != ' ' ) // If we are in quotes (' ' means no quotes).
			{
				if ( cQuotes == cChar ) 
				{
					cQuotes = ' ';
				}
				data += cChar;
				continue;
			} 
			else if( cChar == '"' || cChar == '\'' ) 
			{
				data += cChar;
				cQuotes = cChar;
				continue;
			} 
			else if( unify::string::LeftString( data, 8 ) == "![CDATA[" ) 
			{
				bInElement = false;
				bInCData = true;
				data = "";
			} 
			else if( cChar == '>' ) 
			{
				bInElement = false;
				if( unify::string::LeftString( data, 3 ) == "!--" ) 
				{
					bInComment = true;
					if( unify::string::RightString( data, 2 ) == "--" ) {  
						data = unify::string::RightString( data, (unsigned int)data.length() - 1 );
						//printf( "COMMENT = \"%s\"\n", data.c_str() );
						pElement = AddElement( new Element( "COMMENT", Element::NodeType::Comment, this, line ) );
						pElement->m_data = data;
						pParent->TakeChild( pElement );
						data = "";
						bInComment = false;
						continue;
					} 
					else 
					{
						bInElement = true;
						data += cChar;
						continue;
					}	
				} 
				else 
				{
					// Determine if this is a close element...
					if( unify::string::LeftString( data, 1 ) == "/" ) 
					{
						data = unify::string::RightString( data, (unsigned int)data.length() - 1 );
						if( data != pParent->GetName() )
						{
							throw unify::Exception( "Line " + unify::Cast< std::string >( line ) + ": Mismatched end element in file \"" + filePath.ToString() + "\"! (end = " + data + "(" + unify::Cast< std::string >( data_line ) + ")" + ", open = " + pParent->GetName() + "(" + unify::Cast< std::string>( parent_line ) + ")" + ")!" );
						}
						pParent = pParent->GetParent();
						data = "";
						continue;
					}

					// Determine if this is a single element (no children)...
					bool bSingle = false;
					if( unify::string::RightString( data, 1 ) == "/" ) 
					{
						bSingle = true;
						data = unify::string::LeftString( data, (unsigned int)data.length() - 1 );
					}

					// Determine if this is a "command" element (single as well)...
					bool bCommand = false;
					if( unify::string::LeftString( data, 1 ) == "?" && unify::string::RightString( data, 1 ) == "?" ) 
					{
						bCommand = true;
						bSingle = true;
						data = data.substr( 1, data.length() - 2 );
					}

					Element::NodeType::TYPE type = bCommand ? Element::NodeType::ProcessingInstruction : Element::NodeType::Element  ;
					std::string tagName = unify::string::ListPart( data, {' ', '\t'}, 0 );
					pElement = AddElement( new Element( tagName, type, this, line ) );

					// Process out attributes...
					for( unsigned int i = 1; i < unify::string::ListPartCount( data, {' ', '\t'} ); i++ )
					{
						std::string attribute = unify::string::ListPart( data, {' ', '\t'}, i );
						if( attribute == "" ) continue;
						pElement->AddAttribute( Attribute::shared_ptr( new Attribute( attribute ) ) );
					}
					// Append as a child to the current parent...
					if( pParent == 0 )
					{
						pParent = pElement;
						parent_line = line;
					}
					else
					{
						pParent->TakeChild( pElement );
					}

					// Check if this is a standalone element, if so add it..
					if( bSingle == false )
					{
						// Not a standalone element, we become parent.
						pParent = pElement;
						parent_line = line;
					}
				}
				data = "";
				continue;
			}
			data += cChar;
			continue;
		} 
		else if( !bInComment && cChar == '<' )
		{
			if( pParent )
			{
				pParent->AddText( data );
			}
			data = "";
			data_line = line;
			bInElement = true;
			continue;
		}
		else
		{
			data += cChar;
		}
	}

	stream.Close();
}

void Document::Destroy()
{
	m_elementList.Clear();
	m_root = 0;
	m_filePath = unify::Path( "" );
}

Element * Document::GetRoot()
{
	return m_root;
}

const Element * Document::GetRoot() const
{
	return m_root;
}

Element * Document::GetElement( unsigned int uElement )
{
	return m_elementList.Item( uElement );
}

Element * Document::FindElement( std::string element )
{
	return m_elementList.Find( element );
}

// Find an element whose attribute is said value.
Element * Document::FindElement( std::string sElement, std::string attribute, std::string sValue )
{
	unify::Query< Element > query;
	Element * pElement = m_elementList.GotoFirst( &query );
	while( pElement )
	{
		if( pElement->IsTagName( sElement ) ) 
		{
			const Attribute::shared_ptr pAttribute = pElement->GetAttribute( attribute );
			if( pAttribute ) 
			{
				if( unify::string::StringIs( pAttribute->GetString(), sValue ) ) {
					return pElement;
				}
			}
		}
		pElement = m_elementList.GotoNext( &query );
	}

	return nullptr;
}

const unify::Path & Document::GetPath() const
{
    return m_filePath;
}

Element * Document::AddElement( Element * element )
{
	// Ensure we have a root set.
	if( m_xml == 0 && element->GetType() == Element::NodeType::ProcessingInstruction && element->IsTagName( "xml" ) )
	{
		m_xml = element;
	}
	else if( m_root == 0 && element->GetType() == Element::NodeType::Element )
	{
		m_root = element;
	}

    element->m_document = this;

	return m_elementList.AddItem( element, element->GetName() );
}

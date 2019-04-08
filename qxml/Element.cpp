// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <qxml/Element.h>
#include <qxml/Document.h>
#include <unify/String.h>
#include <unify/Exception.h>
#include <cassert>

using namespace qxml;

Element::Element()
: m_index( 0 )
, m_parent( 0 )
, m_prevSibling( 0 )
, m_nextSibling( 0 )
, m_firstChild( 0 )
, m_lastChild( 0 )
, m_numChildren( 0 )
, m_document( 0 )
, m_line( 0 )
{
}

Element::Element( std::string name, NodeType::TYPE type, Document * document, size_t line )
: m_tagName( name )
, m_index( 0 )
, m_type( type )
, m_parent( 0 )
, m_prevSibling( 0 )
, m_nextSibling( 0 )
, m_firstChild( 0 )
, m_lastChild( 0 )
, m_numChildren( 0 )
, m_document( document )
, m_line( 0 )
{
}

Element::~Element()
{
}

std::string Element::GetName() const
{
	return m_tagName;
}

bool Element::IsTagName( std::string tagName ) const
{
	return unify::string::StringIs( m_tagName, tagName );
}

unsigned int Element::NumAttributes() const
{
	return (unsigned int)m_attributeList.size();
}

bool Element::HasAttributes( std::string name ) const
{
	std::vector< std::string > names = unify::string::Split< std::string >( name, ',' );
    for( std::vector< std::string >::const_iterator itrTok = names.begin(); itrTok != names.end(); ++itrTok )
    {
        bool foundMatch = false;
		for( auto & itr : m_attributeList )
		{
            const Attribute & currentAttribute = *itr;
            std::string attributeToFindText = *itrTok;
            std::string left;
            std::string right;
            bool notLeft = false;
            bool notRight = false;

            // Attempt to find = outside of single or double quotes.
            size_t pos = 0;
            bool inQuotes = false;
            char quoteType;
            bool buildingLeft = true;
            for ( pos; pos != attributeToFindText.size(); ++pos )
            {
                if ( inQuotes && attributeToFindText[ pos ] == quoteType )
                {
                    inQuotes = false;
                    continue;
                }
                else if ( ! inQuotes && ( attributeToFindText.at( pos ) == '\'' || attributeToFindText.at( pos ) == '\"' ) )
                {
                    quoteType = attributeToFindText[ pos ];
                    inQuotes = true;
                    continue;
                }

                if ( ! inQuotes && attributeToFindText[ pos ] == '=' )
                {
                    assert( buildingLeft ); // We should not enconter an equals during building the r-param.
                    buildingLeft = false;
                    continue;
                }

                if ( ! inQuotes && attributeToFindText[ pos ] == '!' && ( buildingLeft && left.empty() || ! buildingLeft && right.empty() ) )
                {
                    if ( buildingLeft )
                    {
                        notLeft = true;
                    }
                    else
                    {
                        notRight = true;
                    }
                }

                if ( buildingLeft )
                {
                    left += attributeToFindText[ pos ];
                }
                else
                {
                    right += attributeToFindText[ pos ];
                }
            }

            if( unify::string::StringIs( left, currentAttribute.GetName() ) || notLeft )
            {
                if ( right.empty() || unify::string::StringIs( right, currentAttribute.GetString() ) || notRight ) 
			    {
                    foundMatch = true;
				    continue;
                }
            }
		}

        // We track a bool because out text input can be delimited.
        if ( ! foundMatch )
        {
            return false;
        }
	}	
	return true;
}

bool Element::HasElements( std::string name ) const
{
	std::vector< std::string > names = unify::string::Split< std::string >( name, ',' );
    for( std::vector< std::string >::const_iterator itrTok = names.begin(); itrTok != names.end(); ++itrTok )
    {
		bool found = false;
		const Element * pElement = GetFirstChild();
		while( pElement )
		{
			if( pElement->IsTagName( *itrTok ) ) 
			{
				found = true;
				break;
			}
			pElement = pElement->GetNext();
		}
		if( ! found )
		{
			return false;
		}
	}
	return true;
}

Attribute::shared_ptr Element::GetAttribute( unsigned int attribute ) const
{
	return m_attributeList[ attribute ];
}

Attribute::shared_ptr Element::GetAttribute( std::string attributeName ) const
{
	for( std::vector< Attribute::shared_ptr >::const_iterator itr = m_attributeList.begin(), end = m_attributeList.end(); itr != end; ++itr )
	{
		if( _stricmp( (*itr)->GetName().c_str(), attributeName.c_str() ) == 0 ) return (*itr);
	}

	throw unify::Exception( GetDocument()->GetPath().ToString() + ": Attribute \"" + attributeName + "\" was not found by name!" );
}

void Element::FindElements( std::list< const Element * > & elementList, const std::string tagName, std::string attributes ) const
{
	// Get the first child of our parent
	const Element * element = GetFirstChild();
	while( element )
	{
        if( element->IsTagName( tagName ) && element->HasAttributes( attributes ) ) 
        {
			elementList.push_back( element );
		}
		element = element->GetNext();
	}
}

void Element::FindElementsRecursive( std::list< const Element * > & elementList, const std::string tagName, std::string attributes ) const
{
	// Get the first child of our parent
	const Element * element = GetFirstChild();
	while( element )
	{
		if( element->IsTagName( tagName ) && element->HasAttributes( attributes ) )
		{
			elementList.push_back( element );
		}
		element->FindElementsRecursive( elementList, tagName, attributes );
		element = element->GetNext();
	}
}

const Element * Element::FindFirstElement( const std::string tagName, std::string attributes ) const
{
	// Get the first child of our parent
	const Element * element = GetFirstChild();
	while( element )
	{
        if( element->IsTagName( tagName ) && element->HasAttributes( attributes ) ) 
        {
            return element;
		}
		element = element->GetNext();
	}
    return nullptr;
}

// Search our children for elements with a given tag name...
Element * Element::GetElement( std::string tagName )
{
	// Get the first child of our parent
	Element* pElement = GetFirstChild();
	while( pElement )
	{
		if( pElement->IsTagName( tagName ) )
		{
			return pElement;
		}
		pElement = pElement->GetNext();
	}

	return 0;
}

const Element * Element::GetElement( std::string tagName ) const
{
	// Get the first child of our parent
	const Element * pElement = GetFirstChild();
	while( pElement )
	{
		if( pElement->IsTagName( tagName ) )
		{
			return pElement;
		}
		pElement = pElement->GetNext();
	}

	return 0;
}


Element * Element::GetParent()
{
	return m_parent;
}

const Element * Element::GetParent() const
{
	return m_parent;
}

Element * Element::GetPrevious()
{
	return m_prevSibling;
}

const Element * Element::GetPrevious() const
{
	return m_prevSibling;
}

Element * Element::GetNext()
{
	return m_nextSibling;
}

const Element * Element::GetNext() const
{
	return m_nextSibling;
}

Element * Element::GetFirstChild()
{
	return m_firstChild;
}

const Element * Element::GetFirstChild() const
{
	return m_firstChild;
}

Element * Element::GetLastChild()
{
	return m_lastChild;
}

const Element * Element::GetLastChild() const
{
	return m_lastChild;
}

// Returns the text for an element...
std::string Element::GetText() const
{
	return m_text;
}

unsigned int Element::NumChildren() const
{
	return m_numChildren;
}

unsigned int Element::Index() const
{
	return m_index;
}

// Assign a child to the linkage hierachy...
void Element::TakeChild( Element * pElement )
{
	pElement->m_parent = this;
	m_numChildren++;

	if( m_firstChild == 0 ) {
		m_firstChild = pElement;
	} 
	else {
		m_lastChild->m_nextSibling = pElement;
		pElement->m_prevSibling = m_lastChild;
	}

	// Last added child becomes last child alway...
	m_lastChild = pElement;
}

// Assign a sibling (insert) to the linkage hierachy...
void Element::TakeSibling( Element * pElement )
{
	pElement->m_parent = this->m_parent;
	pElement->m_prevSibling = this;

	if( this->m_nextSibling )
	{
		this->m_nextSibling->m_prevSibling = pElement;
		pElement->m_nextSibling = this->m_nextSibling;
	} else
	{
		this->m_parent->m_lastChild = pElement;
	}

	this->m_nextSibling = pElement;
}

void Element::AddAttribute( std::string name, std::string value )
{
	m_attributeList.push_back( Attribute::shared_ptr( new Attribute( name, value ) ) );
}

void Element::AddAttribute( Attribute::shared_ptr attribute )
{
	m_attributeList.push_back( attribute );
}

Element * Element::AddElement( std::string name, NodeType::TYPE type )
{
	auto * element = new Element( name, type, GetDocument() );
	TakeChild( element );
	return element;
}

Element * Element::AddCData( std::string text )
{
	auto * element = AddElement( "CDATA", NodeType::CDATA );
	element->AddText( text );
	return element;
}

std::string Element::AddText( std::string text )
{
	m_text += unify::string::CleanWhitespace( text );
	return m_text;
}

Element::NodeType::TYPE Element::GetType() const
{
	return m_type;
}

Document * Element::GetDocument() const
{
    return m_document;
}

size_t Element::GetLine() const
{
	return m_line;
}

size_t Element::AttributesToParameters( unify::Parameters & parameters, std::list< AttributeOption > attributes ) const
{
	size_t count {};
	for( auto attribute : attributes )
	{
		if ( HasAttributes( attribute.name ) )
		{
			parameters.Set< std::string >( attribute.name, GetAttribute( attribute.name )->GetString() );
			count++;		
		}
		else if ( attribute.hasDefaultValue )
		{
			parameters.Set( attribute.name, attribute.defaultValue );
		}
	}
	return count;
}


//////////////////////////////////////////////////////////
// ElementList
//////////////////////////////////////////////////////////

Element * ElementList::AddElement( Element * element )
{
	AddItem( element, element->GetName() );
	return element;
}

namespace unify
{
	template<>
	unify::Parameters Cast( const qxml::Element & element )
	{
		unify::Parameters parameters;
		for( size_t i = 0; i < element.NumAttributes(); ++i )
		{
			auto attribute = element.GetAttribute( i );
			parameters.Set( attribute->GetName(), attribute->GetString() );
		}
		return parameters;
	}
}
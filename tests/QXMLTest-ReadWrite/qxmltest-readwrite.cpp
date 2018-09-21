// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <qxml/ExportDocument.h>
#include <unify/test/Suite.h>
#include <iostream>

int main( char *, int )
{
	using namespace std;
	using namespace qxml;
	using namespace unify;

	test::Suite suite;

	suite.BeginSuite( "QXML" );
	{
		suite.BeginCase( "Remove test.xml" );
		{
			// Remove test.xml, if it exists.
			if (Path( "test.xml" ).Exists())
			{
				Path( "test.xml" ).Delete();
			}
			suite.Assert( "Delete test.xml", !Path( "test.xml" ).Exists() );
		}
		suite.EndCase();

		suite.BeginCase( "Write XML" );
		{
			Document doc;

			auto * root = new Element( "root", Element::NodeType::Element, &doc ) ;

			doc.AddElement( root );
		
			auto * first = root->AddElement( "first" );
		
			auto * second = root->AddElement( "second" );
			second->AddAttribute( "attr1", "1234" );

			auto * third = root->AddElement( "third" );
			third->AddAttribute( "attr1", "1234" );
			third->AddAttribute( "attr2", "Longer String" );
			auto * third_first = third->AddElement( "third_first", Element::NodeType::Element );
			third_first->AddAttribute( "attr1", "1234" );

			auto * fourth = root->AddCData( "Line 1\nLine 2\nLine 3 - with </node_like_text>" );

			ExportDocument docOut( unify::Path( "test.xml" ) );
			docOut.WriteDoc( doc );
			docOut.Close();
		}
		suite.EndCase();

		suite.BeginCase( "Read XML" );
		{
			Document docIn( Path( "test.xml" ) );
			suite.Assert( "Root element is valid", docIn.GetRoot() != nullptr );
			suite.Assert( "Root element is called \"root\"", docIn.GetRoot()->GetName() == "root" );
			suite.Assert( "First element is valid", docIn.GetRoot()->GetFirstChild() != nullptr );
			suite.Assert( "First element is called \"first\"", docIn.GetRoot()->GetFirstChild()->GetName() == "first" );
			suite.Assert( "Second element is valid", docIn.GetRoot()->GetFirstChild()->GetNext() != nullptr );
			suite.Assert( "Second element is called \"second\"", docIn.GetRoot()->GetFirstChild()->GetNext()->GetName() == "second" );

			std::list< const Element * > elements;
			docIn.GetRoot()->FindElements( elements, "third" );
			const Element * third = *elements.begin();
			suite.Assert( "Third element is found", third != nullptr );
			suite.Assert( "Third element is called \"third\"", third->GetName() == "third" );
			suite.Assert( "Third element has attribute called \"attr\"", third->HasAttributes( "attr2" ) );
			suite.Assert( "Third attribute has value of \"Longer String\"", third->GetAttribute( "attr2" )->GetString() == "Longer String" );
		}
		suite.EndCase();
	}

	std::cout << "Press enter to continue." << endl;
	std::cin.ignore();

	return 0;
}
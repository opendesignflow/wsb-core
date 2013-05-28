/**
 * simple_dom.cpp
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send 
 * a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.  
 *
 */

// Includes
//-----------------

//-- Std
#include <fstream>
#include <iostream>
#include <omp.h>

//-- Gtest
#include <gtest/gtest.h>

#include <vxml/vdom/VDOMRegistry.h>
#include <vxml/vdom/VDOMDocument.h>

/**
 * Just creates a simple document
 */
TEST(vdom, newdoc) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Create
    VDOM::VDOMDocument * document = libxml2impl->newDocument();
    ASSERT_TRUE(document!=NULL);



    ASSERT_NO_THROW(delete libxml2impl);


}

/**
 * Just parses a document
 */
TEST(vdom, parse) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><root></root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);



    ASSERT_NO_THROW(delete libxml2impl);
}

TEST(vdom, parse_error) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><root></oot>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document==NULL);



    ASSERT_NO_THROW(delete libxml2impl);
}

TEST(vdom, serialize) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><root></root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Serialize

    ASSERT_NO_THROW(delete libxml2impl);
}

TEST(vdom, document_element_no_ns) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><root></root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Get Document Element
    ASSERT_TRUE(document->getDocumentElement()!=NULL);
    ASSERT_EQ(document->getDocumentElement()->getLocalName(),"root");
    ASSERT_EQ(document->getDocumentElement()->getName(),"root");
    ASSERT_EQ(document->getDocumentElement()->getPrefix(),"");
    ASSERT_EQ(document->getDocumentElement()->getNamespaceURI(),"");

    ASSERT_NO_THROW(delete libxml2impl);

}

/**
 * Check that to same nodes return the same pointer instance
 */
TEST(vdom, node_pool) {


    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><r:root xmlns:r='http://regression' attr='test'></r:root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Document Element
    //-------------------------------
    VDOM::VDOMElement * documentElement = document->getDocumentElement();
    VDOM::VDOMElement * documentElement2 = document->getDocumentElement();
    ASSERT_EQ(documentElement,documentElement2);

    ASSERT_NO_THROW(delete libxml2impl);
    ASSERT_NO_THROW(delete document);


}

TEST(vdom, document_element_ns_no_prefix) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><root xmlns='http://regression'></root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Get Document Element
    ASSERT_TRUE(document->getDocumentElement()!=NULL);
    ASSERT_EQ(document->getDocumentElement()->getLocalName(),"root");
    ASSERT_EQ(document->getDocumentElement()->getName(),"root");
    ASSERT_EQ(document->getDocumentElement()->getPrefix(),"");
    ASSERT_EQ(document->getDocumentElement()->getNamespaceURI(),"http://regression");

    ASSERT_NO_THROW(delete libxml2impl);

}

TEST(vdom, document_element_ns_prefix) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><r:root xmlns:r='http://regression'></r:root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Get Document Element
    ASSERT_TRUE(document->getDocumentElement()!=NULL);
    ASSERT_EQ(document->getDocumentElement()->getLocalName(),"root");
    ASSERT_EQ(document->getDocumentElement()->getName(),"r:root");
    ASSERT_EQ(document->getDocumentElement()->getPrefix(),"r");
    ASSERT_EQ(document->getDocumentElement()->getNamespaceURI(),"http://regression");

    ASSERT_NO_THROW(delete libxml2impl);

}

TEST(vdom, document_element_attributes_text) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    string xml("<?xml version='1.0' encoding='UTF8'?><r:root xmlns:r='http://regression' attr='test'></r:root>");
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Get Document Element
    ASSERT_TRUE(document->getDocumentElement()!=NULL);
    ASSERT_EQ(document->getDocumentElement()->getLocalName(),"root");
    ASSERT_EQ(document->getDocumentElement()->getName(),"r:root");
    ASSERT_EQ(document->getDocumentElement()->getPrefix(),"r");
    ASSERT_EQ(document->getDocumentElement()->getNamespaceURI(),"http://regression");

    //-- Attributes
    ASSERT_TRUE(document->getDocumentElement()->hasAttribute("attr"));
    ASSERT_EQ(document->getDocumentElement()->getAttribute("attr"),"test");

    ASSERT_FALSE(document->getDocumentElement()->hasAttribute("attrNonExistent"));
    ASSERT_EQ(document->getDocumentElement()->getAttribute("attrNonExistent"),"");

    //-- Text
    ASSERT_EQ(document->getDocumentElement()->getTextContent(),"");


    ASSERT_NO_THROW(delete libxml2impl);
    ASSERT_NO_THROW(delete document);
}

TEST(vdom, first_level_fetch_no_thread) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    std::ifstream t("tests/vxml/vdom/vdom_regressions_doc.xml");
    std::string xml((std::istreambuf_iterator<char>(t)),
    std::istreambuf_iterator<char>());
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Get All First level elements
    //VDOM::VDOMNodeList * nl = document->getDocumentElement()->getElementsByTagNameNS("http://regression","l1subelement");
    VDOM::VDOMNodeList * nl = document->getDocumentElement()->getElementsByTagName("l1subelement");
    ASSERT_EQ(nl->getLength(),(unsigned int)8);

    //-- Go through all and check the index attributes
    for (unsigned int i=0;i<nl->getLength();i++) {

        //-- Check kattribute presence and value
        VDOM::VDOMElement * elt = nl->getItem<VDOM::VDOMElement>(i);
        ASSERT_TRUE(elt!=NULL);
        ASSERT_NE(elt->getAttribute("index"),"");
        ASSERT_EQ((unsigned int)atoi(elt->getAttribute("index").c_str()),i);

    }

    ASSERT_NO_THROW(delete libxml2impl);

}

TEST(vdom, first_level_fetch_thread) {

    //-- Get Implementation
    VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

    //-- Parse
    std::ifstream t("tests/vxml/vdom/vdom_regressions_doc.xml");
    std::string xml((std::istreambuf_iterator<char>(t)),
    std::istreambuf_iterator<char>());
    VDOM::VDOMDocument * document = libxml2impl->parseDocument(xml);
    ASSERT_TRUE(document!=NULL);

    //-- Get All First level elements
    //VDOM::VDOMNodeList * nl = document->getDocumentElement()->getElementsByTagNameNS("http://regression","l1subelement");
    VDOM::VDOMNodeList * nl = document->getDocumentElement()->getElementsByTagName("l1subelement");
    ASSERT_EQ(nl->getLength(),(unsigned int)8);

    //-- Go through all and check the index attributes
    #pragma omp parallel for
    for (unsigned int i=0;i<(nl->getLength()*500);i++) {

        if (omp_get_num_threads()<=1) {
            cerr << "THREAD TEST NOT PARALLELIZED" << endl;
        }

        unsigned ei = i%nl->getLength();

        //-- Check kattribute presence and value
        VDOM::VDOMElement * elt = nl->getItem<VDOM::VDOMElement>(ei);
        string index = elt->getAttribute("index");
        unsigned int indexInt = atoi(elt->getAttribute("index").c_str());
        EXPECT_TRUE(elt!=NULL);
        EXPECT_NE(elt->getAttribute("index"),"");
        EXPECT_EQ(indexInt,ei);

    }

    ASSERT_NO_THROW(delete libxml2impl);

}

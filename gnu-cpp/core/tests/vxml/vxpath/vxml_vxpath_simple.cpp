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
#include <vxml/vxpath/VXPathContext.h>


class vxpath : public ::testing::Test {

    protected:

        VDOM::VDOMDocument * document;

    public:

    virtual void SetUp() {

        //-- Get Implementation
        VDOM::VDOMImplementation * libxml2impl = VDOM::VDOMRegistry::getInstance().getImplementationForName("libxml2.dom");

        //-- Parse
        std::ifstream t("tests/vxml/vxpath/vxpath_regressions_doc.xml");
        std::string xml((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());
        document = libxml2impl->parseDocument(xml);
        ASSERT_TRUE(document!=NULL);

        ASSERT_NO_THROW(delete libxml2impl);

    }


    virtual void TearDown() {

        if (document!=NULL)
            ASSERT_NO_THROW(delete document);

    }


};


/**
 *
 */
TEST_F(vxpath, top_expressions_namespace) {


    //-- Execute Expressions
    //------------
    VDOM::VXPathContext * xpathContext = document->createXPathContext();
    xpathContext->addNamespaceBinding("r","http://regression");

    //-- Get document element
    string xpath("//r:regression");
    VDOM::VXPathResult * result = xpathContext->evaluate(xpath);

    //-- Check only one element in result set
    ASSERT_EQ(result->getNodeList().getLength(),1);
    ASSERT_EQ(result->getNodeList().getItem<VDOM::VDOMElement>(0)->getLocalName(),"regression");



}

TEST_F(vxpath, elements_get) {


    //-- Execute Expressions
    //------------
    VDOM::VXPathContext * xpathContext = document->createXPathContext();
    xpathContext->addNamespaceBinding("r","http://regression");

    //-- Get l1subelement
    string xpath("//r:l1subelement");
    VDOM::VXPathResult * result = xpathContext->evaluate(xpath);

    //-- Check 8 elements in result set
    ASSERT_EQ(result->getNodeList().getLength(),8);
    ASSERT_EQ(result->getNodeList().getItem<VDOM::VDOMElement>(0)->getLocalName(),"l1subelement");

    delete result;
    delete xpathContext;

}

TEST_F(vxpath, elements_empty_result) {

    //-- Execute Expressions
    //------------
    VDOM::VXPathContext * xpathContext = document->createXPathContext();
    xpathContext->addNamespaceBinding("r","http://regression");

    //-- Get l1subelement
    string xpath("//r:l1subelementNonExisting");
    VDOM::VXPathResult * result = xpathContext->evaluate(xpath);

    //-- Check only one element in result set
    ASSERT_EQ(result->getNodeList().getLength(),0);


    delete result;
    delete xpathContext;

}

TEST_F(vxpath, elements_single) {

    //-- Execute Expressions
    //------------
    VDOM::VXPathContext * xpathContext = document->createXPathContext();
    xpathContext->addNamespaceBinding("r","http://regression");

    //-- Get l1subelement
    string xpath("//r:l1subelement");
    VDOM::VDOMElement * result = xpathContext->evaluateAsElement(xpath);

    //-- Check only one element in result set
    ASSERT_TRUE(result!=NULL);

    delete xpathContext;

}




TEST_F(vxpath, attribute_get_check) {




    //-- Execute Expressions
    //------------
    VDOM::VXPathContext * xpathContext = document->createXPathContext();
    xpathContext->addNamespaceBinding("r","http://regression");

    //-- Get document attr0
    //---------------
    string xpath("//r:regression/@attr0");

    //-- FIXME: As Attribute Node

//    VDOM::VXPathResult * result = xpathContext->evaluate(xpath);
//    ASSERT_EQ(result->getNodeList().getLength(),1);
//    ASSERT_EQ(result->getNodeList().getItem<VDOM::VDOMElement>(0)->getLocalName(),"regression");

    //-- As String
    string attrStrValue  = xpathContext->evaluateAsString(xpath);
    ASSERT_EQ(attrStrValue,"test");

    //-- Get Inexisting attribute: empty string
    //---------------
    xpath = "//r:regression/@attrNonExisting";
    attrStrValue  = xpathContext->evaluateAsString(xpath);
    ASSERT_EQ(attrStrValue,"");


    delete xpathContext;

}

TEST_F(vxpath, attribute_local_element_get_check) {




    //-- Execute Expressions
    //------------
    VDOM::VXPathContext * xpathContext = document->createXPathContext(document->getDocumentElement());
    xpathContext->addNamespaceBinding("r","http://regression");

    //-- Get document attr0
    //---------------
    string xpath("@attr0");

    //-- FIXME: As Attribute Node

//    VDOM::VXPathResult * result = xpathContext->evaluate(xpath);
//    ASSERT_EQ(result->getNodeList().getLength(),1);
//    ASSERT_EQ(result->getNodeList().getItem<VDOM::VDOMElement>(0)->getLocalName(),"regression");

    //-- As String
    string attrStrValue  = xpathContext->evaluateAsString(xpath);
    ASSERT_EQ("test",attrStrValue);

    //-- Get Inexisting attribute: empty string
    //---------------
    xpath = "@attrNonExisting";
    attrStrValue  = xpathContext->evaluateAsString(xpath);
    ASSERT_EQ("",attrStrValue);


    delete xpathContext;

}

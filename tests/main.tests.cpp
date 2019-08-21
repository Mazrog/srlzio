//
// Created by Maxime.Cabanal-Duvil on 8/21/2019.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../deps/doctest.h"

#include "../srlzio/srlzio.hpp"
#include "models.hpp"

TEST_CASE("XML Tests") {
    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile("data/data.xml");
    XMLElement * root = doc.FirstChildElement("DataFile");

    REQUIRE(root);

    SUBCASE("Leaf parsing") {

        SUBCASE("Only value") {
            XMLElement * elem = root->FirstChildElement("Test1");
            Leaf1 leaf;

            parse(elem, leaf);
            CHECK_EQ(leaf.value, 3.141592);
        }

        SUBCASE("Value and one attribute") {
            XMLElement *elem = root->FirstChildElement("Test2");
            Leaf2 leaf;

            parse(elem, leaf);
            CHECK_EQ(leaf.value, 6.28456);
            CHECK_EQ(leaf.attributes.names[0], "id");
            CHECK_EQ(std::get<0>(leaf.attributes.values), 42);
        }

        SUBCASE("Value and multiples attributes") {
            XMLElement *elem = root->FirstChildElement("Test3");
            Leaf3 leaf;

            parse(elem, leaf);
            CHECK_EQ(leaf.value, 42);
            CHECK_EQ(leaf.attributes.names[0], "stiven");
            CHECK_EQ(leaf.attributes.names[1], "max");
            CHECK_EQ(leaf.attributes.names[2], "file");

            CHECK_EQ(std::get<0>(leaf.attributes.values), 1);
            CHECK_EQ(std::get<1>(leaf.attributes.values), 17);
            CHECK_EQ(std::get<2>(leaf.attributes.values), "/home/kappa");
        }
    }

    SUBCASE("Node Parsing") {
        SUBCASE("single child") {
            XMLElement * parent = root->FirstChildElement("Node1");
            Nested1 nested;

            parse(parent, nested);
            CHECK_EQ(nested.children.names[0], "Child1");
            CHECK_EQ(std::get<0>(nested.children.values).value, 6.66);
        }
    }
}
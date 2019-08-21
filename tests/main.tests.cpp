//
// Created by Maxime.Cabanal-Duvil on 8/21/2019.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../deps/doctest.h"

#include "../srlzio/srlzio.hpp"
#include "models.hpp"

#if 0
XMLElement * test = root->FirstChildElement("Test1"),
        * test2 = root->FirstChildElement("Test2"),
        * test3 = root->FirstChildElement("Test3");

    Leaf1 t;
    parse(test, t);
    std::cout << "Test  ------- " << t.value << '\n';

    Leaf2 t2;
    parse(test2, t2);
    std::cout << "Test2 ------- " << t2.value << " - And attribute id " << std::get<0>(t2.attributes.values) << '\n';

    Leaf3 t3;
    parse(test3, t3);
    std::cout << "Test3 ------- " << t3.value << " - And attributes: "
        << "stiven -> " << std::get<0>(t3.attributes.values)
        << " ## max -> " << std::get<1>(t3.attributes.values)
        << " ## file -> " << std::get<2>(t3.attributes.values) << '\n';

    puts("-----------------------------\nNested examples");

    XMLElement * node1 = root->FirstChildElement("Node1"),
        * node2 = root->FirstChildElement("Node2"),
        * node3 = root->FirstChildElement("Node3");

    Nested1 n1;
    parse(node1, n1);
    std::cout << "Test  ------- " << std::get<0>(n1.children.values).value << '\n';

    Nested2 n2;
    parse(node2, n2);
    std::cout << "Test  ------- \nid -> " << std::get<0>(n2.attributes.values)
              << " and children: \n" << std::get<0>(n2.children.values).value << " with attribute "
              << std::get<0>(n2.children.values).attributes.names[0] << " -> " <<
              std::get<0>(std::get<0>(n2.children.values).attributes.values) << '\n';

    TestNested1 n3;
    parse(node3, n3);
    std::cout << "Test  ------- \nid -> " << std::get<0>(n3.attributes.values) << " and children: \n"
              << std::get<0>(n3.children.values).value << " with attribute "
              << std::get<0>(n3.children.values).attributes.names[0] << " -> " <<
              std::get<0>(std::get<0>(n3.children.values).attributes.values) << '\n'

              << std::get<1>(n3.children.values).value << " with attributes "
              << std::get<1>(n3.children.values).attributes.names[0] << " -> " << std::get<0>(std::get<1>(n3.children.values).attributes.values) << " ## "
              << std::get<1>(n3.children.values).attributes.names[1] << " -> " << std::get<1>(std::get<1>(n3.children.values).attributes.values) << '\n';
#endif

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

        SUBCASE("single child and attributes") {
            XMLElement * parent = root->FirstChildElement("Node2");
            Nested2 nested;

            parse(parent, nested);
            CHECK_EQ(nested.children.names[0], "Child1");
            CHECK_EQ(std::get<0>(nested.children.values).value, 6.66);

            CHECK_EQ(nested.attributes.names[0], "id");
            CHECK_EQ(std::get<0>(nested.attributes.values), 42);

            CHECK_EQ(std::get<0>(nested.children.values).attributes.names[0], "stiven");
            CHECK_EQ(std::get<0>(std::get<0>(nested.children.values).attributes.values), "best");
        }
    }
}
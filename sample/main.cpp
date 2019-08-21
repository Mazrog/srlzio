#include <iostream>
#include "../srlzio/srlzio.hpp"
#include "../tests/models.hpp"

template < typename ... Args >
void pretty(Args && ...) {
    puts(__PRETTY_FUNCTION__);
}

/* --------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------- */
int main() {
    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile("data/data.xml");
    XMLElement * root = doc.FirstChildElement("DataFile");

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

    return 0;
}
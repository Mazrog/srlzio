//
// Created by Maxime.Cabanal-Duvil on 8/21/2019.
//

#include "../srlzio/srlzio.hpp"

struct Leaf1 : xml::leaf<double, xml::attributes<>> {};

struct Leaf2 : xml::leaf<double, xml::attributes<int>> {
    Leaf2() { attributes.names = { "id" }; }
};

struct Leaf3 : xml::leaf<int, xml::attributes<int, int, std::string>> {
    Leaf3() { attributes.names = { "stiven", "max", "file" }; }
};

struct Nested1 : xml::node<xml::attributes<>, xml::leaf<double, xml::attributes<>>> {
    Nested1() { children.names = { "Child1" }; }
};

struct Nested2 : xml::node<xml::attributes<int>, xml::leaf<double, xml::attributes<std::string>>> {
    Nested2() {
    attributes.names = { "id" };
    children.names = { "Child1" };
    std::get<0>(children.values).attributes.names = { "stiven" };
}
};

struct Nested3
        : xml::node<
                xml::attributes<int>,
          xml::leaf<double, xml::attributes<std::string>>,
          xml::leaf<double, xml::attributes<std::string, int>>
>
{
    Nested3() {
    attributes.names = { "id" };
    children.names = { "Child1", "Child2" };
    std::get<0>(children.values).attributes.names = { "stiven" };
    std::get<1>(children.values).attributes.names = { "max", "age" };
}
};

struct SubNested1 : xml::leaf<double, xml::attributes<std::string, int>> {
    SubNested1() { attributes.names = { "max", "age" }; }
};

struct TestNested1 : xml::node<xml::attributes<int>,
                    xml::leaf<double, xml::attributes<std::string>>,
                    SubNested1> {
    TestNested1() {
        attributes.names = { "id" };
        children.names = { "Child1", "Child2" };
        std::get<0>(children.values).attributes.names = { "stiven" };
    }
};
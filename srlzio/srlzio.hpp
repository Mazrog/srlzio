//
// Created by Maxime.Cabanal-Duvil on 8/21/2019.
//

#ifndef SRLZIO_SRLZIO_HPP
#define SRLZIO_SRLZIO_HPP

#include <array>
#include <tuple>
#include <string>

#include <tinyxml2.h>

namespace xml {
    template < typename ... Ts >
    struct dict {
        std::tuple<Ts...> values;
        std::array<std::string, sizeof...(Ts)>   names;
    };

    template < bool is_node, typename ... > struct tag;

    template < typename ... Attrs, typename ... Children >
    struct tag<true, dict<Attrs...>, Children...> {
        dict<Attrs...> attributes;
        dict<Children...> children;
    };

    template < typename T, typename ... Attrs >
    struct tag<false, T, dict<Attrs...>> {
        T value;
        dict<Attrs...> attributes;
    };


    template < typename ... Attrs >
    using attributes = dict<Attrs...>;

    template < typename T, typename ... Attrs >
    using leaf = tag<false, T, Attrs...>;

    template < typename ... Ts >
    using node = tag<true, Ts...>;
}

/* --------------------------------------------------------------------------------------- */

template < typename T >
void read(tinyxml2::XMLElement *, T *);

template < >
void read<double>(tinyxml2::XMLElement * elem, double * d) { elem->QueryDoubleText(d); }

template < >
void read<int>(tinyxml2::XMLElement * elem, int * i) { elem->QueryIntText(i); }



/* --------------------------------------------------------------------------------------- */
template < typename T >
void load(tinyxml2::XMLElement * , std::string_view , T & );

template <>
void load<int>(tinyxml2::XMLElement * elem, std::string_view attribute_name, int & attribute) {
    elem->QueryAttribute(attribute_name.data(), &attribute);
}

template <>
void load<std::string>(tinyxml2::XMLElement * elem, std::string_view attribute_name, std::string & attribute) {
    attribute = elem->Attribute(attribute_name.data());
}


/* --------------------------------------------------------------------------------------- */
// Some declarations
template < typename T, typename ... Attrs >
void parse(tinyxml2::XMLElement * , xml::leaf<T, xml::attributes<Attrs...>> &);

template < typename ... Attrs, typename ... Children >
void parse(tinyxml2::XMLElement * , xml::node<xml::attributes<Attrs...>, Children...> &);


/* --------------------------------------------------------------------------------------- */
template < typename ... Attrs, std::size_t ... I >
void read_attrs(tinyxml2::XMLElement * elem, xml::dict<Attrs...> & attributes, std::index_sequence<I...>) {
    (load(elem, attributes.names[I], std::get<I>(attributes.values)), ...);
}

template < typename T >
void load_children(tinyxml2::XMLElement * parent, const char * child_name, T & model) {
    tinyxml2::XMLElement * elem = parent->FirstChildElement(child_name);
    if (elem) {
        parse(elem, model);
    }
}

template < typename ... Children, std::size_t ... I >
void read_children(tinyxml2::XMLElement * parent, xml::dict<Children...> & children, std::index_sequence<I...>) {
    (load_children(parent, children.names[I].c_str(), std::get<I>(children.values)), ...);
}

/* --------------------------------------------------------------------------------------- */
template < typename T, typename ... Attrs >
void parse(tinyxml2::XMLElement * elem, xml::leaf<T, xml::attributes<Attrs...>> & leaf) {
    read<T>(elem, &leaf.value);

    if constexpr (sizeof...(Attrs)) {
        read_attrs(elem, leaf.attributes, std::make_index_sequence<sizeof...(Attrs)>{});
    }
}

template < typename ... Attrs, typename ... Children >
void parse(tinyxml2::XMLElement * elem, xml::node<xml::attributes<Attrs...>, Children...> & node) {
    if constexpr (sizeof...(Attrs)) {
        read_attrs(elem, node.attributes, std::make_index_sequence<sizeof...(Attrs)>{});
    }

    read_children(elem, node.children, std::make_index_sequence<sizeof...(Children)>{});
}

#endif //SRLZIO_SRLZIO_HPP

# srlzio - C++ Header-only XML parsing library

- [Get started](#get-started)
- [Reference](#reference)

srlzio lets you create model-structures to represent your data, and then parse it without writing anything else!

srlzio was built with the help of [tinyxml2](https://github.com/leethomason/tinyxml2).

## Todo Road

- [XML] Simplify access to children or attributes (helper functions or operator overload)
- (Future) Thinking about adding support for others languages than XML

## Get Started

As it is a header-only library, download the latest version of the file and include it in your project and as it uses the tinyxml2 library, be sure you have it installed and linked to your project.

Let's consider this example: we have a file that contains multiple IDs informations (name and age) about employees for an certain office (name).
In XML, it could be written this way

```xml
<?xml version="1.0" encoding="utf-8" ?>
<OfficeInfo>
  <Office>
    <Name>Splendid Office</Name>
  </Office>
  <Employees>
    <Employee id="0" gender="H">
      <Name>John</Name>
      <Age>42</Age>
    </Employee>
    <Employee id="1" gender="F">
      <Name>Jane</Name>
      <Age>36</Age>
    </Employee>
    <Employee id="2" gender="F">
      <Name>Meryl</Name>
      <Age>40</Age>
    </Employee>
  </Employees>
</OfficeInfo>
```

We create the model-structure in C++, from the leaves to the root:

```cpp
// models.hpp
#include "srlzio.hpp"

struct Employee : xml::node<
    xml::attributes<int, std::string>,
    xml::leaf<std::string, xml::attributes<>>,
    xml::leaf<int, xml::attributes<>>
  > {
    Employee() {
      attributes.names = { "id", "gender" };
      children.names = { "Name", "Age" };
    }
};

struct Employees : xml::node<xml::attributes<>, xml::collection<Employee>> {
  Employees() { children.names = { "Employee" }; }
};

struct Office : xml::node<xml::attributes<>, xml::leaf<std::string, xml::attributes<>>> {
  Office() { children.names = { "Name" }; }
};

struct OfficeInfo : xml::node<xml::attributes<>, Office, Employees> {
  OfficeInfo() { children.names = { "Office", "Employees" }; }
};
```

And the main:

```cpp
#include <iostream>
#include "srlzio.hpp"
#include "models.hpp"

int main() {
  using namespace tinyxml2;
  XMLDocument doc;
  doc.LoadFile("example.xml");
  XMLElement * root = doc.FirstChildElement("OfficeInfo");

  OfficeInfo infos;
  parse(root, infos);

  /* display loop */
  std::cout << "Office's name: " << std::get<0>(std::get<0>(infos.children.values).children.values).value << '\n';
  // here the first child of infos is the Office object, which has its name as first child too

  for (Employee const& employee : std::get<0>(std::get<1>(infos.children.values).children.values).items) {
    std::cout << "Employee [ID=" << std::get<0>(employee.attributes.values) << "] - Gender: " << std::get<1>(employee.attributes.values) << '\n';
    std::cout << std::get<0>(employee.children.values).value << ", " << std::get<1>(employee.children.values).value << "years old\n\n";
  }

  return 0;
}
```


## Reference

srlzio gives you three main structures to represent your data, `leaf`, `node`, and `collection`, the latter being a logical one (ie not an existing Xml element in the file).

***Note:** For this reference, I will omit some implementation details (for instance the `tag` structure) and focus on the conceptual ones. If you want to have a look at it, feel free, the main implementation is 50ish lines.*

### # Leaf

A `leaf` is just a single xml tag, with a plain data value.

```xml
<Pi>3.1415</Pi>

<File desc="my beautiful item" id="42">/home/kappa/foo</File>
```

It is represented by 2 fields, the data contained and its attributes. In srlzio, the structure `xml::leaf` is here for that, and is expecting two template parameters: the `DataType`, and a parameter pack with the *types* of attributes, contained within a `xml::attributes` struct.

Equivalent model-structures to the leafs above:

```cpp
// Standard format:
// xml::leaf<DataType, xml::attributes<AttributesValueTypes...>>

xml::leaf<double, xml::attributes<>> pi;

xml::leaf<std::string, xml::attributes<std::string, int>> file;
```

### # Node

A `node` is basicly a `leaf` but with children instead of a value.

```xml
<Node1>
  <Child1>6.66</Child1>
</Node1>

<Node3 id="44">
  <Child1 stiven="best">6.66</Child1>
  <Child2 max="mazrog" age="23">17.69</Child2>
</Node3>
```

In these examples, children are leafs, but they could be other nodes.

Instead of a value, the `node` will have children nodes, it is written with the help of `xml::node` structure:

```cpp
// Standard format:
// xml::node<xml::attributes<AttributesValueTypes...>, Children...>

xml::node<xml::attributes<>, xml::leaf<double, xml::attributes<>>> node1;

xml::node<
  xml::attributes<int>, // parent node attributes
  xml::leaf<double, xml::attributes<std::string>>,      // first child
  xml::leaf<double, xml::attributes<std::string, int>>  // second child
  > node3;
```

### # Collection

A `collection` is a logical structure used to contain multiple identical items (ie with the same tagname).

```xml
<Collection1>
  <Item id="1">45</Item>
  <Item id="2">78</Item>
</Collection1>
```

To represent this, we use the structure `xml::collection`, that takes a type of element that will be read a unknown number of times.

```cpp
// Standard format:
// xml::collection<Item>

xml::collection<xml::leaf<int, xml::attributes<int>>> collection1;
```

### Attributes, Children and value

The following structure is used to contain attributes and children (`xml::attribute` is just an `using` declaration for clarity). Objects are supposed to be in the same order in the two containers, ie the first attribute read will be read with the first name in `names` and will be stored in the first place of `values`.

```cpp
template < typename ... Ts >
struct dict {
  std::tuple<Ts...> values;
  std::array<std::string, sizeof...(Ts)>   names;
};
```

Setting names for children and attributes is what allows parsing, so be sure to fill it properly at the creation of the model structure!

### Usage Notes

The structure were thought in a heritage way, letting you create your structures and inherit from srlzio ones, and setting things up in a default constructor:

```cpp
// From examples above in the leaf section
// <File desc="my beautiful item" id="42">/home/kappa/foo</File>
struct FileLeaf : xml::leaf<std::string, xml::attributes<std::string, int>> {
  FileLeaf() { attributes.names = { "desc", "id" }; }
};
```

We set **in the same order** the names of the attributes as the types in the inherit line.

### Parsing

Once your structures are created, you can load your file, get the `XmlElement *` corresponding to the root of your model, and call the `parse` function!

```cpp
/*
Example taken from sample/main.cpp
*/

using namespace tinyxml2;
XMLDocument doc;
doc.LoadFile("data/recipe.xml");
XMLElement * root = doc.FirstChildElement("DataFile");

XMLElement * models_node = root->FirstChildElement("Models");

// declaring the high level model-structure
Models models;
parse(models_node, models);
// and that's all ! models structure should be filled
```
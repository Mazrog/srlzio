#include <iostream>
#include "../srlzio/srlzio.hpp"
#include "models.hpp"

/* --------------------------------------------------------------------------------------- */
int main() {
    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile("data/recipe.xml");
    XMLElement * root = doc.FirstChildElement("DataFile");

    XMLElement * models_node = root->FirstChildElement("Models");

    Models models;
    parse(models_node, models);

    for (ModelBuffer const& model_buffer : models.children.get<0>().items) {
        std::cout << "ModelBuffer: [ID=" << model_buffer.attributes.get<0>() << "]\n";

        for (Model const& model : model_buffer.children.get<0>().items) {
            std::cout << "\tModel [ID=" << model.attributes.get<0>() << "] ## "
                << model.children.names[0] << " -> " << model.children.get<0>().value << "\n";
        }
        std::cout << '\n';
    }

    return 0;
}
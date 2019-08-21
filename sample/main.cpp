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

    for (ModelBuffer const& model_buffer : std::get<0>(models.children.values).items) {
        std::cout << "ModelBuffer: [ID=" << std::get<0>(model_buffer.attributes.values) << "]\n";

        for (Model const& model : std::get<0>(model_buffer.children.values).items) {
            std::cout << "\tModel [ID=" << std::get<0>(model.attributes.values) << "] ## "
                << model.children.names[0] << " -> " << std::get<0>(model.children.values).value << "\n";
        }
        std::cout << '\n';
    }

    return 0;
}
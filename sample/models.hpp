//
// Created by Maxime.Cabanal-Duvil on 8/21/2019.
//

#ifndef SRLZIO_MODELS_HPP
#define SRLZIO_MODELS_HPP

#include "../srlzio/srlzio.hpp"

struct Model : xml::node<xml::attributes<int>, xml::leaf<std::string, xml::attributes<>>> {
    Model() {
        attributes.names = { "id" };
        children.names = { "File" };
    }
};

struct ModelBuffer : xml::node<xml::attributes<int>, xml::collection<Model>> {
    ModelBuffer() {
        attributes.names = { "id" };
        children.names = { "Model" };
    }
};

struct Models : xml::node<xml::attributes<>, xml::collection<ModelBuffer>> {
    Models() {
        children.names = { "ModelBuffer" };
    }
};

#endif //SRLZIO_MODELS_HPP

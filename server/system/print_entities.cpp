#include "systems.hpp"

#include <iostream>

#include "core/component/tags.hpp"

namespace server::system {

void print_entities(flecs::world& ecs) {
    ecs.system<const core::component::Actor>("PrintEntities")
        .kind(flecs::OnUpdate)
        .iter([](flecs::iter& iter) {
            for (size_t i = 0; i < iter.count(); ++i) {
                flecs::entity e = iter.entity(i);

                flecs::entity_to_json_desc_t desc;
                desc.serialize_id_labels = false;
                desc.serialize_base = false;
                desc.serialize_path = false;
                desc.serialize_type_info = false;
                desc.serialize_label = true;
                desc.serialize_values = true;
                std::cout << e.to_json(&desc) << "\n";
            }
        });
}

}  // namespace server::system

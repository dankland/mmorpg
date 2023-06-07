#include <iostream>
#include <stdexcept>

#include "asio.hpp"
#include "flecs.h"
#include "spdlog/spdlog.h"

#include "renderer.hpp"
#include "window.hpp"

#include "core/system/systems.hpp"

#include "core/component/actor_command.hpp"
#include "core/component/position.hpp"
#include "core/component/tags.hpp"
#include "core/component/velocity.hpp"
#include "server_connection.hpp"

using udp = asio::ip::udp;

namespace client {

void setup_singletons(flecs::world& ecs) {
    ecs.set<std::unordered_map<udp::endpoint, flecs::entity>>({});
}

void setup_reflection(flecs::world& ecs) {
    ecs.component<core::component::Position>().member<float>("x").member<float>("y");
    ecs.component<core::component::Velocity>().member<float>("x").member<float>("y");
    ecs.component<core::component::ActorCommand>().member<std::uint64_t>("m_commands");
    ecs.component<core::component::ActorCommandBuffer>()
        .member<std::size_t>("m_head")
        .member<std::size_t>("m_tail")
        .member<core::component::ActorCommand[core::component::ActorCommandBuffer::buffer_size]>(
            "m_command_buffer");
}

void create_actor(flecs::entity entity,
                  const std::string& actor_name,
                  udp::socket* socket,
                  udp::endpoint client_endpoint) {
    entity.set_name(actor_name.c_str())
        .add<core::component::Actor>()
        .set<core::component::Position>({0.0f, 0.0f})
        .set<core::component::Velocity>({})
        .set<core::component::ActorCommandBuffer>({})
        .emplace<ServerConnection>(socket, std::move(client_endpoint), entity);
}

void setup_systems(flecs::world& ecs) {
    core::system::load_actor_command(ecs, flecs::OnUpdate);
    core::system::update_velocity(ecs, flecs::OnUpdate);
    core::system::update_position(ecs, flecs::OnUpdate);

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
    // system::print_entities(ecs);
    // system::afk_check(ecs);
    // system::prepare_server_broadcast(ecs);
    // system::send_server_broadcast(ecs);
}

asio::awaitable<void> receive_messages(udp::socket& socket,
                                       udp::endpoint server_endpoint,
                                       flecs::world& ecs) {
    bool entity_created = false;
    auto client_entity = ecs.entity();
    std::array<char, 4096> buffer{};
    while (true) {
        std::fill(buffer.begin(), buffer.end(), 0);
        co_await socket.async_receive_from(
            asio::buffer(buffer), server_endpoint, asio::use_awaitable);
        std::string message{buffer.data()};
        message = message.substr(0, message.find('\n'));

        spdlog::info("Received connection from {}:{}",
                     server_endpoint.address().to_string(),
                     server_endpoint.port());
        if (!entity_created) {
            create_actor(client_entity, message, &socket, std::move(server_endpoint));
            entity_created = true;
        }
        client_entity.get_mut<ServerConnection>()->handle_message(message);
    }
}

asio::awaitable<void> login(udp::socket& socket, udp::endpoint server_endpoint) {
    co_await socket.async_send_to(asio::buffer("client1"), server_endpoint, asio::use_awaitable);
}

}  // namespace client

int main() {
    flecs::world ecs;
    ecs.set_target_fps(1);

    client::setup_singletons(ecs);
    client::setup_reflection(ecs);
    client::setup_systems(ecs);

    std::thread server_thread{[&]() {
        asio::io_context io_context;
        asio::signal_set signals{
            io_context, SIGINT, SIGTERM, SIGQUIT, asio::defaulted_constraint()};
        signals.async_wait([&](auto, auto) { io_context.stop(); });

        auto server_endpoint = udp::endpoint{udp::v4(), 54321};
        udp::socket socket{io_context.get_executor(), udp::endpoint{udp::v4(), 54322}};
        asio::co_spawn(io_context, client::login(socket, server_endpoint), asio::detached);
        asio::co_spawn(
            io_context, client::receive_messages(socket, server_endpoint, ecs), asio::detached);
        io_context.run();
    }};

    try {
        auto window = Window(800, 600, "MMORPG");
        auto renderer = OpenGLRenderer();

        while (!window.should_close()) {
            window.poll();

            if (window.get_key(Inputs::KEY_ESCAPE)) {
                break;
            }

            auto actor_command_buffer = ecs.get_mut<core::component::ActorCommandBuffer>();
            if (window.get_key(Inputs::KEY_W)) {
                actor_command_buffer->push(
                    core::component::ActorCommand{core::component::ActorCommand::Type::MoveUp});
            } else if (window.get_key(Inputs::KEY_S)) {
                actor_command_buffer->push(
                    core::component::ActorCommand{core::component::ActorCommand::Type::MoveDown});
            }
            if (window.get_key(Inputs::KEY_A)) {
                actor_command_buffer->push(
                    core::component::ActorCommand{core::component::ActorCommand::Type::MoveLeft});
            } else if (window.get_key(Inputs::KEY_D)) {
                actor_command_buffer->push(
                    core::component::ActorCommand{core::component::ActorCommand::Type::MoveRight});
            }

            ecs.progress();

            window.clear();

            renderer.set_aspect(window.get_aspect());

            renderer.start();

            renderer.draw_quad(glm::vec2(0.0f, 0.0f),
                               glm::vec2(0.4f, 0.4f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                               -5.0f);

            renderer.draw_quad(glm::vec2(0.2f, 0.2f),
                               glm::vec2(0.4f, 0.4f),
                               glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                               5.0f);

            ecs.each<core::component::Position>(
                [&](flecs::entity e, core::component::Position& position) {
                    renderer.draw_quad(glm::vec2(position.x, position.y),
                                       glm::vec2(0.2f, 0.2f),
                                       glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
                                       0.0f);
                });

            renderer.end();

            window.swap();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#include <iostream>

#include "asio.hpp"
#include "flecs.h"
#include "spdlog/spdlog.h"

#include "core/component/character_command.hpp"
#include "core/component/position.hpp"
#include "core/component/tags.hpp"
#include "core/component/velocity.hpp"

using udp = asio::ip::udp;

void setup_entities(flecs::world& ecs) {
    ecs.component<core::component::Position>().member<float>("x").member<float>("y");
    ecs.component<core::component::Velocity>().member<float>("x").member<float>("y");
    ecs.component<core::component::CharacterCommand>().member<std::uint64_t>("m_commands");
}

void setup_systems(flecs::world& ecs) {
    ecs.system<const core::component::CharacterCommand, core::component::Velocity>("VelocityUpdate")
        .kind(flecs::OnUpdate)
        .each([](const core::component::CharacterCommand& character_command,
                 core::component::Velocity& velocity) {
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            if (character_command.contains(core::component::CharacterCommand::MoveUp)) {
                velocity.y -= 100.0f;
            }
            if (character_command.contains(core::component::CharacterCommand::MoveLeft)) {
                velocity.x -= 100.0f;
            }
            if (character_command.contains(core::component::CharacterCommand::MoveDown)) {
                velocity.y += 100.0f;
            }
            if (character_command.contains(core::component::CharacterCommand::MoveRight)) {
                velocity.x += 100.0f;
            }
        });
    ecs.system<core::component::CharacterCommand>("ClearCharacterCommand")
        .kind(flecs::OnUpdate)
        .each([](core::component::CharacterCommand& character_command) {
            character_command.clear();
        });
    ecs.system<const core::component::Velocity, core::component::Position>("PositionTransformation")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 const core::component::Velocity& velocity,
                 core::component::Position& position) {
            float dt = e.delta_time();
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
        });
    ecs.system<const core::component::Actor>("PrintEntity")
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

void run_app(flecs::world& ecs) {
    ecs.app().target_fps(1).run();
}

asio::awaitable<void> request_handler(udp::socket& socket,
                                      udp::endpoint client_endpoint,
                                      flecs::world& ecs) {
    // TODO: Replace the ecs parameter with an actual class which contains the ecs and will be
    //       called to create new entities upon receiving new connections as well as commands
    //       received from clients atomically. Create entity upon starting the connection and
    //       populate a buffer with commands received from the client atomically.
    flecs::entity entity = ecs.entity()
                               .add<core::component::Actor>()
                               .set<core::component::Position>({0, 0})
                               .set<core::component::Velocity>({0, 0})
                               .set<core::component::CharacterCommand>({});
    std::array<char, 4096> buffer{};
    while (socket.is_open()) {
        try {
            std::size_t bytes_read = co_await socket.async_receive_from(
                asio::buffer(buffer), client_endpoint, asio::use_awaitable);
            if (bytes_read == 0) {
                throw std::runtime_error("read nothing from socket!");
            }

            auto payload = std::string(buffer.data(), buffer.data() + bytes_read);
            std::uint64_t command = std::stoull(payload);

            entity.get_mut<core::component::CharacterCommand>()->add(
                static_cast<core::component::CharacterCommand::Type>(command));
        } catch (std::exception& e) {
            spdlog::error("Exception: {}", e.what());
        }
    }
    entity.destruct();
}

asio::awaitable<void> connection_listener(unsigned short port, flecs::world& ecs) {
    auto executor = co_await asio::this_coro::executor;
    auto endpoint = asio::ip::udp::endpoint{udp::v4(), port};
    spdlog::info("Starting server at {}:{}", endpoint.address().to_string(), endpoint.port());
    udp::socket server_socket{executor, endpoint};
    udp::endpoint client_endpoint;
    std::array<char, 4096> buffer{};
    while (true) {
        co_await server_socket.async_receive_from(
            asio::buffer(buffer), client_endpoint, asio::use_awaitable);
        asio::co_spawn(executor,
                       request_handler(server_socket, std::move(client_endpoint), ecs),
                       asio::detached);
    }
}

int main() {
    flecs::world ecs;

    setup_entities(ecs);
    setup_systems(ecs);

    std::thread server_thread{[&]() {
        asio::io_context io_context;
        asio::signal_set signals{io_context, SIGINT, SIGTERM, SIGQUIT};
        signals.async_wait([&](auto, auto) { io_context.stop(); });
        asio::co_spawn(io_context, connection_listener(54321, ecs), asio::detached);
        io_context.run();
    }};

    run_app(ecs);

    return 0;
}

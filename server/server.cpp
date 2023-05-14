#include <set>

#include "asio.hpp"
#include "flecs.h"
#include "spdlog/spdlog.h"

#include "core/component/actor_command.hpp"
#include "core/component/position.hpp"
#include "core/component/tags.hpp"
#include "core/component/velocity.hpp"
#include "core/system/systems.hpp"
#include "core/utility/server_message.hpp"

#include "system/systems.hpp"

#include "client_connection.hpp"

using udp = asio::ip::udp;

namespace server {

void setup_singletons(flecs::world& ecs) {
    ecs.set<std::unordered_map<udp::endpoint, flecs::entity>>({});
}

void setup_entities(flecs::world& ecs) {
    ecs.component<core::component::Position>().member<float>("x").member<float>("y");
    ecs.component<core::component::Velocity>().member<float>("x").member<float>("y");
    ecs.component<core::component::ActorCommand>().member<std::uint64_t>("m_commands");
}

void setup_systems(flecs::world& ecs) {
    core::system::update_velocity(ecs, flecs::OnUpdate);
    core::system::clear_actor_command(ecs, flecs::OnUpdate);
    core::system::update_position(ecs, flecs::OnUpdate);

    system::acknowledge_server_messages(ecs);
    system::print_entities(ecs);
    system::afk_check(ecs);
}

void run_app(flecs::world& ecs) {
    ecs.app().target_fps(1).run();
}

asio::awaitable<void> receive_messages(unsigned short port, flecs::world& ecs) {
    auto executor = co_await asio::this_coro::executor;
    auto endpoint = asio::ip::udp::endpoint{udp::v4(), port};

    spdlog::info("Starting server at {}:{}", endpoint.address().to_string(), endpoint.port());
    udp::socket server_socket{executor, endpoint};

    std::array<char, 4096> buffer{};
    while (true) {
        udp::endpoint client_endpoint;
        co_await server_socket.async_receive_from(
            asio::buffer(buffer), client_endpoint, asio::use_awaitable);

        auto client_connections_ptr =
            ecs.get_mut<std::unordered_map<udp::endpoint, flecs::entity>>();
        auto [client_connections_iter, is_new] =
            client_connections_ptr->emplace(client_endpoint, ecs.entity());

        auto client_entity = client_connections_iter->second;
        if (is_new) {
            spdlog::info("Received connection from {}:{}",
                         client_endpoint.address().to_string(),
                         client_endpoint.port());
            client_entity.add<core::component::Actor>()
                .set<core::component::Position>({0, 0})
                .set<core::component::Velocity>({0, 0})
                .set<core::component::ActorCommand>({})
                .set<std::vector<core::utility::ServerMessage::Acknowledgement>>({})
                .emplace<ClientConnection>(&server_socket, client_endpoint, client_entity);
        }
        client_entity.get_mut<ClientConnection>()->handle_message(buffer);
    }
}

}  // namespace server

int main() {
    flecs::world ecs;

    server::setup_singletons(ecs);
    server::setup_entities(ecs);
    server::setup_systems(ecs);

    std::thread server_thread{[&]() {
        asio::io_context io_context;
        asio::signal_set signals{
            io_context, SIGINT, SIGTERM, SIGQUIT, asio::defaulted_constraint()};
        signals.async_wait([&](auto, auto) { io_context.stop(); });
        asio::co_spawn(io_context, server::receive_messages(54321, ecs), asio::detached);
        io_context.run();
    }};

    server::run_app(ecs);

    return 0;
}

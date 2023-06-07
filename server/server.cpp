#include <set>

#include "asio.hpp"
#include "flecs.h"
#include "spdlog/spdlog.h"

#include "core/component/actor_command.hpp"
#include "core/component/position.hpp"
#include "core/component/tags.hpp"
#include "core/component/velocity.hpp"

#include "core/system/systems.hpp"
#include "system/systems.hpp"

#include "client_connection.hpp"

using udp = asio::ip::udp;

namespace server {

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

void create_actor(flecs::entity client_entity,
                  const std::string& actor_name,
                  udp::socket* server_socket,
                  udp::endpoint client_endpoint) {
    client_entity.set_name(actor_name.c_str())
        .add<core::component::Actor>()
        .set<core::component::Position>({0.0f, 0.0f})
        .set<core::component::Velocity>({})
        .set<core::component::ActorCommand>({})
        .set<core::component::ActorCommandBuffer>({})
        .emplace<ClientConnection>(server_socket, std::move(client_endpoint), client_entity);
}

void setup_systems(flecs::world& ecs) {
    core::system::load_actor_command(ecs, flecs::OnUpdate);
    core::system::update_velocity(ecs, flecs::OnUpdate);
    core::system::update_position(ecs, flecs::OnUpdate);

    system::print_entities(ecs);
    system::afk_check(ecs);
    system::prepare_server_broadcast(ecs);
    system::send_server_broadcast(ecs);
}

asio::awaitable<void> receive_messages(unsigned short port, flecs::world& ecs) {
    auto executor = co_await asio::this_coro::executor;
    auto endpoint = udp::endpoint{udp::v4(), port};

    spdlog::info("Starting server at {}:{}", endpoint.address().to_string(), endpoint.port());
    udp::socket server_socket{executor, endpoint};

    std::array<char, 4096> buffer{};
    while (true) {
        udp::endpoint client_endpoint;
        std::fill(buffer.begin(), buffer.end(), 0);
        co_await server_socket.async_receive_from(
            asio::buffer(buffer), client_endpoint, asio::use_awaitable);
        std::string message{buffer.data()};
        message = message.substr(0, message.find('\n'));

        auto client_connections_ptr =
            ecs.get_mut<std::unordered_map<udp::endpoint, flecs::entity>>();
        auto [client_connections_iter, is_new] =
            client_connections_ptr->emplace(client_endpoint, ecs.entity());

        auto client_entity = client_connections_iter->second;
        if (is_new) {
            spdlog::info("Received connection from {}:{}",
                         client_endpoint.address().to_string(),
                         client_endpoint.port());
            create_actor(client_entity, message, &server_socket, std::move(client_endpoint));
        } else {
            client_entity.get_mut<ClientConnection>()->handle_message(message);
        }
    }
}

}  // namespace server

int main() {
    flecs::world ecs;

    server::setup_singletons(ecs);
    server::setup_reflection(ecs);
    server::setup_systems(ecs);

    std::thread server_thread{[&]() {
        asio::io_context io_context;
        asio::signal_set signals{
            io_context, SIGINT, SIGTERM, SIGQUIT, asio::defaulted_constraint()};
        signals.async_wait([&](auto, auto) { io_context.stop(); });
        asio::co_spawn(io_context, server::receive_messages(54321, ecs), asio::detached);
        io_context.run();
    }};

    ecs.app().target_fps(1.0).run();

    return 0;
}

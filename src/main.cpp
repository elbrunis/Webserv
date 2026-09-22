#include "../inc/Headers.hpp"
#include "../inc/Server.hpp"

int main() // en un futuro recibira de argumento la config
{
    Server host = Server();
    host.run();
    return (0);
}
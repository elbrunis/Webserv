#include "../inc/Headers.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

void	Server::accept_new_client()
{
	int new_sockfd = accept(this->lisent_fd, NULL, NULL);
	fcntl(new_sockfd, F_SETFL, O_NONBLOCK);

	struct pollfd	new_fd;
	new_fd.fd = new_sockfd;
	new_fd.events = POLLIN;
	this->fds.push_back(new_fd);

	this->clients[new_sockfd] = Client();
}

int	Server::read_client(int i)
{
	char buf[BUFF_LEN];
	int n = recv(fds[i].fd, buf, sizeof(buf), 0);
	if (n <= 0)
	{
		this->clients.erase(fds[i].fd); // esto es un map y va con entero
		close(this->fds[i].fd);
		this->fds.erase(this->fds.begin() + i); // esto es un vector y va por puntero
		return (1);
	}

	Client& c = this->clients[fds[i].fd];
	c.readbuffer.append(buf, n);
	if (!c.headersComplete && c.readbuffer.find("\r\n\r\n") != std::string::npos)
	{
		c.writebuffer = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello World";
		c.bitesent = 0;
		c.headersComplete = true;
		fds[i].events = POLLOUT;
	}
	return (0);
}

void	Server::write_client(int i)
{
	Client& c = this->clients[fds[i].fd];
	int n = send(fds[i].fd, c.writebuffer.c_str() + c.bitesent,
						c.writebuffer.size() - c.bitesent, 0);
	if (n > 0)
		c.bitesent += n;
	if (c.bitesent == (int)c.writebuffer.size())
	{
		this->clients.erase(fds[i].fd);
		close(this->fds[i].fd);
		this->fds.erase(this->fds.begin() + i);
	}
}

void	Server::run()
{
	fcntl(this->lisent_fd, F_SETFL, O_NONBLOCK);

	struct pollfd lisentPfd;
	lisentPfd.fd = this->lisent_fd;
	lisentPfd.events = POLLIN;
	this->fds.push_back(lisentPfd);

	while(1)
	{
		poll(&this->fds[0], this->fds.size(), -1);
		for (size_t i = 0; i < this->fds.size(); i++)
		{
			if ((this->fds[i].fd == this->lisent_fd) &&
								(this->fds[i].revents & POLLIN)) //Acepta y crea un nuevo cliente
			{
				this->accept_new_client();
				continue;
			}
			if (this->fds[i].revents & POLLIN) // lee la peticion de un cliente
			{
				if (this->read_client(i))
					i--;
			}
			else if(this->fds[i].revents & POLLOUT)
			{
				this->write_client(i);
				i--;
			}
		}
	}
}
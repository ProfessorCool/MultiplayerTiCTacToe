#include <enet/enet.h>
#include <iostream>

int main(int argc, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	std::cout << "Enter 0 in to create a client, enter 1 in to create a server.\n";
	bool isServer;
	std::cin >> isServer;
	if (!isServer)	//client code
	{
		ENetHost* client;
		client = enet_host_create(NULL, 1, 1, 0, 0);

		if (client == NULL)
		{
			fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
			return EXIT_FAILURE;
		}

		ENetAddress address;
		ENetEvent event;
		ENetPeer* peer;

		enet_address_set_host(&address, "127.0.0.1");	//This is localhost
		address.port = 7777;

		peer = enet_host_connect(client, &address, 1, 0);
		if (peer == NULL)
		{
			fprintf(stderr, "No available peers for initiating an ENet connection.\n");
			return EXIT_FAILURE;
		}

		if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) //is true if data is received from server in 5 seconds and the packet is a connection type
		{
			puts("Connection to 127.0.0.1:7777 succeeded!");
		}
		else
		{
			enet_peer_reset(peer);
			puts("Connection to 127.0.0.1:7777 failed.");
		}

		// GAME LOOP START

		while (enet_host_service(client, &event, 1000) > 0) //is true if data is received from server in 1 second
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID);

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Server at %x:%u shut down.\n",
					event.peer->address.host,
					event.peer->address.port);
				break;
			}
		}

		// GAME LOOP END

		enet_peer_disconnect(peer, 0);

		while (enet_host_service(client, &event, 3000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnection succeeded.");
				break;
			}
		}

		return EXIT_SUCCESS;
	}
	else   //Host code
	{
		ENetAddress address;
		ENetHost* server;
		ENetEvent event;

		address.host = ENET_HOST_ANY;
		address.port = 7777;

		server = enet_host_create(&address, 1, 1, 0, 0);
		
		if (server == NULL)
		{
			fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
			return EXIT_FAILURE;
		}
		
		

		//GAME LOOP START
		while (true)
		{
			while (enet_host_service(server, &event, 1000) > 0)
			{
				
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					printf("A new client connected from %x:%u.\n",
						event.peer->address.host,
						event.peer->address.port);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
						event.packet->dataLength,
						event.packet->data,
						event.peer->address.host,
						event.peer->address.port,
						event.channelID);

					// Clean up the packet now that we're done using it. 
					enet_packet_destroy(event.packet);

					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%x:%u disconnected.\n",
						event.peer->address.host,
						event.peer->address.port);
					break;
				}
				
			}
		}
		//GAME LOOP END
		
		enet_host_destroy(server);
		
		return EXIT_SUCCESS;
	}
}
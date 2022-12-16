#include <Server.h>

int main(int argc, char **argv) {
	WSADATA data{};
	SOCKET conn;
	WORD Port;
	SOCKADDR_IN sv_addr{};
	SOCKADDR_IN cl_addr{};

	if (argc != 2) {
		print("Usage: %s: port\n", argv[0]);
		return 0;
	}

	Port = (WORD)atoi(argv[1]);

	WSAStartup(MAKEWORD(2, 2), &data);

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (conn == INVALID_SOCKET) {
		print("Failed to create socket with errorcode: %d\n", WSAGetLastError());
		return -1;
	}

	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port = htons(Port);
	sv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(conn, (SOCKADDR*)&sv_addr, sizeof(sv_addr)) == SOCKET_ERROR) {
		print("An error occured while binding!\nCode: %d\n", WSAGetLastError());
		closesocket(conn);
		return -1;
	}

	std::ifstream ifile("html/test.html", std::ios::in | std::ios::ate);

	size_t len = ifile.tellg();
	ifile.seekg(0, std::ios::beg);

	char* buffer = new char[len + 1];
	buffer[len] = '\0';

	listen(conn, 3);

	while (1) {
		int ret;
		int size = sizeof(cl_addr);

		SOCKET hClient = accept(conn, (SOCKADDR*)&cl_addr, &size);
		char buf[1024];

		if (hClient == INVALID_SOCKET) {
			continue;
		}

		ret = recv(hClient, buf, 1024, 0);

		if (ret != SOCKET_ERROR) {
			buf[ret] = '\0';
			if (!strcmp(buf, "qqq"))
				break;

#ifdef _DEBUG
			print("Message received: %s\n", buf);
#endif

			std::string str((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());

			sprintf(buffer, "HTTP/1.1 200 OK\nContent-length: %llu\nContent-Type: text/html\n\n%s", len, str.c_str());
			send(hClient, buffer, strlen(buffer), 0);

			//delete[] buffer;

			closesocket(conn);
		}
	}

	closesocket(conn);
}
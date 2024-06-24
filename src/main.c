#include"includes.h"

#define BUFFER_SIZE 1024



void handle_client(int client_socket);



int main(){

    printf("Enter the port: ");
    LONG PORT;
    scanf("%d", PORT);

    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2), &wsadata) != 0){perror("WSAStartup failed\n"); return 1;} //Инициализируем использование сокетов в Windows

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){perror("Couldn't create socket\n"); return 1;}//Выдаем сам сокет (если вернет -1, то ошибка)

    struct sockaddr_in server_addr;//настраиваем структуру
    server_addr.sin_family = AF_INET;//Указывает IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;//Храним наш IP аддрес
    server_addr.sin_port = htons(PORT);//Устанавливаем порт который будем слушать

    if(bind(server_socket,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){//Привязываем текущий сокет к нашим данным
        perror("Bind faild");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if(listen(server_socket, 3) < 0){//начинаем слушать сокет
        perror("Listen failed");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Server is listening on port %d\n", PORT);

    while(1){
        struct sockaddr_in client_addr;//храним адресс и порт клиента
        int client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);//Ожидает подключение к server_socket 
        if(client_socket < 0){perror("Accept failed\n"); return 1; continue;}//если подключение установлено, то создается новый сокет
        handle_client(client_socket);
    }

    return 0;
}


void handle_client(int client_socket){
    char buffer[BUFFER_SIZE];
    int recived = recv(client_socket, buffer, BUFFER_SIZE - 1, 0); //Получает данные от клиента
    if(recived < 0){perror("Accept failed\n"); return;}

    buffer[recived] = '\0';//Задаем конец строки в буфере
    printf("Recived request: \n%s\n", buffer);

    char *http_responce = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 44\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>It Works!</h1></body></html>";

    send(client_socket, http_responce, strlen(http_responce), 0);//отправляем данные через сокет клиенту
    closesocket(client_socket);
}
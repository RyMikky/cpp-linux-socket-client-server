#include "server.h"

// Базовая область видимости сетевых компонентов
namespace net {

    namespace server {

        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Инициализация мьютекса

        void *СonnectionHandler(void *socket_desc) {
            int client_socket = *(int*)socket_desc;

            /* Читаем данные из сокета */
            char buffer[__BUFFER_SIZE__];
            ssize_t bytes_received;
            while ((bytes_received = recv(client_socket, buffer, __BUFFER_SIZE__, 0)) > 0) {

                pthread_mutex_lock(&mutex); 
                std::ofstream log(__LOG_FILE__, std::ios::app);
                if (log.is_open()) {
                    log << utils::time::GetCurrentTimeStamp() << " " << std::string(buffer, bytes_received) << std::endl;
                    log.close();
                }
                pthread_mutex_unlock(&mutex);
            }

            if (bytes_received == -1) {
                close(client_socket);
                delete (int*)socket_desc;
                throw std::runtime_error("СonnectionHandler()::ERROR::Data readin error\n");
            }

            close(client_socket);
            delete (int*)socket_desc;
            pthread_exit(NULL);
        }


        Server& Server::SetServerSettins(utils::Settings* settings) {
            if (!InProcess()) {
                Clear();
                
                SetSettins( dynamic_cast<utils::ServerSettings*>(settings));
                return *this;
            }
            throw std::runtime_error("Server::SetServerSettins()::ERROR::Server already in process\n");
        }

        utils::ServerSettings* Server::GetServerSettins() const {
            return dynamic_cast<utils::ServerSettings*>(GetSettins());
        }

        Server::Server(int args, char** argv) 
            : SettingsUser(new utils::ServerSettings(args, argv)) {
            Init().Run();
        }
        

        Server::~Server() {
            Clear();
        }

        Server& Server::Run() {
            if (IsInit() && !InProcess()) {
                Server::_in_process = true;

                return Process();
            }
            return *this;
        }

        Server& Server::Abort() {
            if (InProcess()) {
                Server::_in_process = false;
            }
            return Close();
        }

        Server& Server::Close() {
            if (IsInit() && !InProcess()) {
                close(_socket);
            }
            return *this;
        }

        Server& Server::Init() {
            if (!IsInit() && GetSettins() != nullptr) {
                /* Выполняем инициализацию сокета */
                _socket = socket(AF_INET, SOCK_STREAM, 0);
                if (_socket == -1) {
                    throw std::runtime_error("Server::Init()::ERROR::Socket implementation error\n");
                }

                /* Выполняем инициализацию конечного адреса */
                _endpoint.sin_family = AF_INET;
                _endpoint.sin_addr.s_addr = inet_addr(__LOCAL_HOST__);
                // _endpoint.sin_addr.s_addr = INADDR_ANY;
                _endpoint.sin_port = htons(dynamic_cast<utils::ServerSettings*>(GetSettins())->_port);

                IsInit(true);
                return Bind();
            }
            else {
                return *this;
            }
        }

        Server& Server::Bind() {
            if (IsInit()) {
                /* Привязываем сокет к адресу и порту */
                if (bind(_socket, (struct sockaddr *)&_endpoint, sizeof(_endpoint)) == -1) {
                    perror("");
                    throw std::runtime_error("Server::Bind()::ERROR::Socket binding error\n");
                }

                return Listen();
            }

            throw std::runtime_error("Server::Bind()::ERROR::Socket is not init\n");
        }

        Server& Server::Listen() {
            if (IsInit()) {
                /* Начинаем слушать входящие подключения */
                if (listen(_socket, CONNECTION_REQUST_LIMMIT) == -1) {
                    throw std::runtime_error("Server::Listen()::ERROR::Socket listing prepare error\n");
                }

                return *this;
            }

            throw std::runtime_error("Server::Listen()::ERROR::Socket is not init\n");
        }

        Server& Server::Process() {
            
            while (_in_process) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_socket == -1) {
                    std::cerr << "Server::Process()::ERROR::Incomming connection accept error.\nProcess is continue.\n";
                    continue;
                }

                pthread_t thread;
                int *new_socket = new int;
                *new_socket = client_socket;
                if (pthread_create(&thread, NULL, СonnectionHandler, (void*)new_socket) != 0) {
                    std::cerr << "Server::Process()::ERROR::pthread_create() error.\nProcess is continue.\n";
                    delete new_socket;
                    continue;
                }
                pthread_detach(thread); 
            }

            return *this;
        }

        Server& Server::Clear() {
            if (GetSettins() != nullptr) {
                SetSettins(nullptr);
            }

            return *this;
        }

    } // namespace server

} // namespace net
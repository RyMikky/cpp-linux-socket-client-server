#include "client.h"

// Базовая область видимости сетевых компонентов
namespace net {

    namespace client {

        Client& Client::SetClientSettins(utils::Settings* settings) {
            if (!InProcess()) {
                Clear();
                
                SetSettins( dynamic_cast<utils::ClientSettings*>(settings));
                return *this;
            }
            throw std::runtime_error("Client::SetClientSettins()::ERROR::Client already in process\n");
        }

        utils::ClientSettings* Client::GetClientSettins() const {
            return dynamic_cast<utils::ClientSettings*>(GetSettins());
        }

        Client::Client(int args, char** argv) 
            : SettingsUser(new utils::ClientSettings(args, argv)) {
            Init().Run();
        }
        

        Client::~Client() {
            Clear();
        }

        Client& Client::Run() {
            if (!InProcess()) {
                Client::_in_process = true;

                return Connect().Process();
            }
            return *this;
        }


        Client& Client::Abort() {
            if (InProcess()) {
                Client::_in_process = false;
            }
            return Disconnect();
        }


        Client& Client::Connect() {
            if (!IsInit()) {
                throw std::runtime_error("Client::Connect()::ERROR::Client is not initialize\n");
            }

            if (connect(_socket, reinterpret_cast<struct sockaddr*>(&_endpoint), sizeof(_endpoint)) == -1) {
                throw std::runtime_error("Client::Connect()::ERROR::Conneсt to server is fault\n");
            }
            return *this;
        }
        

        Client& Client::Disconnect() {
            if (IsInit() && !InProcess()) {
                close(_socket);
            }
            return *this;
        }

        Client& Client::Init() {
            if (!IsInit() && GetSettins() != nullptr) {

                /* Выполняем инициализацию сокета */
                _socket = socket(AF_INET, SOCK_STREAM, 0);
                if (_socket == -1) {
                    throw std::runtime_error("Client::Init()::ERROR::Socket implementation error\n");
                }

                /* Выполняем инициализацию конечного адреса */
                _endpoint.sin_family = AF_INET;
                _endpoint.sin_port = htons(dynamic_cast<utils::ClientSettings*>(GetSettins())->_port);
                _endpoint.sin_addr.s_addr = inet_addr(__LOCAL_HOST__); 
                // localhost берем потому что не задаано другое, но в принципе можно брать из настроек

                IsInit(true);    
            }
            return *this;
        }


        Client& Client::Process() {
            while (_in_process) {
                if (send(_socket, dynamic_cast<utils::ClientSettings*>(GetSettins())->_name, 
                    strlen(dynamic_cast<utils::ClientSettings*>(GetSettins())->_name), 0) == -1) {
                    Abort();

                    throw std::runtime_error("Client::Process()::ERROR::Send message is fault\n");
                }

                sleep(dynamic_cast<utils::ClientSettings*>(GetSettins())->_time); // Задержка перед отправкой следующего сообщения
            }

            return *this;
        }

        Client& Client::Clear() {
            if (GetSettins() != nullptr) {
                SetSettins(nullptr);
            }

            return *this;
        }

    } // namespace client

} // namespace net
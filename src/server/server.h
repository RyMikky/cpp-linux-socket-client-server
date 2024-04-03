#pragma once 
#include "../time/time.h"
#include "../arguments/arguments.h"

// Базовая область видимости сетевых компонентов
namespace net {

    // Область видимости компонентов реализации сервера
    namespace server {

        class Server : public utils::SettingsUser {
        private:
            static std::atomic<bool> _in_process;
            const int CONNECTION_REQUST_LIMMIT = 10;

            int _socket = 0;
            sockaddr_in _endpoint = {};
        public:
            // Устанавливает новые параметры подключения
            Server& SetServerSettins(utils::Settings* /*settings*/);
            // Возвращает параметры настроек клиента
            utils::ServerSettings* GetServerSettins() const;

            // Возвращает текущий статус выполнения
            bool InProcess() const { return Server::_in_process; }

            Server() = default;
            Server(const Server&) = delete;
            Server& operator=(const Server&) = delete;
            Server(Server&&) = delete;
            Server& operator=(Server&&) = delete;

            Server(int /*args*/, char** /*argv*/);
            ~Server();

            // Запускает выполнение операций
            Server& Run();

            // Прекращает выполнение операций
            Server& Abort();

        private:

            // Закрытие приёма соединения
            Server& Close();

            // Метод базовой инициализации компонентов
            Server& Init();

            // Метод фиксации сокета на приём соединения о укзанному адресу и порту
            Server& Bind();

            // Запуск прослушивания сокета на входящие соединения
            Server& Listen();

            // Основной метод исполнения
            Server& Process();

            // Удаляет настройки и очищает память
            Server& Clear();
        };

        inline std::atomic<bool> Server::_in_process = false;

    } // namespace server

} // namespace net
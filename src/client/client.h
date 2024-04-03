#pragma once 
#include "../arguments/arguments.h"

// Базовая область видимости сетевых компонентов
namespace net {

    namespace client {

        class Client : public utils::SettingsUser {
        private:
            static std::atomic<bool> _in_process;

            int _socket = 0;
            sockaddr_in _endpoint = {};

        public:
            // Устанавливает новые параметры подключения
            Client& SetClientSettins(utils::Settings* /*settings*/);
            // Возвращает параметры настроек клиента
            utils::ClientSettings* GetClientSettins() const;

            // Возвращает текущий статус выполнения
            bool InProcess() const { return Client::_in_process; }

            Client() = default;
            Client(const Client&) = delete;
            Client& operator=(const Client&) = delete;
            Client(Client&&) = delete;
            Client& operator=(Client&&) = delete;

            Client(int /*args*/, char** /*argv*/);
            ~Client();

            // Запускает выполнение операций
            Client& Run();

            // Прекращает выполнение операций
            Client& Abort();

        private:
            // Подключение к серверу
            Client& Connect();

            // Отключение от сервера
            Client& Disconnect();

            // Метод базовой инициализации компонентов
            Client& Init();

            // Основной метод исполнения
            Client& Process();

            // Удаляет настройки и очищает память
            Client& Clear();
        };

        inline std::atomic<bool> Client::_in_process = false;

    } // namespace client

} // namespace net
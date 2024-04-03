#pragma once
#include "../includes/base_includes.h"

// Область видимости дополнительных компонентов
namespace utils {

    class Settings {
    public:
        Settings() = delete;
        Settings(const Settings&) = delete;
        Settings& operator=(const Settings&) = delete;

        Settings(Settings&&) = default;
        Settings& operator=(Settings&&) = default;
        virtual ~Settings() = default;

        // Возвращает количество сохраненных параметров
        int GetParamCount() const;

        // Возвращает записанный ппараметр по индексу
        // Выбросит std::out_of_range если индекс некорректный
        char* GetParamByIndex(int /*idx*/) const;

    protected:

        // Конструктор записывает основные параметры командной строки, клоторые можнно потом прочитать
        Settings(int /*count*/, int /*args*/, char** /*argv*/);

        // Храним ппараметры командной строки
        std::vector<char*> _args = {};
        
    };

    bool operator== (const Settings& /*lhs*/, const Settings& /*rhs*/);
    bool operator!= (const Settings& /*lhs*/, const Settings& /*rhs*/);

    class ClientSettings : public Settings {
    private:
        static int _COUNT;
    public:
        ClientSettings(int /*args*/, char** /*argv*/);

        // Имя клиента
        const char* _name;
        // Порт подключения к серверу
        const int _port;
        // Период обращения в секундах
        const int _time;
    };

    inline int ClientSettings::_COUNT = 3;

    class ServerSettings : public Settings {
    private:
        static int _COUNT;
    public:
        ServerSettings(int /*args*/, char** /*argv*/);

        // Порт просслушивания сервера
        const int _port;
    };

    inline int ServerSettings::_COUNT = 1;

    // Класс использующий настройки
    class SettingsUser {
    private:
        utils::Settings* _settings = nullptr;
        bool _is_init = false;
    public:
        SettingsUser() = delete;
        SettingsUser(const SettingsUser&) = delete;
        SettingsUser& operator=(const SettingsUser&) = delete;
        SettingsUser(SettingsUser&&) = delete;
        SettingsUser& operator=(SettingsUser&&) = delete;

        ~SettingsUser();

        explicit SettingsUser(utils::Settings* /*settings*/);
        // Задаёт новые параметры настроек
        SettingsUser& SetSettins(utils::Settings* /*settings*/);
        // Возвращает параметры настроек
        utils::Settings* GetSettins() const;

        // Возвращает текущий статус инициализации
        inline bool IsInit() const { return _is_init; }
        // Переназначает флаг иницилизации
        inline bool IsInit(bool is_init) { return (this->_is_init = is_init); }
    };

} // namespace utils
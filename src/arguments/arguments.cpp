#include "arguments.h"

// Область видимости дополнительных компонентов
namespace utils {

    int Settings::GetParamCount() const {
        return _args.size();
    }

    char* Settings::GetParamByIndex(int idx) const {
        if (idx >= 0 && idx < GetParamCount()) {
            return _args[idx];
        }
        throw std::out_of_range("Settings::GetParamByIndex()::ERROR::Income index is out of range\n");
    }

    Settings::Settings(int count, int args, char** argv) {
        if (count != args - 1) {
            throw std::runtime_error("Settings()::ERROR::Arguments count is incorrect\n");
        }

        _args.resize(count);
        for (int i = 1; i != args; ++i){
            _args[i - 1] = argv[i];
        }
    }

    bool operator==(const Settings& lhs, const Settings& rhs) {
        if (lhs.GetParamCount() != rhs.GetParamCount()) {
            return false;
        }
        else {
            for (int i = 0; i != lhs.GetParamCount(); ++i) {
                
                if (strcmp(lhs.GetParamByIndex(i), rhs.GetParamByIndex(i))) {
                    return false;
                }
            }

            return true;
        }
    }
    bool operator!=(const Settings& lhs, const Settings& rhs) {
        return !(lhs == rhs);
    }

    /*
        Релизация неоптимальна, есть "магиеческие числа", 
        можно отказаться от "сложностей" по поводу создания класса настроек в принципе
        Но вся "внутрянка" парсинга инкапсулирована, тогда как сами настройки имеют только 
        открытые и готовые к использованию константные поля 
    */

    ClientSettings::ClientSettings(int args, char** argv) 
        : Settings(_COUNT, args, argv)
        , _name(_args[0])
        , _port(std::stoi(_args[1]))
        , _time(std::stoi(_args.back())) {
    }

    ServerSettings::ServerSettings(int args, char** argv) 
        : Settings(_COUNT, args, argv), _port(std::stoi(_args.back())) {
    }

    SettingsUser::SettingsUser(utils::Settings* settings) 
        : _settings(settings) {
    }

    SettingsUser::~SettingsUser() {
        if (_settings != nullptr) {
            delete _settings;
            _settings = nullptr;
        }
    }
    
    SettingsUser& SettingsUser::SetSettins(utils::Settings* settings) {
        if (*_settings != *settings) {

            if (_settings != nullptr) {
                delete _settings;
            }

            _settings = settings;
            IsInit(false);
        }
        return *this;
    }

    utils::Settings* SettingsUser::GetSettins() const {
        return _settings;
    }

} // namespace utils
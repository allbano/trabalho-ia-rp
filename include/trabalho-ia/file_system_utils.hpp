#ifndef FILE_SYSTEM_UTILS_HPP
#define FILE_SYSTEM_UTILS_HPP

#include <string>
#include <vector>

namespace FileSystemUtils {
    /**
     * @brief Tenta obter o caminho dos dados via argumento de linha de comando ou interação com o usuário.
     * * 1. Verifica se argv[1] foi passado e é válido.
     * 2. Se não, entra em loop pedindo ao usuário o caminho absoluto.
     * 3. Verifica se os arquivos essenciais (WTEXpc.dat, etc) existem dentro desse caminho.
     * * @param argc Contagem de argumentos do main
     * @param argv Vetor de argumentos do main
     * @return std::string O caminho absoluto validado.
     */
    std::string resolveDataPath(int argc, char* argv[]);

    /**
     * @brief Valida se os arquivos necessários existem no diretório.
     * @return true se todos existirem, false caso contrário.
     */
    bool validateRequiredFiles(const std::string& basePath);
}

#endif // FILE_SYSTEM_UTILS_HPP

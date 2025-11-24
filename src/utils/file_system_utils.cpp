#include "../../include/trabalho-ia/file_system_utils.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>

namespace fs = std::filesystem;

namespace FileSystemUtils {

    // Lista de arquivos que OBRIGATORIAMENTE devem existir
    const std::vector<std::string> REQUIRED_FILES = {
        "WTEXpc.dat", 
        "CLtx.dat", 
        "PALAVRASpc.txt", 
        "WWRDpc.dat"
    };

    // Função auxiliar privada (apenas dentro deste .cpp) para limpar aspas
    std::string sanitizePathString(std::string pathStr) {
        pathStr.erase(std::remove(pathStr.begin(), pathStr.end(), '\"'), pathStr.end());
        pathStr.erase(std::remove(pathStr.begin(), pathStr.end(), '\''), pathStr.end());
        return pathStr;
    }

    bool validateRequiredFiles(const std::string& basePath) {
        fs::path dir(basePath);
        bool allFound = true;

        for (const auto& file : REQUIRED_FILES) {
            fs::path fullPath = dir / file;
            if (!fs::exists(fullPath)) {
                std::cerr << "[Erro] Arquivo nao encontrado: " << fullPath.string() << std::endl;
                allFound = false;
            }
        }
        return allFound;
    }

    std::string resolveDataPath(int argc, char* argv[]) {
        std::string selectedPath;

        // 1. Tentativa via Argumento de Linha de Comando
        if (argc > 1) {
            std::string argPath = sanitizePathString(argv[1]);
            fs::path dir(argPath);

            if (fs::exists(dir) && fs::is_directory(dir)) {
                // Verifica se os arquivos estão lá
                if (validateRequiredFiles(argPath)) {
                    std::cout << "[Config] Caminho valido recebido via argumento." << std::endl;
                    return fs::absolute(dir).string();
                }
            } 
            
            std::cerr << "[Aviso] O caminho passado via argumento e invalido ou incompleto." << std::endl;
            std::cerr << "Entrando em modo de selecao manual...\n" << std::endl;
        }

        // 2. Tentativa via Interação com Usuário (Loop)
        while (true) {
            std::cout << "Insira o CAMINHO ABSOLUTO da pasta contendo os arquivos de dados:\n> ";
            std::getline(std::cin, selectedPath);

            if (selectedPath.empty()) continue;

            selectedPath = sanitizePathString(selectedPath);

            try {
                fs::path dir(selectedPath);
                if (fs::exists(dir) && fs::is_directory(dir)) {
                    if (validateRequiredFiles(selectedPath)) {
                        return fs::absolute(dir).string();
                    } else {
                        std::cerr << "[Erro] A pasta existe, mas faltam arquivos essenciais (verifique acima).\n";
                    }
                } else {
                    std::cerr << "[Erro] Diretorio nao encontrado.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "[Erro] Excecao ao processar caminho: " << e.what() << "\n";
            }
        }
    }
}

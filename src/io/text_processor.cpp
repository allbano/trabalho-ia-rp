#include "../../include/trabalho-ia/text_processor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype> // Para toupper

using namespace Config;

void TextProcessor::loadVocabulary(const std::string& wordFile, const std::string& vecFile) {
    std::ifstream wf(wordFile);
    std::ifstream vf(vecFile);
    
    if (!wf.is_open() || !vf.is_open()) {
        std::cerr << "[IO Error] Erro ao abrir arquivos de vocabulario." << std::endl;
        exit(1);
    }

    std::string word, line;
    int count = 0;
    
    while (std::getline(wf, word)) {
        // Limpeza de carriage return (\r) do Windows/CRLF
        word.erase(std::remove(word.begin(), word.end(), '\r'), word.end());
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        
        // O arquivo PALAVRASpc.txt já é maiúsculo, mas garantimos a consistência
        // Nenhuma ação necessária se já for maiúsculo, mas removemos espaços extras
        
        std::vector<double> vec;
        if (std::getline(vf, line)) {
            std::stringstream ss(line);
            double val;
            while (ss >> val) {
                vec.push_back(val);
            }
        }
        
        if (vec.size() == INPUT_SIZE) {
            // Normalizamos as palavras base também? Geralmente ajuda.
            // Math::normalizeVector(vec); (Opcional, mas recomendado testar)
            wordVectors[word] = vec;
            count++;
        }
    }
    std::cout << "[TextProcessor] Vocabulario carregado: " << count << " palavras." << std::endl;
}

Dataset TextProcessor::loadTrainingData(const std::string& featureFile, const std::string& labelFile) {
    Dataset data;
    std::ifstream fFeat(featureFile);
    std::ifstream fLabel(labelFile);

    if (!fFeat.is_open() || !fLabel.is_open()) {
        std::cerr << "[IO Error] Erro ao abrir arquivos de treino." << std::endl;
        exit(1);
    }

    std::string lineF, lineL;
    while (std::getline(fFeat, lineF) && std::getline(fLabel, lineL)) {
        std::vector<double> vec;
        std::stringstream ss(lineF);
        double val;
        while (ss >> val) vec.push_back(val);

        // Parse da label (notação científica ou inteira)
        double label = std::stod(lineL);

        if (vec.size() == INPUT_SIZE) {
            // IMPORTANTE: Normalizar o vetor de entrada do treinamento
            Math::normalizeVector(vec);
            
            data.inputs.push_back(vec);
            data.targets.push_back(label);
        }
    }
    std::cout << "[TextProcessor] Dados de treino carregados e normalizados: " << data.inputs.size() << " exemplos." << std::endl;
    return data;
}

std::vector<double> TextProcessor::textToVector(std::string text) {
    std::vector<double> avgVec(INPUT_SIZE, 0.0);
    int foundWords = 0;

    std::stringstream ss(text);
    std::string token;
    
    while (ss >> token) {
        // Remove pontuação
        token.erase(std::remove_if(token.begin(), token.end(), ::ispunct), token.end());
        
        // CORREÇÃO: Transforma para UPPERCASE para dar match com PALAVRASpc.txt
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);

        if (wordVectors.count(token)) {
            const auto& v = wordVectors[token];
            for (int i = 0; i < INPUT_SIZE; ++i) {
                avgVec[i] += v[i];
            }
            foundWords++;
        }
    }

    if (foundWords > 0) {
        // Média
        for (int i = 0; i < INPUT_SIZE; ++i) {
            avgVec[i] /= foundWords;
        }
        // IMPORTANTE: Normaliza o vetor resultante do usuário também
        Math::normalizeVector(avgVec);
    }
    
    return avgVec;
}
#include "../../include/trabalho-ia/text_processor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace Config;

void TextProcessor::loadVocabulary(const std::string& wordFile, const std::string& vecFile) {
    std::ifstream wf(wordFile);
    std::ifstream vf(vecFile);
    
    if (!wf.is_open() || !vf.is_open()) {
        std::cerr << "[IO Error] Erro ao abrir arquivos de vocabulario (" << wordFile << " ou " << vecFile << ")" << std::endl;
        exit(1);
    }

    std::string word, line;
    int count = 0;
    
    while (std::getline(wf, word)) {
        // Remove carriage return (comum em arquivos Windows/DOS)
        word.erase(std::remove(word.begin(), word.end(), '\r'), word.end());
        
        std::vector<double> vec;
        if (std::getline(vf, line)) {
            std::stringstream ss(line);
            double val;
            while (ss >> val) {
                vec.push_back(val);
            }
        }
        
        if (vec.size() == INPUT_SIZE) {
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

        double label = std::stod(lineL);

        if (vec.size() == INPUT_SIZE) {
            data.inputs.push_back(vec);
            data.targets.push_back(label);
        }
    }
    std::cout << "[TextProcessor] Dados de treino carregados: " << data.inputs.size() << " exemplos." << std::endl;
    return data;
}

std::vector<double> TextProcessor::textToVector(std::string text) {
    std::vector<double> avgVec(INPUT_SIZE, 0.0);
    int foundWords = 0;

    std::stringstream ss(text);
    std::string token;
    
    while (ss >> token) {
        // Limpeza básica
        token.erase(std::remove_if(token.begin(), token.end(), ::ispunct), token.end());
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);

        if (wordVectors.count(token)) {
            const auto& v = wordVectors[token];
            for (int i = 0; i < INPUT_SIZE; ++i) {
                avgVec[i] += v[i];
            }
            foundWords++;
        }
    }

    if (foundWords > 0) {
        for (int i = 0; i < INPUT_SIZE; ++i) {
            avgVec[i] /= foundWords;
        }
    }
    
    return avgVec;
}

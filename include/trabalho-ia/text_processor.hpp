#ifndef TEXT_PROCESSOR_HPP
#define TEXT_PROCESSOR_HPP

#include "utils.hpp"
#include <string>
#include <map>
#include <vector>

class TextProcessor {
private:
    std::map<std::string, std::vector<double>> wordVectors;

public:
    // Carrega o vocabulário e vetores (PALAVRASpc.txt e WWRDpc.dat)
    void loadVocabulary(const std::string& wordFile, const std::string& vecFile);

    // Carrega dados de treinamento prontos (WTEXpc.dat e CLtx.dat)
    Dataset loadTrainingData(const std::string& featureFile, const std::string& labelFile);

    // Converte texto livre em vetor usando média de embeddings
    std::vector<double> textToVector(std::string text);
};

#endif // TEXT_PROCESSOR_HPP

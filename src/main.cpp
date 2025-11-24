/**
 * Trabalho de Inteligência Computacional Aplicada I
 * Classificador de Sentimentos
 * * Estrutura do Main:
 * 1. Configuração de Ambiente (via FileSystemUtils)
 * 2. Pipeline de Dados (via TextProcessor)
 * 3. Treinamento (via NeuralNetwork)
 * 4. Inferência (Interativa)
 */

#include <iostream>
#include <iomanip>
#include <filesystem> // Usado apenas para concatenar paths finais

// Nossas bibliotecas modulares
#include "../include/trabalho-ia/utils.hpp"
#include "../include/trabalho-ia/file_system_utils.hpp"
#include "../include/trabalho-ia/neural_network.hpp"
#include "../include/trabalho-ia/text_processor.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::cout << "=== Sistema de Classificacao de Comentarios - UFPR DS803 ===" << std::endl;

    // ---------------------------------------------------------
    // 1. RESOLUÇÃO DO AMBIENTE (Nova chamada única)
    // ---------------------------------------------------------
    // Esta função encapsula toda a lógica de validar argv, 
    // limpar strings e pedir input ao usuário se necessário.
    std::string basePath = FileSystemUtils::resolveDataPath(argc, argv);
    
    std::cout << "\n[Main] Iniciando sistema com dados em: " << basePath << std::endl;

    // Construção segura dos caminhos
    fs::path root(basePath);
    std::string fWtex = (root / "WTEXpc.dat").string();
    std::string fCltx = (root / "CLtx.dat").string();
    std::string fPalavras = (root / "PALAVRASpc.txt").string();
    std::string fWwrd = (root / "WWRDpc.dat").string();

    // ---------------------------------------------------------
    // 2. INSTANCIAÇÃO E CARGA DE DADOS
    // ---------------------------------------------------------
    TextProcessor processor;
    NeuralNetwork nn;

    Dataset trainData = processor.loadTrainingData(fWtex, fCltx);
    processor.loadVocabulary(fPalavras, fWwrd);

    // ---------------------------------------------------------
    // 3. TREINAMENTO
    // ---------------------------------------------------------
    nn.train(trainData.inputs, trainData.targets);

    // Verificação de Acurácia (Sanity Check)
    int correct = 0;
    for (size_t i = 0; i < trainData.inputs.size(); ++i) {
        double output = nn.predict(trainData.inputs[i]);
        int predicted = output > 0.5 ? 1 : 0;
        if (predicted == (int)trainData.targets[i]) correct++;
    }
    std::cout << "Acuracia (Dados de Treino): " 
              << (double)correct / trainData.inputs.size() * 100.0 << "%" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    // ---------------------------------------------------------
    // 4. LOOP DE INFERÊNCIA (Usuário)
    // ---------------------------------------------------------
    std::cout << "\n=== FERRAMENTA DE CLASSIFICACAO EM TEMPO REAL ===" << std::endl;
    std::cout << "Digite 'sair' para encerrar." << std::endl;

    std::string userInput;
    while (true) {
        std::cout << "\nComentario > ";
        std::getline(std::cin, userInput);

        if (userInput == "sair") break;
        if (userInput.empty()) continue;

        std::vector<double> inputVector = processor.textToVector(userInput);
        double score = nn.predict(inputVector);
        
        std::cout << "Analise: " << std::fixed << std::setprecision(4) << score;
        std::cout << (score > 0.5 ? " -> POSITIVO (+)" : " -> NEGATIVO (-)") << std::endl;
    }

    return 0;
}

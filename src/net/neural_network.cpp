#include "../../include/trabalho-ia/neural_network.hpp"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

using namespace Config;

NeuralNetwork::NeuralNetwork() : weightClass0(1.0), weightClass1(1.0) {
    initializeWeights();
}

void NeuralNetwork::initializeWeights() {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Xavier Initialization (melhor que aleatório puro para tanh/sigmoid)
    double limit = std::sqrt(6.0 / (INPUT_SIZE + HIDDEN_SIZE)); 
    std::uniform_real_distribution<> dis(-limit, limit);

    weightsInputHidden.resize(INPUT_SIZE, std::vector<double>(HIDDEN_SIZE));
    biasHidden.resize(HIDDEN_SIZE);
    weightsHiddenOutput.resize(HIDDEN_SIZE, std::vector<double>(OUTPUT_SIZE));
    biasOutput.resize(OUTPUT_SIZE);

    for (auto& row : weightsInputHidden) for (auto& w : row) w = dis(gen);
    for (auto& b : biasHidden) b = dis(gen);
    for (auto& row : weightsHiddenOutput) for (auto& w : row) w = dis(gen);
    for (auto& b : biasOutput) b = dis(gen);
}

double NeuralNetwork::predict(const std::vector<double>& input) {
    // Camada Oculta
    std::vector<double> hidden(HIDDEN_SIZE);
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        double sum = biasHidden[i];
        for (int j = 0; j < INPUT_SIZE; ++j) {
            sum += input[j] * weightsInputHidden[j][i];
        }
        hidden[i] = Math::sigmoid(sum);
    }

    // Camada de Saída
    double output = biasOutput[0];
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        output += hidden[i] * weightsHiddenOutput[i][0];
    }
    return Math::sigmoid(output);
}

void NeuralNetwork::train(const std::vector<std::vector<double>>& inputs, const std::vector<double>& targets) {
    
    // --- 1. Calcular Balanceamento de Classes Automaticamente ---
    int count0 = 0;
    int count1 = 0;
    for (double t : targets) {
        if (t < 0.5) count0++; else count1++;
    }

    // Evita divisão por zero
    if (count0 == 0) count0 = 1;
    if (count1 == 0) count1 = 1;

    double total = (double)(count0 + count1);
    
    // Heurística de pesos inversamente proporcionais
    // Se a classe 1 (ódio) for rara, weightClass1 será alto.
    weightClass0 = total / (2.0 * count0);
    weightClass1 = total / (2.0 * count1);

    std::cout << "[NeuralNet] Balanceamento de Classes detectado:" << std::endl;
    std::cout << "   Classe 0 (Negativos?): " << count0 << " amostras | Peso: " << weightClass0 << std::endl;
    std::cout << "   Classe 1 (Positivos?): " << count1 << " amostras | Peso: " << weightClass1 << std::endl;
    std::cout << "   Iniciando treinamento por " << EPOCHS << " epocas..." << std::endl;
    
    // --- 2. Loop de Treinamento ---
    for (int epoch = 0; epoch < EPOCHS; ++epoch) {
        double totalError = 0.0;

        for (size_t i = 0; i < inputs.size(); ++i) {
            // Forward Pass
            std::vector<double> hidden(HIDDEN_SIZE);
            for (int h = 0; h < HIDDEN_SIZE; ++h) {
                double sum = biasHidden[h];
                for (int in = 0; in < INPUT_SIZE; ++in) {
                    sum += inputs[i][in] * weightsInputHidden[in][h];
                }
                hidden[h] = Math::sigmoid(sum);
            }

            double outputSum = biasOutput[0];
            for (int h = 0; h < HIDDEN_SIZE; ++h) {
                outputSum += hidden[h] * weightsHiddenOutput[h][0];
            }
            double output = Math::sigmoid(outputSum);

            // --- AQUI ESTÁ O TRUQUE: Weighted Error ---
            double rawError = targets[i] - output;
            
            // Aplicamos o peso dependendo da classe alvo real
            double sampleWeight = (targets[i] > 0.5) ? weightClass1 : weightClass0;
            
            // O erro é amplificado se for da classe minoritária
            double weightedError = rawError * sampleWeight;

            totalError += std::abs(rawError); // Para log visual, usamos o erro real
            
            // Backward Pass (usamos o weightedError para o gradiente)
            double dOutput = weightedError * Math::sigmoidDerivative(output);

            for (int h = 0; h < HIDDEN_SIZE; ++h) {
                weightsHiddenOutput[h][0] += LEARNING_RATE * dOutput * hidden[h];
            }
            biasOutput[0] += LEARNING_RATE * dOutput;

            for (int h = 0; h < HIDDEN_SIZE; ++h) {
                double errorHidden = dOutput * weightsHiddenOutput[h][0];
                double dHidden = errorHidden * Math::sigmoidDerivative(hidden[h]);

                for (int in = 0; in < INPUT_SIZE; ++in) {
                    weightsInputHidden[in][h] += LEARNING_RATE * dHidden * inputs[i][in];
                }
                biasHidden[h] += LEARNING_RATE * dHidden;
            }
        }

        if (epoch % 100 == 0) {
            std::cout << "Epoca " << epoch << " | Erro Medio (Absoluto): " << totalError / inputs.size() << std::endl;
        }
    }
}
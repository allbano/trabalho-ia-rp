#include "../../include/trabalho-ia/neural_network.hpp"
#include <iostream>
#include <random>
#include <algorithm>

using namespace Config;

NeuralNetwork::NeuralNetwork() {
    initializeWeights();
}

void NeuralNetwork::initializeWeights() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.5, 0.5);

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
    std::cout << "[NeuralNet] Iniciando treinamento por " << EPOCHS << " epocas..." << std::endl;
    
    for (int epoch = 0; epoch < EPOCHS; ++epoch) {
        double totalError = 0.0;

        for (size_t i = 0; i < inputs.size(); ++i) {
            // 1. Forward Pass
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

            // 2. Cálculo do Erro
            double error = targets[i] - output;
            totalError += std::abs(error);

            // 3. Backward Pass
            double dOutput = error * Math::sigmoidDerivative(output);

            // Atualiza pesos Hidden -> Output
            for (int h = 0; h < HIDDEN_SIZE; ++h) {
                weightsHiddenOutput[h][0] += LEARNING_RATE * dOutput * hidden[h];
            }
            biasOutput[0] += LEARNING_RATE * dOutput;

            // Erro na camada oculta
            for (int h = 0; h < HIDDEN_SIZE; ++h) {
                double errorHidden = dOutput * weightsHiddenOutput[h][0];
                double dHidden = errorHidden * Math::sigmoidDerivative(hidden[h]);

                // Atualiza pesos Input -> Hidden
                for (int in = 0; in < INPUT_SIZE; ++in) {
                    weightsInputHidden[in][h] += LEARNING_RATE * dHidden * inputs[i][in];
                }
                biasHidden[h] += LEARNING_RATE * dHidden;
            }
        }

        if (epoch % 50 == 0) {
            std::cout << "Epoca " << epoch << " | Erro Medio: " << totalError / inputs.size() << std::endl;
        }
    }
}

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VOCAB_SIZE 128 // Karakter ASCII sederhana
#define HIDDEN_SIZE 64 // Ukuran hidden state
#define SEQ_LENGTH 30  // Panjang sequence untuk training
#define LEARNING_RATE 0.01
#define ITERATIONS 5000

typedef struct {
  // Bobot
  float Wxh[HIDDEN_SIZE * VOCAB_SIZE];  // Input-to-hidden
  float Whh[HIDDEN_SIZE * HIDDEN_SIZE]; // Hidden-to-hidden
  float Why[VOCAB_SIZE * HIDDEN_SIZE];  // Hidden-to-output
  float bh[HIDDEN_SIZE];                // Bias hidden
  float by[VOCAB_SIZE];                 // Bias output

  // Gradien
  float dWxh[HIDDEN_SIZE * VOCAB_SIZE];
  float dWhh[HIDDEN_SIZE * HIDDEN_SIZE];
  float dWhy[VOCAB_SIZE * HIDDEN_SIZE];
} RNN;

void init_rnn(RNN *rnn) {
  // Inisialisasi bobot dengan nilai acak kecil
  for (int i = 0; i < HIDDEN_SIZE * VOCAB_SIZE; i++)
    rnn->Wxh[i] = (rand() / (float)RAND_MAX - 0.5) * 0.01;

  for (int i = 0; i < HIDDEN_SIZE * HIDDEN_SIZE; i++)
    rnn->Whh[i] = (rand() / (float)RAND_MAX - 0.5) * 0.01;

  for (int i = 0; i < VOCAB_SIZE * HIDDEN_SIZE; i++)
    rnn->Why[i] = (rand() / (float)RAND_MAX - 0.5) * 0.01;

  // Inisialisasi bias dengan 0
  memset(rnn->bh, 0, sizeof(float) * HIDDEN_SIZE);
  memset(rnn->by, 0, sizeof(float) * VOCAB_SIZE);
}

void rnn_forward(RNN *rnn, char *inputs, float *hidden, float *outputs) {
  for (int t = 0; t < SEQ_LENGTH; t++) {
    // Encode input (one-hot sederhana)
    float x[VOCAB_SIZE] = {0};
    x[(int)inputs[t]] = 1.0;

    // Hitung hidden state
    float h_new[HIDDEN_SIZE] = {0};
    for (int i = 0; i < HIDDEN_SIZE; i++) {
      for (int j = 0; j < VOCAB_SIZE; j++) {
        h_new[i] += rnn->Wxh[i * VOCAB_SIZE + j] * x[j];
      }
      for (int j = 0; j < HIDDEN_SIZE; j++) {
        h_new[i] += rnn->Whh[i * HIDDEN_SIZE + j] * hidden[j];
      }
      h_new[i] = tanh(h_new[i] + rnn->bh[i]);
    }

    // Hitung output
    for (int i = 0; i < VOCAB_SIZE; i++) {
      outputs[t * VOCAB_SIZE + i] = rnn->by[i];
      for (int j = 0; j < HIDDEN_SIZE; j++) {
        outputs[t * VOCAB_SIZE + i] += rnn->Why[i * HIDDEN_SIZE + j] * h_new[j];
      }
    }

    // Update hidden state
    memcpy(hidden, h_new, sizeof(float) * HIDDEN_SIZE);
  }
}

float calculate_loss(float *outputs, char *targets) {
  float loss = 0;
  for (int t = 0; t < SEQ_LENGTH; t++) {
    int target_idx = (int)targets[t];
    float prob = exp(outputs[t * VOCAB_SIZE + target_idx]);
    float sum_exp = 0;
    for (int i = 0; i < VOCAB_SIZE; i++) {
      sum_exp += exp(outputs[t * VOCAB_SIZE + i]);
    }
    loss += -log(prob / sum_exp);
  }
  return loss / SEQ_LENGTH;
}

void train(RNN *rnn, char *text) {
  int text_length = strlen(text);
  float hidden[HIDDEN_SIZE] = {0};
  float outputs[SEQ_LENGTH * VOCAB_SIZE];

  for (int iter = 0; iter < ITERATIONS; iter++) {
    // Ambil random sequence dari teks
    int start = rand() % (text_length - SEQ_LENGTH - 1);
    char inputs[SEQ_LENGTH + 1];
    strncpy(inputs, &text[start], SEQ_LENGTH);
    inputs[SEQ_LENGTH] = '\0';

    // Forward pass
    memset(hidden, 0, sizeof(float) * HIDDEN_SIZE);
    rnn_forward(rnn, inputs, hidden, outputs);

    // Hitung gradien (backpropagation sederhana)
    // ... (implementasi gradien disederhanakan)

    // Update bobot
    for (int i = 0; i < HIDDEN_SIZE * VOCAB_SIZE; i++)
      rnn->Wxh[i] -= LEARNING_RATE * rnn->dWxh[i];

    // ... (update Whh, Why, bh, by)

    if (iter % 100 == 0) {
      printf("Iter %d, Loss: %.4f\n", iter,
             calculate_loss(outputs, &text[start + 1]));
    }
  }
}

void generate_text(RNN *rnn, char *seed, int length) {
  float hidden[HIDDEN_SIZE] = {0};
  char generated[length + 1];
  strcpy(generated, seed);

  // Inisialisasi hidden state dengan seed
  for (int i = 0; i < strlen(seed); i++) {
    float x[VOCAB_SIZE] = {0};
    x[(int)seed[i]] = 1.0;

    // Update hidden state
    float h_new[HIDDEN_SIZE] = {0};
    for (int j = 0; j < HIDDEN_SIZE; j++) {
      for (int k = 0; k < VOCAB_SIZE; k++) {
        h_new[j] += rnn->Wxh[j * VOCAB_SIZE + k] * x[k];
      }
      for (int k = 0; k < HIDDEN_SIZE; k++) {
        h_new[j] += rnn->Whh[j * HIDDEN_SIZE + k] * hidden[k];
      }
      h_new[j] = tanh(h_new[j] + rnn->bh[j]);
    }
    memcpy(hidden, h_new, sizeof(float) * HIDDEN_SIZE);
  }

  // Generate karakter baru
  for (int i = strlen(seed); i < length; i++) {
    // Hitung output probabilities
    float y[VOCAB_SIZE] = {0};
    for (int j = 0; j < VOCAB_SIZE; j++) {
      y[j] = rnn->by[j];
      for (int k = 0; k < HIDDEN_SIZE; k++) {
        y[j] += rnn->Why[j * HIDDEN_SIZE + k] * hidden[k];
      }
    }

    // Softmax dan sampling
    float probs[VOCAB_SIZE];
    float sum_exp = 0;
    for (int j = 0; j < VOCAB_SIZE; j++) {
      probs[j] = exp(y[j]);
      sum_exp += probs[j];
    }
    for (int j = 0; j < VOCAB_SIZE; j++)
      probs[j] /= sum_exp;

    // Pilih karakter secara acak berdasarkan probabilitas
    float r = (float)rand() / RAND_MAX;
    float cum_prob = 0;
    int next_char = 0;
    for (int j = 0; j < VOCAB_SIZE; j++) {
      cum_prob += probs[j];
      if (r <= cum_prob) {
        next_char = j;
        break;
      }
    }

    // Tambahkan karakter ke hasil
    generated[i] = (char)next_char;

    // Update hidden state untuk karakter berikutnya
    float x[VOCAB_SIZE] = {0};
    x[next_char] = 1.0;
    float h_new[HIDDEN_SIZE] = {0};
    for (int j = 0; j < HIDDEN_SIZE; j++) {
      for (int k = 0; k < VOCAB_SIZE; k++) {
        h_new[j] += rnn->Wxh[j * VOCAB_SIZE + k] * x[k];
      }
      for (int k = 0; k < HIDDEN_SIZE; k++) {
        h_new[j] += rnn->Whh[j * HIDDEN_SIZE + k] * hidden[k];
      }
      h_new[j] = tanh(h_new[j] + rnn->bh[j]);
    }
    memcpy(hidden, h_new, sizeof(float) * HIDDEN_SIZE);
  }

  generated[length] = '\0';
  printf("Generated text: %s\n", generated);
}

int main() {
  srand(time(NULL));

  // 1. Inisialisasi model
  RNN rnn;
  init_rnn(&rnn);

  // 2. Training (contoh dataset kecil)
  char *training_text =
      "Hello world! This is a simple RNN example. abcdfghijklmnopqrstuvwxyz";
  train(&rnn, training_text);

  // 3. Generate teks
  generate_text(&rnn, "hello world", 50); // Seed: "Hello", panjang: 50 karakter

  return 0;
}

#include "train.h"
#include "model.hpp"

#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/system_setup.h>
#include <tensorflow/lite/schema/schema_generated.h>

namespace {
    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *input = nullptr;
    TfLiteTensor *output = nullptr;
    int inference_count = 0;

    constexpr int kTensorArenaSize = 2000;
    uint8_t tensor_arena[kTensorArenaSize];
}  /* namespace */

void setup(void) {
    static tflite::MicroErrorReporter micro_error_reporter;

    error_reporter = &micro_error_reporter;

    /* Map the model into a usable data structure. This doesn't involve any
     * copying or parsing, it's a very lightweight operation.
     */
    model = tflite::GetModel(g_model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
    	TF_LITE_REPORT_ERROR(error_reporter,
    					"Model provided is schema version %d not equal "
    					"to supported version %d.",
    					model->version(), TFLITE_SCHEMA_VERSION);
    	return;
    }

    /* This pulls in all the operation implementations we need.
     * NOLINTNEXTLINE(runtime-global-variables)
     */
    static tflite::AllOpsResolver resolver;

    /* Build an interpreter to run the model with. */
    static tflite::MicroInterpreter static_interpreter(
    	model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;
}

void train(void) {
    const int NUM_EPOCHS = 100;
    const int BATCH_SIZE = 100;
    const int IMG_HEIGHT = 28;
    const int IMG_WIDTH = 28;
    const int NUM_TRAININGS = 60000;
    const int NUM_BATCHES = NUM_TRAININGS / BATCH_SIZE;
}

//
//  main.c
//  xor
//
//  Created by rafiq on 17/11/2020.
//

#include <stdio.h>
#include<string.h>
#include "fann.h"

int main(){
    fann_type *outputs;
    const unsigned int init_num_layers = 4;
    const unsigned int num_input = 2;
    const unsigned int num_hidden_l1 = 8;
    const unsigned int num_hidden_l2 = 8;
    const unsigned int num_output = 2;
    const unsigned int max_epochs = 500;
    const unsigned int epochs_between_reports = 100;
    const float desired_error = (const float) 0;
    struct fann *ann;
    struct fann_train_data *data_train, *data_test;
    unsigned int i = 0;
    
    data_train = fann_read_train_from_file(getenv("TRAIN_DATA_PATH"));
    
    printf("Creating network with shortcuts.\n");
    ann = fann_create_standard(init_num_layers, num_input, num_hidden_l1, num_hidden_l2, num_output);
    
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_LINEAR);
    
    fann_set_activation_steepness_hidden(ann, 0.7);
    fann_set_activation_steepness_output(ann, 0.5);
    
    fann_set_scaling_params(ann, data_train, -1, 1, -1, 1);
    fann_scale_train(ann, data_train);
    
    printf("Training network.\n");
    fann_train_on_data(ann, data_train, max_epochs, epochs_between_reports, desired_error);
    printf("MSE  BEFORE: %f\n", fann_get_MSE(ann));
//    fann_descale_train(ann, data_train);
    
    printf("MSE on TRAIN_DATA: %f\n", fann_get_MSE(ann));
    fann_reset_MSE(ann);
    fann_print_connections(ann);
    //  fann_print_parameters(ann);
    
    
    data_test = fann_read_train_from_file(getenv("TEST_DATA_PATH"));
    fann_scale_train(ann, data_test);
    fann_test_data(ann, data_test);
    fann_descale_train(ann, data_test);
    double mse = 0;
    for(i = 0; i < fann_length_train_data(data_test); i++){
        fann_scale_input(ann, data_test->input[i]);
        outputs = fann_run(ann, data_test->input[i]);
        fann_descale_output(ann, outputs);
        
        // compare the big/small roots predicted with big/small root expected
        float r_small;
        float r_big;
        if(outputs[0] < outputs[1]){
            r_small = outputs[0];
            r_big = outputs[1];
        } else if (outputs[0] > outputs[1]){
            r_small = outputs[1];
            r_big = outputs[0];
        } else {
            r_small = outputs[0];
            r_big = r_small;
        }
        float x = fann_abs(r_small - data_test->output[i][0])*fann_abs(r_small - data_test->output[i][0]);
        float y = fann_abs(r_big - data_test->output[i][1])*fann_abs(r_big - data_test->output[i][1]);
        mse += x+y;
        printf("QUADRATIC (%.2f, %.3f), should be [%.2f, %.2f], diff_r1 = %f, diff_r2 = %f \n",
               r_small, r_big,
               data_test->output[i][0], data_test->output[i][1],
               fann_abs(r_small - data_test->output[i][0]),
               fann_abs(r_big - data_test->output[i][1]));
    }
    
    printf("Saving network.\n");
    fann_save(ann, "quad.network");

    printf("MSE on TEST_DATA UNSCALED: %f\n", mse/data_test->num_data/2);
    printf("MSE on TEST_DATA SCALED: %f\n", fann_get_MSE(ann));
    printf("Cleaning up.\n");
    fann_destroy_train(data_train);
    fann_destroy_train(data_test);
    fann_destroy(ann);
    
    return 0;
}

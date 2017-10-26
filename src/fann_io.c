///*
// * fann_io.c
// *
// *  Created on: Oct 23, 2017
// *      Author: patou
// */
#define DECIMAL_POINT       13
#define NUM_LAYERS          3
#define LEARNING_RATE       0.700
#define CONNECTION_RATE     1.00

#define NETWORK_TYPE         0
#define LEARNING_MOMENTUM    0
#define TRAINING_ALGORITHM   2
#define TRAIN_ERROR_FUNC     1
#define TRAIN_STOP_FUNC      1

#define CASCADE_OUTPUT_CHANGE_FRACTION   0.010
#define QUICKDROP_DECAY                 -0.000100
#define QUICKDROP_MU                    1.75
#define RPROP_INCREASE_FACTOR           1.2
#define RPROP_DECREASE_FACTOR           0.5
#define RPROP_DELTAMIN                  0
#define RPROP_DELTAMAX                  50
#define RPROP_DELTAZERO                 0.1
#define CASCADE_OUTPUT_STAG_EPOCH       12
#define CASCADE_CAND_CHANGE_FRACTION    0.10
#define CASCADE_CAND_STAG_EPOCH         12
#define CASCADE_MAX_OUT_EPOCH           150
#define CASCADE_MIN_OUT_EPOCH           50
#define CASCADE_MAX_CAND_EPOCH          150
#define CASCADE_MIN_CAND_EPOCH          50
#define CASCADE_NUM_CAND_GROUPS         2
#define BIT_FAIL_LIMIT                  82

//#define CASCADE_CAND_LIMIT                8192000
#define CASCADE_CAND_LIMIT              32767
#define CASCADE_WEIGHT_MULT             32767

//CASCADE_ACTIVATION_FUNCTIOS
#define CASCADE_ACTIV_FUNC_COUNT        10

#define CAF_1           3
#define CAF_2           5
#define CAF_3           7
#define CAF_4           8
#define CAF_5           10
#define CAF_6           11
#define CAF_7           14
#define CAF_8           15
#define CAF_9           16
#define CAF_10          17

//CASCADE_ACTIVATION_STEEPNESSES_COUNT
#define CASCADE_ACTIVATION_STEEPNESSES_COUNT 4

#define CAS_1 2048
#define CAS_2 4096
#define CAS_3 6144
#define CAS_4 8192

//LAYER SIZES
#define L1   3
#define L2   4
#define L3   2


#include <src/include/config.h>
#include <src/include/fann.h>
#include <src/include/fann_data.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

/**
 * Create a network for XOR.
 */
FANN_EXTERNAL struct fann *FANN_API fann_create_from_static()
{
    struct fann *ann;

    ann = fann_create_XOR();

    return ann;
}

/**
 * INTERNAL FUNCTION
 * Create a network from static variables
 * Values can be found on the XOR example folder (.net file)
 * Just replaced file scraping with declarations
 *
 * TODO: Add a header file with all the declaration to increase usability?
 */
struct fann *fann_create_XOR()
{
    uint8_t num_layers = NUM_LAYERS;
    uint8_t input_neuron;
    uint8_t i;
    uint8_t num_connections;
    uint8_t tmpVal;

    struct fann_neuron *first_neuron, *neuron_it, *last_neuron, **connected_neurons;
    fann_type *weights;
    struct fann_layer *layer_it;
    struct fann *ann = NULL;

    /* Neurons as found on the .net file. */
    uint16_t neurons[9][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {3, 5, 8192},
        {3, 5, 8192},
        {3, 5, 8192},
        {0, 5, 8192},
        {4, 5, 8192},
        {0, 5, 8192}
    };

    /* Connections as found on the .net file. */
    int32_t connections[13][2] ={
        {0, -1921},
        {1, -2261},
        {2, -839},
        {0, -12581},
        {1, -12541},
        {2, 7969},
        {0, 17804},
        {1, 15721},
        {2, 15445},
        {3, 10950},
        {4, 25575},
        {5, 28073},
        {6, -20577}
    };

    /* Layer Sizes. */
    uint8_t layer_size_store[] = {L1, L2, L3};
    uint8_t layer_size;
    
    ann = fann_allocate_structure(num_layers);
    if (ann == NULL) {
        return NULL;
    }

    ann->decimal_point = DECIMAL_POINT;
    ann->multiplier = 1 << DECIMAL_POINT;
    ann->learning_rate = NUM_LAYERS;
    ann->connection_rate = LEARNING_RATE;

    /* Network_type. */
    ann->network_type = (enum fann_nettype_enum)NETWORK_TYPE;
    ann->learning_momentum = LEARNING_MOMENTUM;
    ann->training_algorithm = (enum fann_train_enum)TRAINING_ALGORITHM;
    ann->train_error_function = (enum fann_errorfunc_enum)TRAIN_ERROR_FUNC;
    ann->train_stop_function = (enum fann_stopfunc_enum)TRAIN_STOP_FUNC;

    ann->cascade_output_change_fraction = CASCADE_OUTPUT_CHANGE_FRACTION;
    ann->quickprop_decay = QUICKDROP_DECAY;
    ann->quickprop_mu = QUICKDROP_MU;
    ann->rprop_increase_factor = RPROP_INCREASE_FACTOR;
    ann->rprop_decrease_factor = RPROP_DECREASE_FACTOR;
    ann->rprop_delta_min = RPROP_DELTAMIN;
    ann->rprop_delta_max = RPROP_DELTAMAX;
    ann->rprop_delta_zero = RPROP_DELTAZERO;
    ann->cascade_output_stagnation_epochs = CASCADE_OUTPUT_STAG_EPOCH;
    ann->cascade_candidate_change_fraction = CASCADE_CAND_CHANGE_FRACTION;
    ann->cascade_candidate_stagnation_epochs = CASCADE_CAND_STAG_EPOCH;
    ann->cascade_max_out_epochs = CASCADE_MAX_OUT_EPOCH;
    ann->cascade_min_out_epochs = CASCADE_MIN_OUT_EPOCH;
    ann->cascade_max_cand_epochs = CASCADE_MAX_CAND_EPOCH;
    ann->cascade_min_cand_epochs = CASCADE_MIN_CAND_EPOCH;
    ann->cascade_num_candidate_groups = CASCADE_NUM_CAND_GROUPS;
    ann->bit_fail_limit = BIT_FAIL_LIMIT;
    //Platform size limitation
    //ann->cascade_candidate_limit = 8192000;
    ann->cascade_candidate_limit = CASCADE_CAND_LIMIT;
    ann->cascade_weight_multiplier = CASCADE_WEIGHT_MULT;

    /*
     * Activation function.
     */
    ann->cascade_activation_functions_count = CASCADE_ACTIV_FUNC_COUNT;
    // Reallocate memory
    ann->cascade_activation_functions =
        (enum fann_activationfunc_enum *) realloc(ann->cascade_activation_functions,
        ann->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));

    if (ann->cascade_activation_functions == NULL) {
        fann_error((struct fann_error*)ann, FANN_E_CANT_ALLOCATE_MEM);
        fann_destroy(ann);
        return NULL;
    }

    uint8_t cascade_activation_functions[] = {CAF_1, CAF_2, CAF_3, CAF_4, CAF_5, CAF_6, CAF_7, CAF_8, CAF_9, CAF_10};
    for (i = 0; i < ann->cascade_activation_functions_count; i++) {
        ann->cascade_activation_functions[i] = (enum fann_activationfunc_enum)cascade_activation_functions[i];
    }

    /*
     * Activation steepness.
     */
    ann->cascade_activation_steepnesses_count = CASCADE_ACTIVATION_STEEPNESSES_COUNT;
    // Reallocate memory
    ann->cascade_activation_steepnesses =
        (fann_type *) realloc(ann->cascade_activation_steepnesses,
        ann->cascade_activation_steepnesses_count * sizeof(fann_type));

    uint16_t cascade_activation_steepnesses[] = {CAS_1,CAS_2,CAS_3,CAS_4};

    for (i = 0; i < ann->cascade_activation_steepnesses_count; i++) {
        ann->cascade_activation_steepnesses[i] = cascade_activation_steepnesses[i];
    }

    fann_update_stepwise(ann);

    i = 0;

    for (layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++) {
        layer_size = layer_size_store[i];
        i++;
        if(layer_size == 0) {
            fann_destroy(ann);
            return NULL;
        }
        // We do not allocate room here, but we make sure that
        // last_neuron - first_neuron is the number of neurons
        layer_it->first_neuron = NULL;
        layer_it->last_neuron = layer_it->first_neuron + layer_size;
        ann->total_neurons += layer_size;
    }

    ann->num_input = (unsigned int) (ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1);
    ann->num_output = (unsigned int) ((ann->last_layer - 1)->last_neuron - (ann->last_layer - 1)->first_neuron);
    if(ann->network_type == FANN_NETTYPE_LAYER) {
        // one too many (bias) in the output layer
        ann->num_output--;
    }

    /* Allocate room for the actual neurons, */
    fann_allocate_neurons(ann);
    if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM) {
        fann_destroy(ann);
        return NULL;
    }
    
    i = 0;

    last_neuron = (ann->last_layer - 1)->last_neuron;
    for (neuron_it = ann->first_layer->first_neuron; neuron_it != last_neuron; neuron_it++) {
        num_connections = neurons[i][0];
        tmpVal = neurons[i][1];
        neuron_it->activation_steepness = neurons[i][2];
        i++;

        neuron_it->activation_function = (enum fann_activationfunc_enum)tmpVal;
        neuron_it->first_con = ann->total_connections;
        ann->total_connections += num_connections;
        neuron_it->last_con = ann->total_connections;
    }

    fann_allocate_connections(ann);
    if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM) {
        fann_destroy(ann);
        return NULL;
    }

    connected_neurons = ann->connections;
    weights = ann->weights;
    first_neuron = ann->first_layer->first_neuron;

    for (i = 0; i < ann->total_connections; i++) {
        input_neuron = connections[i][0];
        weights[i] = connections[i][1];
        connected_neurons[i] = first_neuron + input_neuron;
    }

    return ann;
}


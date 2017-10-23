/*
 * fann_io.c
 *
 *  Created on: Oct 23, 2017
 *      Author: patou
 */
#include <src/include/config.h>
#include <src/include/fann.h>
#include <src/include/fann_data.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

/* Create a network for XOR.
 */
FANN_EXTERNAL struct fann *FANN_API fann_create_from_static()
{
    struct fann *ann;

    ann = fann_create_XOR();

    return ann;
}

/* INTERNAL FUNCTION
*   Create a network from static variables
*Values can be found on the XOR example folder(.net file)
*Just replaced file scraping with declarations
*TODO: Add a header file with all the declaration to increase usability?
 */
struct fann *fann_create_XOR()
{
    uint8_t num_layers = 3;
    uint8_t input_neuron;
    uint8_t i;
    uint8_t num_connections;
    uint8_t tmpVal;

    struct fann_neuron *first_neuron, *neuron_it, *last_neuron, **connected_neurons;
    fann_type *weights;
    struct fann_layer *layer_it;
    struct fann *ann = NULL;

    ann = fann_allocate_structure(num_layers);
    if(ann == NULL)
    {
        return NULL;
    }

    ann->learning_rate = 0.700;
    ann->connection_rate = 1.00;

    //network_type
    tmpVal = 0;
    ann->network_type = (enum fann_nettype_enum)tmpVal;
    ann->learning_momentum = 0;

    tmpVal = 2;
    ann->training_algorithm = (enum fann_train_enum)tmpVal;

    tmpVal = 1;
    ann->train_error_function = (enum fann_errorfunc_enum)tmpVal;

    tmpVal = 1;
    ann->train_stop_function = (enum fann_stopfunc_enum)tmpVal;

    ann->cascade_output_change_fraction = 0.010;
    ann->quickprop_decay = -0.000100;
    ann->quickprop_mu = 1.75000;
    ann->rprop_increase_factor = 1.2;
    ann->rprop_decrease_factor = 0.5;
    ann->rprop_delta_min = 0;
    ann->rprop_delta_max = 50;
    ann->rprop_delta_zero = 0.1;
    ann->cascade_output_stagnation_epochs = 12;
    ann->cascade_candidate_change_fraction = 0.010;
    ann->cascade_candidate_stagnation_epochs = 12;
    ann->cascade_max_out_epochs = 150;
    ann->cascade_min_out_epochs = 50;
    ann->cascade_max_cand_epochs = 150;
    ann->cascade_min_cand_epochs = 50;
    ann->cascade_num_candidate_groups = 2;
    ann->bit_fail_limit = 82;
    //Bit size limitation
    //Does it play a role
    //ann->cascade_candidate_limit = 8192000;
    ann->cascade_candidate_limit = 32767;
    ann->cascade_weight_multiplier = 3277;

    /*
    *Activation function
    */
    ann->cascade_activation_functions_count = 10;
    /* reallocate mem */
    ann->cascade_activation_functions =
        (enum fann_activationfunc_enum *)realloc(ann->cascade_activation_functions,
        ann->cascade_activation_functions_count * sizeof(enum fann_activationfunc_enum));
    if(ann->cascade_activation_functions == NULL)
    {
        fann_error((struct fann_error*)ann, FANN_E_CANT_ALLOCATE_MEM);
        fann_destroy(ann);
        return NULL;
    }

    uint8_t cascade_activation_functions[]= {3, 5, 7, 8, 10, 11, 14, 15, 16, 17};
    //int i;
    for (i = 0; i < ann->cascade_activation_functions_count; i++)
    {
        ann->cascade_activation_functions[i] =(enum fann_activationfunc_enum)cascade_activation_functions[i];
    }

    /*
    * Activation Steepness
    */
    ann->cascade_activation_steepnesses_count = 4;
    /* reallocate mem */
    ann->cascade_activation_steepnesses =
        (fann_type *)realloc(ann->cascade_activation_steepnesses,
        ann->cascade_activation_steepnesses_count * sizeof(fann_type));

    uint16_t cascade_activation_steepnesses[] = {2048, 4096, 6144, 8192};

    for (i = 0; i < ann->cascade_activation_steepnesses_count; i++)
    {
        ann->cascade_activation_steepnesses[i] = cascade_activation_steepnesses[i];
    }

    fann_update_stepwise(ann);

    /*
    * Layer Sizes
    */
    uint8_t layer_size_store[] = {3, 4, 2};
    uint8_t layer_size;
    i=0;

    for(layer_it = ann->first_layer; layer_it != ann->last_layer; layer_it++)
    {
        layer_size = layer_size_store[i];
        i++;
        if(layer_size == 666)
        {
            fann_destroy(ann);
            return NULL;
        }
        /* we do not allocate room here, but we make sure that
         * last_neuron - first_neuron is the number of neurons */
        layer_it->first_neuron = NULL;
        layer_it->last_neuron = layer_it->first_neuron + layer_size;
        ann->total_neurons += layer_size;
    }

    ann->num_input = (unsigned int)(ann->first_layer->last_neuron - ann->first_layer->first_neuron - 1);
    ann->num_output = (unsigned int)((ann->last_layer - 1)->last_neuron - (ann->last_layer - 1)->first_neuron);
    if(ann->network_type == FANN_NETTYPE_LAYER)
    {
        /* one too many (bias) in the output layer */
        ann->num_output--;
    }

    /* allocate room for the actual neurons */
    fann_allocate_neurons(ann);
    if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
    {
        fann_destroy(ann);
        return NULL;
    }

    /*Neurons as found on the .net file*/
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
    i=0;

    last_neuron = (ann->last_layer - 1)->last_neuron;
    for(neuron_it = ann->first_layer->first_neuron; neuron_it != last_neuron; neuron_it++)
    {
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
    if(ann->errno_f == FANN_E_CANT_ALLOCATE_MEM)
    {
        fann_destroy(ann);
        return NULL;
    }

    connected_neurons = ann->connections;
    weights = ann->weights;
    first_neuron = ann->first_layer->first_neuron;

    /*Connections as found on the .net file*/
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

    for(i = 0; i < ann->total_connections; i++)
    {
        input_neuron = connections[i][0];
        weights[i] = connections[i][1];
        connected_neurons[i] = first_neuron + input_neuron;
    }

    return ann;
}


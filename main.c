#include <msp430.h>
#include <stdio.h>
#include "src/include/fixedfann.h"


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR = 0X41;    //Declare PIN0 AND PIN1 OF PORT 1 AS OUTPUT

    P1OUT = 0X01;   //MAKE PIN0 HIGH AND PIN1 LOW INITIALLY

    
    /*Initialize*/
    int8_t i;
    int8_t ret = 0;

    fann_type *calc_out;
    fann_type error_d;
    struct fann *ann;
    struct fann_train_data *data;

    
    /*Reads the data of the train network*/
    ann = fann_create_from_static();
    //ann = fann_create_from_file("xor_fixed.net");
    if(!ann)
    {
        return -1;
    }

    /*Read the data used for training*/
    data = fann_read_train_from_static();
    //data = fann_read_train_from_file("xor_fixed.data");
    

    for(i = 0; i < fann_length_train_data(data); i++)
    {
        fann_reset_MSE(ann);
        
        calc_out = fann_test(ann, data->input[i], data->output[i]);
        
        /*Calculates error between results and data*/
        error_d  = (float)( fann_abs(calc_out[0] - data->output[i][0]) / fann_get_multiplier(ann));

        #ifdef DEBUG
        printf("XOR test (%d, %d) -> %d, should be %d, difference=%d\n",
             data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
             error_d);
        #endif

        if( error_d > 0.2)
        {
            //Test failure
            ret = -1;
        }else{

            //Test Success 
            P1OUT ^=0X41;

        }
       }

    /*Finalize*/
    fann_destroy_train(data);
    fann_destroy(ann);

    return ret;

}

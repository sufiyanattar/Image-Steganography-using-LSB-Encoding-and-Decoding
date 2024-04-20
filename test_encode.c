/* NAME : SUFIYAN ATTAR
DATE : 20/04/2024
*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>


int main(int argc ,char *argv[])
{
    //strcuture varibale
    EncodeInfo encInfo;

    //option validation
    if(check_operation_type(argv) == e_encode)
    {
	// Input argument validation
	if (argc == 4 || argc == 5)
	{
	    //check and validate the filenames 
	    if (read_and_validate_encode_args(argv,&encInfo) == e_success)
	    {
		//start encoding
		if (do_encoding(&encInfo) == e_success)
		{
		    //after successfull operation close the files
		    fclose(encInfo.fptr_src_image);
		    fclose(encInfo.fptr_secret);
		    fclose(encInfo.fptr_stego_image);
		    printf("INFO: ## Encoding Done Successfully ##\n");
		}
		else
		{
		    printf("ERROR: Failed to encode\n");
		    return 1;
		}
	    }
	    else
	    {
		printf("ERROR: Failed to read to and validate encode arguments\n");
		return 1;
	    }
	}
	else
	{
	    printf("ERROR: Please the pass the argument\nLike ./a.out -e beautiful.bmp secret.txt\n");
	    return 1;
	}
    }

    //option validation
    else if(check_operation_type(argv) == e_decode)
    {
	DecodeInfo decInfo;

	// Input argument validation
	if (argc == 3 || argc == 4)
	{
	    //check and validate the filenames
	    if (read_and_validate_decode_args(argv,&decInfo) == e_success)
	    {
		//start decoding
		if (do_decoding(&decInfo) == e_success)
		{
		    fclose(decInfo.fptr_stego_image);
		    fclose(decInfo.fptr_decoded_file);
		    printf("INFO: ## Decoding Done Successfully ##\n");
		    return 1;
		}
		else
		{
		    printf("ERROR: Failed to decode\n");
		    return 1;
		}
	    }
	    else
	    {
		printf("ERROR: Failed to read to and validate decode arguments\n");
		return 1;
	    }
	}
	else
	{
	    printf("ERROR: Please pass the argument\nLike ./a.out -d stego_image.bmp\n");
	    return 1;
	}
    }
    else
    {
	printf("ERROR: Please pass the valid argument like -e or -d\n");
	return 1;
    }
    return 0;
}

/* Check for operation to be performed
 * Input: Command line arguments
 * Output: Operation to be performed
 * Return: integer constant coresponding to operation
 */
OperationType check_operation_type(char *argv[])
{

    if (strcmp(argv[1],"-e") == 0)
    {
	return e_encode;
    }
    else if (strcmp(argv[1],"-d") == 0)
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}



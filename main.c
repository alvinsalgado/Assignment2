#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#define size 9

typedef struct
{
   int *grid;
   int row;
   int column;
   int return_value;
}

parameters;

void *check_columns(void *param)
{

   int find_number[9];

   int i,column,row,num;

   parameters *data=(parameters *)param;
   int *grid;
   grid=data->grid;

   for(column=0;column<size;column++)
   {

       for(i=0;i<9;i++)
           find_number[i]=0;

       for(row=0;row<size;row++)
       {
           num=grid[row*size+column];
           if(find_number[num-1])
           {
               data->return_value=0;
               pthread_exit(NULL);
           }
           else
               find_number[num-1]=1;
       }
   }


   data->return_value=1;
   pthread_exit(NULL);

}

void *check_rows(void *param)
{

   int find_number[9];

   int i,column,row,num;

   parameters *data=(parameters *)param;
   int *grid;
   grid=data->grid;

   for(row=0;row<size;row++)
   {
       for(i=0;i<9;i++)
           find_number[i]=0;

       for(column=0;column<size;column++)
       {
           num=grid[row*size+column];
           if(find_number[num-1])
           {
               data->return_value=0;
               pthread_exit(NULL);
           }
           else
               find_number[num-1]=1;
       }
   }


   data->return_value=1;
   pthread_exit(NULL);

}

void *check_subgrid(void *param)
{

   int find_number[9];

   int i,column,row,num;

   parameters *data=(parameters *)param;
   int *grid;
   grid=data->grid;

   for(row=0;row<3;row++)
   {

       for(i=0;i<9;i++)
           find_number[i]=0;

       for(column=0;column<3;column++)
       {
           num=grid[row*size+column];
           if(find_number[num-1])
           {
               data->return_value=0;
               pthread_exit(NULL);
           }
           else
               find_number[num-1]=1;
       }
   }


   data->return_value=1;
   pthread_exit(NULL);

}

int main()
{

   int grid[size][size] = {
                            6,5,3,1,2,8,7,9,4,
                            1,7,4,3,5,9,6,8,2,
                            9,2,8,4,6,7,5,3,1,
                            2,8,6,5,1,4,3,7,9,
                            3,9,1,7,8,2,4,5,6,
                            5,4,7,6,9,3,2,1,8,
                            8,6,5,2,3,1,9,4,7,
                            4,1,2,9,7,5,8,6,3,
                            7,3,9,8,4,6,1,2,5
                          };
	int i,j,tid,row_or_column;

	int subgrids_in_line=(size/3),num_subgrids=subgrids_in_line*subgrids_in_line,num_threads=num_subgrids+2;


	pthread_t *threads=(pthread_t *)malloc(num_threads*sizeof(pthread_t));


	parameters *params=(parameters *) malloc(num_threads*sizeof(parameters));


	params[0].grid=&grid[0][0];
	params[1].grid=&grid[0][0];

	row_or_column = pthread_create(threads, NULL, check_rows, (void *)&params[0]);
	if (row_or_column)
	{
		printf("ERROR; return code from pthread_create() is %d\n", row_or_column);
		exit(-1);
	}

	row_or_column = pthread_create(&threads[1], NULL, check_columns, (void *)&params[1]);
	if (row_or_column)
	{
		printf("\nERROR; return code from pthread_create() is %d\n", row_or_column);
		exit(-1);
	}

	tid=2;
	for(i=0;i<size;i+=3)
	{

		for(j=0;j<size;j+=3)
		{
			params[tid].grid=&grid[0][0];
			params[tid].row=i;
			params[tid].column=j;
			row_or_column = pthread_create(&threads[tid], NULL, check_subgrid, (void *)&params[tid]);
			if (row_or_column)
			{
				printf("\nERROR; return code from pthread_create() is %d\n", row_or_column);
				exit(-1);
			}

			tid++;
       }
   }


		for(i=0;i<num_threads;i++)
		{
			pthread_join(threads[i],NULL);
		}



	int valid=1;
		for(i=0;i<num_threads;i++)
		{
			if(params[i].return_value==0)
			valid=0;
		}

		if(valid)
		{
			printf("\n\nThe given sudoku is VALID ");
		}
		else
		{
			printf("\n\nThe given sudoku is INVALID ");
		}

		pthread_exit(NULL);
}

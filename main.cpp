#include <cstdio>
#include <ctime>
#include "rb_tree/rb_tree.h"
#include "student/student.h"

#define ERROR (-1)

#define SCAN_LEVEL_PRINT_FORMAT "-L%d"

int find_level_print(const char **array, const int size, int *level_print)
{
	for( int i = 0; i<size; i++ )
	{
		if( sscanf(array[i], SCAN_LEVEL_PRINT_FORMAT, level_print)==1 )
			return 1;
	}

	return 0;
}
int find_param(const char **array, const int size, int *param)
{
	for( int i = 0; i<size; i++ )
	{
		if( sscanf(array[i], "%d", param)==1 )
			return 1;
	}

	return 0;
}

int main(const int argc, const char **argv)
{
	int level_print = DEFAULT_LEVEL_PRINT, param;
	int param_set = find_param(argv + 1, argc - 2, &param);
	int num_options = param_set + find_level_print(argv + 1, argc - 2, &level_print);

	if( argc<2 || num_options!=(argc - 2) )
	{
		fprintf(stderr, "Usage: %s [-Llevel_print] [parameter] filename\n", argv[0]);
		fprintf(stderr, "\tDefault: %s -L%d [parameter] filename\n", argv[0], DEFAULT_LEVEL_PRINT);
		return ERROR;
	}
	const char *filename = argv[argc - 1];

	rb_tree<student> *tr = new rb_tree<student>;
	if( tr==nullptr )
	{
		fprintf(stderr, "Not enough memory!\n");
		return ERROR;
	}

	int res_read;
	clock_t begin, end;
	begin = clock();
	if( (res_read = tr->read(filename))!=rb_tree<student>::SUCCESS )
	{
		delete tr;

		switch( res_read )
		{
		case rb_tree<student>::ALLOC_ERROR:
			fprintf(stderr, "Not enough memory!\n");
			break;
		case rb_tree<student>::OPEN_ERROR:
			fprintf(stderr, "Can't open file %s!\n", filename);
			break;
		case rb_tree<student>::READ_ERROR:
			fprintf(stderr, "Can't read student from %s!\n", filename);
			break;
		default:
			fprintf(stderr, "Unknown error with code %d!\n", res_read);
		}

		return ERROR;
	}
	end = clock();
	printf("Reading complete.\n");
	if( level_print>0 )
	{
		printf("Result:\n");
		tr->print(level_print);
	}
	printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

	if( param_set )
	{
		int total_size_sbtrs_nodes_ng, total_size_sbtrs_levels_ng,
			total_size_sbtrs_width_ng, num_values_level, total_size_paths_len;

		begin = clock();
		total_size_sbtrs_nodes_ng = tr->total_size_sbtrs_nodes_ng(param);
		end = clock();
		printf("\nTotal number of values in subtrees with number of nodes non-greater than %d calculated.\n", param);
		printf("Result: %d\n", total_size_sbtrs_nodes_ng);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_sbtrs_levels_ng = tr->total_size_sbtrs_levels_ng(param);
		end = clock();
		printf("\nTotal number of values in subtrees with number of levels non-greater than %d calculated.\n", param);
		printf("Result: %d\n", total_size_sbtrs_levels_ng);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_sbtrs_width_ng = tr->total_size_sbtrs_width_ng(param);
		end = clock();
		printf("\nTotal number of values in subtrees with width non-greater than %d calculated.\n", param);
		printf("Result: %d\n", total_size_sbtrs_width_ng);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		num_values_level = tr->num_values_level(param);
		end = clock();
		printf("\nNumber of values on level %d calculated.\n", param);
		printf("Result: %d\n", num_values_level);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_paths_len = tr->total_size_paths_len(param);
		end = clock();
		printf("\nTotal number of values in paths length %d from root to leaf nodes calculated.\n", param);
		printf("Result: %d\n", total_size_paths_len);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);
	}

	delete tr;

	return 0;
}

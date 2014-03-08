#include "container_test.h"
#include "graph_test.h"
#include "lexer_test.h"
#include "matrix_test.h"
#include "parser_test.h"
#include "sort_test.h"
#include "string_test.h"
#include "subcc_test.h"

int main(int argc, char* argv[]) {
	lib_calvin_container::containerTest();
	lib_calvin_graph::graphTest();
	lib_calvin_matrix::matrixTest();
	lib_calvin_sort::sortTest();
	lib_calvin_string::stringTest();
	lib_calvin_lexer::lexerTest();
	lib_calvin_parser::parserTest();
	subcc::subccTest(1, NULL);
	std::cout << "\n\nlib_calvin test completed!\n";
	return 0;
}

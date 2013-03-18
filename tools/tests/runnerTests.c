#include <cgreen/cgreen.h>

#ifdef __cplusplus
namespace cgreen {
#endif

#include "../runner.c"

Describe(Runner);

BeforeEach(Runner){}

AfterEach(Runner){}

#define CONTEXT_NAME "context"
#define TEST_NAME "test"

static char SPEC_NAME[] = CGREEN_SPEC_PREFIX CGREEN_SEPARATOR CONTEXT_NAME CGREEN_SEPARATOR TEST_NAME CGREEN_SEPARATOR;
static char SPEC_NAME_WITH_DEFAULT_CONTEXT[] = CGREEN_SPEC_PREFIX CGREEN_SEPARATOR CGREEN_DEFAULT_SUITE CGREEN_SEPARATOR TEST_NAME CGREEN_SEPARATOR; 


Ensure(Runner, can_get_context_name_from_specification_name) {
    char *context_name = context_name_from_specname(SPEC_NAME);

    assert_that(context_name, is_equal_to_string(CONTEXT_NAME));

    free(context_name);
}


Ensure(Runner, can_get_test_name_from_symbol) {
    assert_that(test_name_from_specname(SPEC_NAME), is_equal_to_string(TEST_NAME));
}


Ensure(Runner, can_get_context_name_of_name) {
    char *context_name = context_name_of("Context:Test");
    assert_that(context_name, is_equal_to_string("Context"));
    free(context_name);

    context_name = context_name_of("Test");
    assert_that(context_name, is_equal_to_string(CGREEN_DEFAULT_SUITE));
    free(context_name);
}


Ensure(Runner, can_get_test_name_of_symbolic_name) {
    char *test_name = test_name_of("Context:Test");
    assert_that(test_name, is_equal_to_string("Test"));
    free(test_name);

    test_name = test_name_of("Test");
    assert_that(test_name, is_equal_to_string("Test"));
    free(test_name);
}


Ensure(Runner, can_mangle_default_context_and_test_name_into_function_name) {
    char *mangled_test_name = mangle_test_name(TEST_NAME);

    assert_that(mangled_test_name, is_equal_to_string(SPEC_NAME_WITH_DEFAULT_CONTEXT));

    free(mangled_test_name);
}


Ensure(Runner, can_mangle_explicit_context_and_test_name_into_function_name) {
    char *mangled_test_name = mangle_test_name(CONTEXT_NAME ":" TEST_NAME);

    assert_that(mangled_test_name, is_equal_to_string(SPEC_NAME));

    free(mangled_test_name);
}


Ensure(Runner, can_identify_cgreen_spec) {
    assert_that(is_cgreen_spec("_CgreenSpec__Runner__can_get_test_name_from_function__"));
}


Ensure(Runner, can_identify_a_cxx_mangled_cgreen_spec) {
    assert_that(is_cgreen_spec("__ZN6cgreen51CgreenSpec__Runner__can_get_test_name_from_function__E"));
}


Ensure(Runner, can_register_context_and_test_from_a_symbol) {
    TestItem test_items[2] = {{NULL, NULL, NULL}};
    register_test(test_items, 2, SPEC_NAME);
    assert_that(test_items[0].context_name, is_equal_to_string(CONTEXT_NAME));
    assert_that(test_items[0].test_name, is_equal_to_string(TEST_NAME));

    // TODO: make destroy_test_item(TestItem*)
    free(test_items[0].context_name);
    free(test_items[0].symbol);
    free(test_items[0].test_name);
}

Ensure(Runner, can_ensure_test_exists_from_context_and_name) {
    TestItem test_items[5] = {{(char *)"", (char *)"Context1", (char *)"Test1"},
                              {(char *)"", (char *)"Context1", (char *)"Test2"},
                              {(char *)"", (char *)"Context2", (char *)"Test1"},
                              {(char *)"", (char *)"Context2", (char *)"Test2"},
                              {NULL, NULL, NULL}};
    assert_that(matching_test_exists("Context1:Test1", test_items));
}

Ensure(Runner, can_match_test_name) {
    TestItem test_item = {(char *)"", (char *)"Context1", (char *)"Test1"};

    assert_that(test_matches_pattern("Context1:Test1", test_item));
    assert_that(test_matches_pattern("Context*:Test1", test_item));
    assert_that(test_matches_pattern("*:Test1", test_item));

    assert_that(test_matches_pattern("Context*:Test1", test_item));
    assert_that(test_matches_pattern("*:Test1", test_item));

    assert_that(test_matches_pattern("Context1:Test*", test_item));
    assert_that(test_matches_pattern("Context*:Test*", test_item));
    assert_that(test_matches_pattern("Context*:*", test_item));
    assert_that(test_matches_pattern("*:Test*", test_item));
    assert_that(test_matches_pattern("*:*", test_item));
}

Ensure(Runner, can_add_test_to_the_suite_for_its_context) {
	ContextSuite *suite_list = NULL;
	CgreenTest *test = (CgreenTest *)&test;
	TestSuite *parent_suite = create_test_suite();
	TestSuite *first_suite, *second_suite;

	assert_that(suite_list, is_null);

	add_test_to_context(parent_suite, &suite_list, "TheFirstContext", "TheName", test);
	first_suite = suite_for_context(suite_list, "TheFirstContext");
	assert_that(first_suite, is_non_null);
	assert_that(first_suite->size, is_equal_to(1));

	second_suite = suite_for_context(suite_list, "TheSecondContext");
	assert_that(second_suite, is_null);

	add_test_to_context(parent_suite, &suite_list, "TheSecondContext", "TheName", test);
	assert_that(suite_for_context(suite_list, "TheFirstContext")->size, is_equal_to(1));
	assert_that(suite_for_context(suite_list, "TheSecondContext")->size, is_equal_to(1));

	destroy_test_suite(parent_suite);
}


#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
/* Local variables: */
/* tab-width: 4     */
/* End:             */
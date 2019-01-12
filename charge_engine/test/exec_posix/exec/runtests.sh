
test_pgm="${BUILD_ROOT}/bin/run_exec"
if [[ ! -e "${test_pgm}" ]]; then
    echo \$BUILD_ROOT is not correct configured.
    exit 2
fi

capture_fn="$(mktemp)"

function cleanup {
    rm "${capture_fn}"
}

function error {
    echo "###################"
    echo "   Tests failed!"
    echo "###################"
    cleanup
}


echo Running tests:


echo - Repeats passed arguments to program
"${test_pgm}" repeat_args.sh a b c > "${capture_fn}"
if ! cmp --quiet "${capture_fn}" expected_result_01; then
    error
    exit 1
fi


echo - Repeats quoted arguments
"${test_pgm}" repeat_args.sh a "b c" > "${capture_fn}"
if ! cmp --quiet "${capture_fn}" expected_result_02; then
    error
    exit 1
fi


echo - Exit zero code
if ! "${test_pgm}" exit_with_code.sh 0 > /dev/null; then
    error
    exit 1
fi


echo - Exit non-zero code
"${test_pgm}" exit_with_code.sh 22 > /dev/null
if [[ $? != 22 ]]; then
    error
    exit 1
fi


echo Ran all tests successfully.
cleanup
exit 0


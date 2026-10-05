if (TARGET gtest_main)

    add_executable(test_customDrivers
        testButtonTimedMultiple.cpp
    )

    target_link_libraries(test_customDrivers
        PRIVATE customDrivers
        PRIVATE gtest_main
    )

endif()

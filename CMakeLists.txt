cmake_minimum_required(VERSION 3.14)
project(homeworks)

set(CMAKE_CXX_STANDARD 14)

#add_executable(test
#        test/test.c)

#add_executable(homeworks
#        homeworks/01_easy/01_easy_v1.c)

#add_executable(homeworks
#        homeworks/02_easy/02_easy_v1.c)

#add_executable(homeworks
#        homeworks/03_easy/03_easy_v1.c)

#add_executable(homeworks
#        homeworks/04_easy/04_easy_v1.c)

#add_executable(homeworks
##        homeworks/05_easy/05_easy_v1.c)
#add_executable(homeworks
#        homeworks/05_easy/05_easy_v2.c)

#add_executable(homeworks
#        homeworks/05_hard/05_hard_v2.c)

#add_executable(homeworks
#        homeworks/06_easy/06_easy_v3.c)

#add_executable(homeworks
#        homeworks/06_hard/06_hard_v1.c)

#add_executable(homeworks
#        homeworks/07_hard/07_hard_v2.c)


set(SOURCE_FILES homeworks/06/06_hard/06_hard_v1-for_review.c)
add_executable(homeworks ${SOURCE_FILES})

target_link_libraries(homeworks m)
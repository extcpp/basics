#pragma once
#ifndef OBI_ALGORITHM_STRING_DISTANCES
#define OBI_ALGORITHM_STRING_DISTANCES

#include <string>
#include <vector>

namespace obi { namespace algorithm { namespace distances {

    // For Unicode we would require lib ICU

    template<typename T> //chars get copied
    T min_of_3(T a, T b, T c){
        return a < b ? std::min(a, c) : std::min(b, c);
    }

	//simple length
    template<typename T>
    int length(const T& first, const T& second)
    {
        //needs to be cast to int or substraction will habe a problem!
        return abs((int)first.length() - (int)second.length());
    }

    //edit distance fast implementation
    template<typename T>
    int edit_fast(const T& first, const T& second){
        const int items_per_col   = first.length();
        const int number_of_cols  = second.length();

        std::vector<int> previous_col(items_per_col + 1);
        std::vector<int> current_col(items_per_col + 1);

        for (int i = 0; i <= items_per_col; i++){
            previous_col[i] = i;
        }

        for (int j = 1; j <= number_of_cols; j++) {
            current_col[0] = j;
            for (int i = 1; i <= items_per_col; i++){
                current_col[i] = min_of_3(
                     current_col[i-1] + 1,  //delete
                    previous_col[ i ] + 1,  //insert
                    previous_col[i-1] + (
                        first.at(i-1) == second.at(j-1) ?
                            0 :             //equal
                            1               //replace
                    )
                );
            }
            current_col.swap(previous_col);
        }
        return previous_col[items_per_col];
    } // edit_fast

    template<typename T>
    int edit_matrix(const T& first, const T& second){
        // requirese more Memory
        int items_per_col  = first.length();
        int number_of_cols = second.length();

        std::vector<std::vector<int>> matrix(
            items_per_col + 1,
            std::vector<int>(number_of_cols + 1)
        );

        for (int i = 0; i <= items_per_col; ++i){
            matrix[i][0] = i;
        }

        for (int j = 0; j <= number_of_cols; ++j){
            matrix[0][j] = j;
        }

        for (int j = 1; j <= number_of_cols; ++j){
            for (int i = 1; i <= items_per_col; ++i){
                matrix[i][j] = min_of_3(
                    matrix[i-1][ j ] + 1,
                    matrix[ i ][j-1] + 1,
                    matrix[i-1][j-1] + (first[i-1] == second[j-1] ? 0 : 1)
                );
            }
        }
        return matrix[items_per_col][number_of_cols];
    } // edit_matrix

    template<typename T>
    int edit(const T& first, const T& second){
        return edit_matrix(first, second);
    }
}}} // obi::algorithm::distances
#endif

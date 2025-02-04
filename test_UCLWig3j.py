#!/usr/bin/env python


""" 
    Routines for testing UCLWig3j results. See README.md for more details.
    Author: Lorne Whiteway.
"""


import subprocess
import random
import sys


# Returns 3j(j1, j2, j, m, -m, 0) (calculated using run_UCLWig3j).
def UCLWig3j_wrapper(j1, j2, j, m):
    return float(subprocess.check_output(["./run_UCLWig3j", str(j1), str(j2), str(j), str(m)]))


# Returns 3j(j1, j2, j, m, -m, 0) (calculated using wigxjpf).
def wigxjpf_wrapper(j1, j2, j, m):
    cmd = "--3j=" + str(j1) + "," + str(j2) + "," + str(j) + "," + str(m) + "," + str(-m) + "," + str(0)
    string_data = subprocess.check_output(["../wigxjpf-1.7/bin/wigxjpf", cmd])
    # TODO: Add error check on return.
    string_data = string_data.replace("trivially", "")
    return float(string_data.split("=")[1])


def generate_test_cases(maxj12, f_handle):
    for m in [0, 2]:
        for j1 in range(maxj12):
            for j2 in range(maxj12):
                j_max = max(j1, j2)
                j_min = min(j1, j2)
                for j in range(j_max - j_min, j_max + j_min + 1):
                    f_handle.write(str(j1) + "," + str(j2) + "," + str(j) + "," + str(m) + "," + str(-m) + "," + str(0) + "," + "%.18e"%wigxjpf_wrapper(j1, j2, j, m) + "\n")


def generate_random_test_cases(num_cases, maxj12, f_handle):
    for i in range(num_cases):
        j1 = random.randint(0, maxj12)
        j2 = random.randint(0, maxj12)
        j_max = max(j1, j2)
        j_min = min(j1, j2)
        j = random.randrange(j_max - j_min, j_max + j_min + 1)
        m = random.choice((0, 2))
        f_handle.write(str(j1) + "," + str(j2) + "," + str(j) + "," + str(m) + "," + str(-m) + "," + str(0) + "," + "%.18e" % wigxjpf_wrapper(j1, j2, j, m) + "\n")


def test_file_name():
    return "./test_UCLWig3j.txt"


def generate_all_test_cases():
    with open(test_file_name(), "w") as f_handle:
        generate_test_cases(25, f_handle)
        generate_random_test_cases(20000, 10000, f_handle)


# Assumes that parameter_tuple is an 8-tuple containing: j1, j2, j, m, correct wig3j, our calculated wig3j, absolute error, percentage error.
def parameters_to_str(parameter_tuple):
    return "j1 = " + str(parameter_tuple[0]) + "; j2 = " + str(parameter_tuple[1]) + "; j = " + str(parameter_tuple[2]) + "; m = " + str(parameter_tuple[3]) + "; correct wig3j = " + "{0:.17e}".format(parameter_tuple[4]) + "; our calculated wig3j = " + "{0:.17e}".format(parameter_tuple[5]) + "; absolute error = " + str(parameter_tuple[6]) + "; percentage error = " + str(parameter_tuple[7])


# From https://stackoverflow.com/questions/845058
def lines_in_file(filename):
    with open(filename, "r") as f_handle:
        lines = 0
        buf_size = 1024 * 1024
        read_f = f_handle.read # loop optimization
        buf = read_f(buf_size)
        while buf:
            lines += buf.count('\n')
            buf = read_f(buf_size)
    return lines


def run_all_test_cases():
    max_absolute_error = (0, 0, 0, 0, 0, 0, 0, 0.0)
    max_percentage_error = (0, 0, 0, 0, 0, 0, 0, 0.0)
    max_absolute_error_when_correct_value_is_zero = (0, 0, 0, 0, 0, 0, 0, 0.0)
    
    print("Running...")
    
    # To provide a running status update
    lines = lines_in_file(test_file_name())
    lines_read = 0
    old_percentage_read = -1
    
    with open(test_file_name(), "r") as f_handle:
        for line in f_handle:
            arr = line.split(",")
            j1 = int(arr[0])
            j2 = int(arr[1])
            j = int(arr[2])
            m = int(arr[3])
            correct_wig3j = float(arr[6])
            our_calculated_wig3y = UCLWig3j_wrapper(j1, j2, j, m)
            
            absolute_error = abs(our_calculated_wig3y - correct_wig3j)
            percentage_error = abs(our_calculated_wig3y/correct_wig3j - 1.0) if (correct_wig3j != 0.0) else 0.0
            
            this_trial = (j1, j2, j, m, correct_wig3j, our_calculated_wig3y, absolute_error, percentage_error)
            
            if absolute_error > max_absolute_error[6]:
                max_absolute_error = this_trial
               
            if correct_wig3j != 0.0 and percentage_error > max_percentage_error[7]:
                max_percentage_error = this_trial
                
            if correct_wig3j == 0.0 and absolute_error > max_absolute_error_when_correct_value_is_zero[6]:
                max_absolute_error_when_correct_value_is_zero = this_trial
            
            # To provide a running status update
            lines_read += 1
            percentage_read = int(100.0 * lines_read / float(lines))
            if (percentage_read > old_percentage_read):
                sys.stdout.write("\r" + "X" * percentage_read + "." * (100 - percentage_read))
                sys.stdout.flush()
                old_percentage_read = percentage_read
            
                
    print("\n")
    
    print("Maximum absolute error:")
    print(parameters_to_str(max_absolute_error))
            
    print("Maximum percentage error:")
    print(parameters_to_str(max_percentage_error))
            
    print("Maximum absolute error when correct value is zero:")
    print(parameters_to_str(max_absolute_error_when_correct_value_is_zero))


# MAIN PROCEDURE STARTS HERE

if __name__ == "__main__":

    usage_string = "Usage: 'test_UCLWig3j.py generate' or 'test_UCLWig3j.py run'"
    
    if len(sys.argv) != 2:
        print(usage_string)
    else:
        if sys.argv[1] == "generate":
            generate_all_test_cases()
        elif sys.argv[1] == "run":
            run_all_test_cases()
        else:
            print(usage_string)



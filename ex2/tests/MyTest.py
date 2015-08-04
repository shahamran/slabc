from random import randint, random
from math import pi, tan
DIMENSION = (2, 2)
points = []

def my_line(x):
    return 0

def get_sign(line, vector):
    return 1 if float(vector[1]) > line(vector[0]) else -1

def get_random_num():
    return random() * (randint(1,21) - 11)

def get_unsigned_vector(n):
    vec = ''
    for i in range(n - 1):
        vec += str(get_random_num()) + ","
    vec += str(get_random_num())
    return vec

def get_signed_vector(n):
    vec = ''
    for i in range(n):
        vec += str(get_random_num()) + ","
    vec += str(get_sign(my_line, vec.split(",")))
    return vec

def make_test_file(file_name):
    f = open(file_name, "w")
    n = randint(DIMENSION[0], DIMENSION[1])
    f.write(str(n) + "\n")
    m = randint(5, 20)
    f.write(str(m) + "\n")
    for i in range(m):
        f.write(get_signed_vector(n) + "\n")
    k = randint(5, 15)
    for i in range(k):
        f.write(get_unsigned_vector(n) + "\n")
    f.close()

def main():
    for i in range(10):
        make_test_file("test" + str(i + 1) + ".in")

if __name__ == "__main__":
    main()

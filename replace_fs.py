Import("env")
print("Replace MKSPIFFSTOOL with mklittlefs")
env.Replace (MKSPIFFSTOOL = "./mklittlefs")
